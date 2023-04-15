#include "MainWindow.h"

#include <QUrl>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QWebSocket>
#include <QWebSocketServer>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QStatusBar>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

void MainWindow::slot_clear()
{
    outtextedit_ptr_->clear();
}

void MainWindow::slot_connect()
{
//    const QString& host {"ws://127.0.0.1"};
//    const int port {3435};
//    QUrl url(host);
//    url.setPort(port);

//    socket_ptr_->open(url);
//    QTimer::singleShot(1000*2, [this, &host]{
//        if(socket_ptr_->state()!=QAbstractSocket::ConnectedState){
//            socket_ptr_->close();
//            QString msg {"Fail to connect to host"};
//            statusBar()->showMessage(msg,2000);
//        }
//    });

    if(server_ptr_->isListening()){
        statusBar()->showMessage("Server is listening", 2000);
        return;
    }
    server_ptr_->listen(QHostAddress::Any,2435);
    statusBar()->showMessage("Server is listening", 2000);
}

void MainWindow::slot_execute()
{
    if(socket_ptr_ && socket_ptr_->state()==QAbstractSocket::ConnectedState){
        const QJsonObject& out_object{
            {"program", programlineedit_ptr_->text()},
            {"arguments", argslineedit_ptr_->text()}
        };
        socket_ptr_->sendTextMessage(QJsonDocument(out_object).toJson());
    }
}

void MainWindow::slot_interrapt()
{
    if(socket_ptr_ && socket_ptr_->state()==QAbstractSocket::ConnectedState){
        const QJsonObject& out_object{
            {"signal", "break"}
        };
        socket_ptr_->sendTextMessage(QJsonDocument(out_object).toJson());
    }
}

void MainWindow::slot_dictonnect()
{
    if(socket_ptr_){
        socket_ptr_->close();
    }
    if(server_ptr_->isListening()){
        server_ptr_->close();
        statusBar()->showMessage("Server is closed", 2000);
    }
}

void MainWindow::slot_new_connection()
{
    socket_ptr_.reset(server_ptr_->nextPendingConnection());
    QObject::connect(socket_ptr_.get(),&QWebSocket::connected,
                     this,&MainWindow::slot_connected);
    QObject::connect(socket_ptr_.get(),&QWebSocket::disconnected,
                     this,&MainWindow::slot_dictonnected);
    QObject::connect(socket_ptr_.get(),&QWebSocket::textMessageReceived,
                     this,&MainWindow::slot_textmessage_received);

    statusBar()->showMessage("Socket connected", 2000);
}

void MainWindow::slot_connected()
{

}

void MainWindow::slot_dictonnected()
{
    statusBar()->showMessage("Socket disconnected", 2000);
}

void MainWindow::slot_textmessage_received(const QString &message)
{
    outtextedit_ptr_->append(message);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    argslineedit_ptr_.reset(new QLineEdit);
    programlineedit_ptr_.reset(new QLineEdit);

    QVBoxLayout* invboxlayout_ptr {new QVBoxLayout};
    invboxlayout_ptr->addWidget(new QLabel("Program"));
    invboxlayout_ptr->addWidget(programlineedit_ptr_.get());

    invboxlayout_ptr->addWidget(new QLabel("Arguments"));
    invboxlayout_ptr->addWidget(argslineedit_ptr_.get());

    clearbtn_ptr_.reset(new QPushButton("Clear"));
    QObject::connect(clearbtn_ptr_.get(),&QPushButton::clicked,
                     this,&MainWindow::slot_clear);

    connectbtn_ptr_.reset(new QPushButton("Open"));
    QObject::connect(connectbtn_ptr_.get(),&QPushButton::clicked,
                     this,&MainWindow::slot_connect);

    executebtn_ptr_.reset(new QPushButton("Execute"));
    QObject::connect(executebtn_ptr_.get(),&QPushButton::clicked,
                     this,&MainWindow::slot_execute);

    interraptbtn_ptr_.reset(new QPushButton("Interrapt"));
    QObject::connect(interraptbtn_ptr_.get(),&QPushButton::clicked,
                     this,&MainWindow::slot_interrapt);

    disconnectbtn_ptr_.reset(new QPushButton("Close"));
    QObject::connect(disconnectbtn_ptr_.get(),&QPushButton::clicked,
                     this,&MainWindow::slot_dictonnect);

    QVBoxLayout* btnvboxlayout_ptr {new QVBoxLayout};
    btnvboxlayout_ptr->addWidget(connectbtn_ptr_.get());
    btnvboxlayout_ptr->addWidget(disconnectbtn_ptr_.get());
    btnvboxlayout_ptr->addWidget(executebtn_ptr_.get());
    btnvboxlayout_ptr->addWidget(interraptbtn_ptr_.get());
    btnvboxlayout_ptr->addWidget(clearbtn_ptr_.get());

    QHBoxLayout* tophboxlayout_ptr {new QHBoxLayout};
    tophboxlayout_ptr->addLayout(invboxlayout_ptr,5);
    tophboxlayout_ptr->addLayout(btnvboxlayout_ptr,0);

    outtextedit_ptr_.reset(new QTextEdit);
    outtextedit_ptr_->setReadOnly(true);

    QHBoxLayout* outhboxlayout_ptr {new QHBoxLayout};
    outhboxlayout_ptr->addWidget(outtextedit_ptr_.get());

    QVBoxLayout* mainlayout_ptr {new QVBoxLayout};
    mainlayout_ptr->addLayout(tophboxlayout_ptr,0);
    mainlayout_ptr->addLayout(outhboxlayout_ptr,10);

    QWidget* centralwidget_ptr {new QWidget};
    centralwidget_ptr->setLayout(mainlayout_ptr);
    setCentralWidget(centralwidget_ptr);

    server_ptr_.reset(new QWebSocketServer("Echo server",QWebSocketServer::NonSecureMode));
    QObject::connect(server_ptr_.get(),&QWebSocketServer::newConnection,
                     this,&MainWindow::slot_new_connection);

    resize(600,600);

}
