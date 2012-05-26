#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <list>
#include <QString>
#include <QSplitter>
#include <QListView>
#include <QTextEdit>
#include <QTreeView>
#include <QTextDocument>
#include <QtWebKit/QWebView>

//MongoDB C++ Client
#include "client/dbclient.h"

#include "treemodel.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mongo::DBClientConnection c;
    c.connect("127.0.0.1");
    //mongo::BSONObjBuilder b;

    std::list<std::string> dbs;
    dbs = c.getDatabaseNames();
    QStringList toform;
    for (std::list<std::string>::iterator it=dbs.begin();it!=dbs.end();++it) {
        QString db = (*it).c_str();
        toform << db;

        std::list<std::string> collections;
        collections = c.getCollectionNames((*it).c_str());
        for (std::list<std::string>::iterator ct=collections.begin();ct!=collections.end();++ct) {
            QString col = (*ct).c_str();
            toform << " "+col;
        }

    }


    QSplitter *splitter = new QSplitter(ui->centralWidget);


    view = new QWebView(ui->centralWidget);
    view->setHtml("<HTML><BODY><h1>127.0.0.1:27017</h1>"+toform.join("<br>")+"</BODY></HTML>");
    view->show();


    TreeModel *model = new TreeModel(toform);
    //TreeModel model(toform);
    QTreeView *tree = new QTreeView(this);

    tree->setModel(model);
    tree->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(tree, SIGNAL(clicked(QModelIndex)), this, SLOT(changeCurrent(QModelIndex)));

    splitter->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    splitter->addWidget(tree);
    splitter->addWidget(view);

    setCentralWidget(splitter);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeCurrent(const QModelIndex &current)
 {

    QString dbn = current.data(Qt::DisplayRole).toString();
    statusBar()->showMessage(
    tr("woWop (%1) ")
        .arg(dbn));

    mongo::DBClientConnection c;
    c.connect("127.0.0.1");
    mongo::BSONObjBuilder b;


    QString out;

    QStringList ol;

    ol << "<h1>" << dbn << "<h1>";

    ol << "<p>count:" << tr("%1").arg(c.count(dbn.toStdString().c_str()));

    std::auto_ptr<mongo::DBClientCursor> cursor = c.query(dbn.toStdString().c_str(), mongo::BSONObj());
    while( cursor->more() )
        ol << "<div style='padding:10px;border:1px solid #ccc;margin:10px;background-color:#eee;'>" << Qt::escape(QString::fromUtf8(cursor->next().toString().c_str())) << "</div>";

    out = ol.join("\n");

    view->setHtml(out);

}
