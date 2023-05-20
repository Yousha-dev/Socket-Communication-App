#include "mainwindow.h"
#include "ui_mainwindow.h"

QHash<QString, QTcpSocket*> loggedInClients;
QHash<QTcpSocket*, QString> loggedInSockets;
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_server = new QTcpServer();

    if(m_server->listen(QHostAddress::Any, 8080))
    {
       connect(this, &MainWindow::newMessage, this, &MainWindow::displayMessage);
       connect(m_server, &QTcpServer::newConnection, this, &MainWindow::newConnection);
       ui->statusBar->showMessage("Server is listening...");
    }
    else
    {
        QMessageBox::critical(this,"Socket Server",QString("Unable to start the server: %1.").arg(m_server->errorString()));
        exit(EXIT_FAILURE);
    }
}

MainWindow::~MainWindow()
{
    foreach (QTcpSocket* socket, connection_set)
    {
        socket->close();
        socket->deleteLater();
    }

    m_server->close();
    m_server->deleteLater();

    delete ui;
}

void MainWindow::newConnection()
{
    while (m_server->hasPendingConnections())
        appendToSocketList(m_server->nextPendingConnection());
}

void MainWindow::appendToSocketList(QTcpSocket* socket)
{
    connection_set.insert(socket);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readSocket);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::discardSocket);
    connect(socket, &QAbstractSocket::errorOccurred, this, &MainWindow::displayError);
    displayMessage(QString("%1 has just connected").arg(socket->socketDescriptor()));
}

void MainWindow::readSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());

    QByteArray buffer;

    QDataStream socketStream(socket);

    socketStream.startTransaction();
    socketStream >> buffer;

    if(!socketStream.commitTransaction())
    {
        QString message = QString("%1 :: Waiting").arg(socket->socketDescriptor());
        emit newMessage(message);
        return;
    }

    QString header = buffer.mid(0,128);
    QString fileType = header.split(",")[0].split(":")[1];

    buffer = buffer.mid(128);

    if(fileType=="login"){
        QString user= QString::fromStdString(buffer.toStdString());
        loggedInClients.insert(user, socket);
        loggedInSockets.insert(socket,user);
        ui->comboBox_receiver->addItem(user);
        displayMessage(QString("%1 is logged in as %2").arg(socket->socketDescriptor()).arg(user));
    }
    else if(fileType=="attachment"){
        QString fileName = header.split(",")[1].split(":")[1];
        QString size = header.split(",")[2].split(":")[1];
        QString receiver = header.split(",")[3].split(":")[1].split(";")[0];
        if (loggedInClients.contains(receiver)){
            QTcpSocket* con=loggedInClients[receiver];
            QDataStream socket_stream(con);
            QByteArray header;
            header.prepend(QString("fileType:attachment,fileName:%1,fileSize:%2,sender:%3;").arg(fileName).arg(size).arg(loggedInSockets[socket]).toUtf8());
            header.resize(128);
            buffer.prepend(header);
            socket_stream << buffer;
        }
        else{
            QDataStream socket_stream(socket);
            QByteArray buf=(receiver+" is not available at the moment").toUtf8();
            QByteArray head;
            head.prepend(QString("fileType:message,fileName:null,fileSize:%1;").arg(buf.size()).toUtf8());
            head.resize(128);
            buf.prepend(head);
            socket_stream << buf;
        }
    }
    else if(fileType=="message"){
        QString receiver = header.split(",")[3].split(":")[1].split(";")[0];
        QString message = QString("%1 :: %2").arg(receiver).arg(QString::fromStdString(buffer.toStdString()));
        emit newMessage(message);

        if (loggedInClients.contains(receiver)){
            QTcpSocket* con=loggedInClients[receiver];
            QDataStream socket_stream(con);
            buffer.prepend((loggedInSockets[con]+": ").toUtf8());
            QByteArray head;
            head.prepend(QString("fileType:message,fileName:null,fileSize:%1;").arg(buffer.size()).toUtf8());
            head.resize(128);
            buffer.prepend(head);
            socket_stream << buffer;
        }
        else{
            QDataStream socket_stream(socket);
            QByteArray buf=(receiver+" is not available at the moment").toUtf8();
            QByteArray head;
            head.prepend(QString("fileType:message,fileName:null,fileSize:%1;").arg(buf.size()).toUtf8());
            head.resize(128);
            buf.prepend(head);
            socket_stream << buf;
        }
    }
}

void MainWindow::discardSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
    QSet<QTcpSocket*>::iterator it = connection_set.find(socket);
    if (it != connection_set.end()){
        displayMessage(QString("INFO :: A client has just left the room").arg(socket->socketDescriptor()));
        connection_set.remove(*it);
        loggedInClients.remove(loggedInSockets[*it]);
        loggedInSockets.remove(*it);
    }
    refreshComboBox();
    
    socket->deleteLater();
}

void MainWindow::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
        break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information(this, "Socket Server", "The host was not found. Please check the host name and port settings.");
        break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(this, "Socket Server", "The connection was refused by the peer. Make sure Socket Server is running, and check that the host name and port settings are correct.");
        break;
        default:
            QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
            QMessageBox::information(this, "Socket Server", QString("The following error occurred: %1.").arg(socket->errorString()));
        break;
    }
}

void MainWindow::on_pushButton_sendMessage_clicked()
{
    QString receiver = ui->comboBox_receiver->currentText();

    if(receiver=="Broadcast")
    {
        foreach (QTcpSocket* socket,connection_set)
        {
            sendMessage(socket);
        }
    }
    else
    {
        if(loggedInClients.contains(receiver))
            {
            QTcpSocket* socket=loggedInClients[receiver];
            sendMessage(socket);
            }
    }
    ui->lineEdit_message->clear();
}


void MainWindow::on_pushButton_sendAttachment_clicked()
{
    QString receiver = ui->comboBox_receiver->currentText();

    QString filePath = QFileDialog::getOpenFileName(this, ("Select an attachment"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), ("File (*.c *.h *.java *.html *.css *.js *.zip*.json *.txt *.png *.jpg *.jpeg)"));

    if(filePath.isEmpty()){
        QMessageBox::critical(this,"Socket Server","You haven't selected any attachment!");
        return;
    }

    if(receiver=="Broadcast")
    {
        foreach (QTcpSocket* socket,connection_set)
        {
            sendAttachment(socket, filePath);
        }
    }
    else
    {
        if(loggedInClients.contains(receiver))
        {
            QTcpSocket* socket=loggedInClients[receiver];
            sendAttachment(socket, filePath);
        }
    }
    ui->lineEdit_message->clear();
}
void MainWindow::sendMessage(QTcpSocket* socket)
{
    if(socket)
    {
        if(socket->isOpen())
        {
            QString str = ui->lineEdit_message->text();

            QDataStream socketStream(socket);

            QByteArray header;
            header.prepend(QString("fileType:message,fileName:null,fileSize:%1;").arg(str.size()).toUtf8());
            header.resize(128);

            QByteArray byteArray = str.toUtf8();
            byteArray.prepend(header);
            socketStream << byteArray;
        }
        else
            QMessageBox::critical(this,"Socket Server","Socket doesn't seem to be opened");
    }
    else
        QMessageBox::critical(this,"Socket Server","Not connected");
}

void MainWindow::sendAttachment(QTcpSocket* socket, QString filePath)
{
    if(socket)
    {
        if(socket->isOpen())
        {
            QFile m_file(filePath);
            if(m_file.open(QIODevice::ReadOnly)){

                QFileInfo fileInfo(m_file.fileName());
                QString fileName(fileInfo.fileName());

                QDataStream socketStream(socket);
                QByteArray header;
                header.prepend(QString("fileType:attachment,fileName:%1,fileSize:%2;").arg(fileName).arg(m_file.size()).toUtf8());
                header.resize(128);

                QByteArray byteArray = m_file.readAll();
                byteArray.prepend(header);

                socketStream << byteArray;
            }else
                QMessageBox::critical(this,"Socket Server","Couldn't open the attachment!");
        }
        else
            QMessageBox::critical(this,"Socket Server","Socket doesn't seem to be opened");
    }
    else
        QMessageBox::critical(this,"Socket Server","Not connected");
}

void MainWindow::displayMessage(const QString& str)
{
    ui->textBrowser_receivedMessages->append(str);
}

void MainWindow::refreshComboBox(){
    ui->comboBox_receiver->clear();
    ui->comboBox_receiver->addItem("Broadcast");
    foreach(QTcpSocket* socket, connection_set)
        ui->comboBox_receiver->addItem(loggedInSockets[socket]);
}
