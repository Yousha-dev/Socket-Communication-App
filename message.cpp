#include "message.h"
#include "qsqlquerymodel.h"
#include "ui_message.h"
#include "menupage.h"
#include <QtGui>
#include <QtCore>

QString name;
Message::Message(QWidget *parent,QString n) :
    QDialog(parent),
    ui(new Ui::Message)
{
    ui->setupUi(this);
    name=n;
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("/home/ubuntu/OS_Project/register.sqlite");
    database.open();
    QSqlQuery qry;
    qry.exec("SELECT * From "+name);
    while(qry.next())
    {
        ui->combobox_receivers->addItem(qry.value(0).toString());
    }
    qry.clear();
    database.close();
    socket = new QTcpSocket(this);
    connect(this, &Message::newMessage, this, &Message::displayMessage);
    connect(socket, &QTcpSocket::readyRead, this, &Message::readSocket);
    connect(socket, &QTcpSocket::disconnected, this, &Message::discardSocket);
    connect(socket, &QAbstractSocket::errorOccurred, this, &Message::displayError);
    socket->connectToHost(QHostAddress::LocalHost,8080);
    if(socket->waitForConnected()){
        ui->textBrowser_receivedMessages->append("Connected\n");
        if(socket)
        {
            if(socket->isOpen())
            {
                QString message = name;
                QDataStream socketStream(socket);
                QByteArray header;
                header.prepend(QString("fileType:login,fileName:null,fileSize:%1;").arg(message.size()).toUtf8());
                header.resize(128);
                QByteArray byteArray = message.toUtf8();
                byteArray.prepend(header);
                socketStream << byteArray;
                ui->textBrowser_receivedMessages->append("You logged in\n");
                ui->lineEdit_message->clear();
            }
            else
                QMessageBox::critical(this,"Error","404");
        }
        else
            QMessageBox::critical(this,"Error","Not connected");
    }

    else{
        QMessageBox::critical(this,"Error", QString("%1.").arg(socket->errorString()));
        menupage *new_win = new menupage(0);
        new_win->setModal(true);
        new_win->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
        new_win->exec();
        exit(EXIT_FAILURE);
    }
}

Message::~Message()
{
    if(socket->isOpen())
        socket->close();
    delete ui;
}

void Message::readSocket()
{
    QByteArray buffer;

    QDataStream socketStream(socket);

    socketStream.startTransaction();
    socketStream >> buffer;

    if(!socketStream.commitTransaction())
    {
        QString message = QString("Message Incoming");
        emit newMessage(message);
        return;
    }

    QString header = buffer.mid(0,128);
    QString fileType = header.split(",")[0].split(":")[1];

    buffer = buffer.mid(128);

    if(fileType=="attachment"){
        QString fileName = header.split(",")[1].split(":")[1];
        QString ext = fileName.split(".")[1];
        QString size = header.split(",")[2].split(":")[1];
        QString sender = header.split(",")[3].split(":")[1].split(";")[0];

        if (QMessageBox::Yes == QMessageBox::question(this, "OS Project", QString("You are receiving an attachment from %1 of size: %2 bytes, called %3. Do you want to accept it?").arg(sender).arg(size).arg(fileName)))
        {
            QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"/"+fileName, QString("File (*.%1)").arg(ext));

            QFile file(filePath);
            if(file.open(QIODevice::WriteOnly)){
                file.write(buffer);
                QString message = QString("%1: %2 successfully stored on %3").arg(sender).arg(fileName).arg(QString(filePath));
                emit newMessage(message);
            }else
                QMessageBox::critical(this,"Error", "An error occurred while trying to write the attachment.");
        }else{
            QString message = QString("%1: %2 discarded").arg(sender).arg(fileName);
            emit newMessage(message);
        }
    }else if(fileType=="message"){
        QString message = QString("%1").arg(QString::fromStdString(buffer.toStdString()));
        emit newMessage(message);
    }
}

void Message::discardSocket()
{
    socket->deleteLater();
    socket=nullptr;
    ui->textBrowser_receivedMessages->append("Disconnected!\n");
}

void Message::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, "Error", "Server is not running");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, "Error", "Connection refused");
        break;
    default:
        QMessageBox::information(this, "Error", QString("%1.").arg(socket->errorString()));
        break;
    }
}

void Message::displayMessage(const QString& str)
{
    ui->textBrowser_receivedMessages->append(str);
}


void Message::on_backButton_clicked()
{
    if(socket->isOpen())
        socket->close();
    delete ui;
    hide();
    menupage *newwin = new menupage(0,name);
    newwin->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    newwin->show();

}


void Message::on_pushButton_add_clicked()
{
    database.open();
    QString name_input = ui->lineEdit_contactadd->text();
    QSqlQuery qry;
    QString query = "INSERT INTO "+name+" (Name) SELECT :name WHERE NOT EXISTS (SELECT 1 FROM "+name+" WHERE Name = :name)";
    qry.prepare(query);
    qry.bindValue(":name", name_input);

    if(qry.exec())
    {
        if (qry.numRowsAffected() > 0)
        {
            QMessageBox::information(this, "Success", "Contact Added");
            ui->combobox_receivers->addItem(name_input);
        }
        else
        {
            QMessageBox::information(this, "Failed", "Contact already exists");
        }
    }
    else
    {
        QMessageBox::information(this, "Failed", "404");
    }
    database.close();
}

void Message::on_pushButton_sendMessage_clicked()
{
    if(socket)
    {
        if(socket->isOpen())
        {
            QString message = ui->lineEdit_message->text();
            QString receiver = ui->combobox_receivers->currentText();
            if(message==""||receiver==""){
                ui->textBrowser_receivedMessages->append("Please enter receiver");
                return;
            }
            QDataStream socketStream(socket);
            QByteArray header;
            header.prepend(QString("fileType:message,fileName:null,fileSize:%1,receiver:%2;").arg(message.size()).arg(receiver).toUtf8());
            header.resize(128);

            QByteArray byteArray = message.toUtf8();
            byteArray.prepend(header);

            socketStream << byteArray;
            ui->textBrowser_receivedMessages->append("You(To "+receiver+"): "+message+"\n");

            ui->lineEdit_message->clear();
        }
        else
            QMessageBox::critical(this,"Error","404");
    }
    else
        QMessageBox::critical(this,"Error","Not connected");
}

void Message::on_pushButton_sendAttachment_clicked()
{
    if(socket)
    {
        if(socket->isOpen())
        {
            QString filePath = QFileDialog::getOpenFileName(this, ("Select an attachment"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), ("File (*.c *.h *.java *.html *.css *.js *.zip*.json *.txt *.png *.jpg *.jpeg)"));

            if(filePath.isEmpty()){
                QMessageBox::critical(this,"Error","You haven't selected any attachment!");
                return;
            }

            QFile m_file(filePath);
            if(m_file.open(QIODevice::ReadOnly)){
                QString receiver = ui->combobox_receivers->currentText();
                if(receiver==""){
                    ui->textBrowser_receivedMessages->append("Please enter receiver");
                    return;
                }
                QFileInfo fileInfo(m_file.fileName());
                QString fileName(fileInfo.fileName());
                QDataStream socketStream(socket);
                QByteArray header;
                header.prepend(QString("fileType:attachment,fileName:%1,fileSize:%2,receiver:%3;").arg(fileName).arg(m_file.size()).arg(receiver).toUtf8());
                header.resize(128);
                QByteArray byteArray = m_file.readAll();
                byteArray.prepend(header);
                socketStream << byteArray;
                ui->textBrowser_receivedMessages->append("You(To "+receiver+"): Sent "+fileName+"\n");
            }else
                QMessageBox::critical(this,"Error","Attachment is not readable");
        }
        else
            QMessageBox::critical(this,"Error","404");
    }
    else
        QMessageBox::critical(this,"Error","Not connected");
}
