#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>

class QLineEdit;
class QTextEdit;
class QWebSocketServer;
class QWebSocket;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QSharedPointer<QTextEdit> outtextedit_ptr_ {nullptr};
    QSharedPointer<QLineEdit> argslineedit_ptr_ {nullptr};
    QSharedPointer<QLineEdit> programlineedit_ptr_ {nullptr};

    QSharedPointer<QWebSocket> socket_ptr_ {nullptr};
    QSharedPointer<QWebSocketServer> server_ptr_ {nullptr};

    QSharedPointer<QPushButton> connectbtn_ptr_ {nullptr};
    QSharedPointer<QPushButton> executebtn_ptr_ {nullptr};
    QSharedPointer<QPushButton> interraptbtn_ptr_ {nullptr};
    QSharedPointer<QPushButton> disconnectbtn_ptr_ {nullptr};

private slots:
    void slot_connect();
    void slot_execute();
    void slot_interrapt();
    void slot_dictonnect();

    void slot_new_connection();
    void slot_connected();
    void slot_dictonnected();
    void slot_textmessage_received(const QString& message);

public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:

};

#endif // MAINWINDOW_H
