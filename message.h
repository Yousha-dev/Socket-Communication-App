#ifndef MESSAGE_H
#define MESSAGE_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QAbstractSocket>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QHostAddress>
#include <QMessageBox>
#include <QMetaType>
#include <QString>
#include <QStandardPaths>
#include <QTcpSocket>

namespace Ui {
class Message;
}

class Message : public QDialog
{
    Q_OBJECT

public:
    QSqlDatabase database;
    explicit Message(QWidget *parent = nullptr,QString n="");
    ~Message();
signals:
    void newMessage(QString);
private slots:
    void readSocket();
    void discardSocket();
    void displayError(QAbstractSocket::SocketError socketError);
    void displayMessage(const QString& str);
    void on_pushButton_sendMessage_clicked();
    void on_pushButton_sendAttachment_clicked();
    void on_backButton_clicked();
    void on_pushButton_add_clicked();

private:
    Ui::Message *ui;
    QTcpSocket* socket;
};

#endif // MESSAGE_H
