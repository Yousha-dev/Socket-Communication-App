#include "resetpassword.h"
#include "ui_resetpassword.h"
#include "entercode.h"
#include "SmtpMIME/SmtpMime.h"
#include "login.h"

#include <stdlib.h>
#include <QtGui>
#include <QtCore>
#include <string>
#include <QMessageBox>

resetpassword::resetpassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::resetpassword)
{
    ui->setupUi(this);
}

resetpassword::~resetpassword()
{
    delete ui;
}

void resetpassword::on_sendButton_clicked()
{
    QString email = ui->lineEdit_resetEmail->text();
    QString str1= "@";
    QString str2= ".";

    if (email.compare(str1) > 0 && email.compare(str2) > 0)
    {
        MimeMessage message;
        EmailAddress sender("cp_project-no-reply@mart.dreamhosters.com", "OS Project");
        message.setSender(sender);

        EmailAddress to(email, "User");
        message.addRecipient(to);
        message.setSubject("Reset Password");

        MimeText text;
        int v = QRandomGenerator::global()->bounded(100000, 999999);
        QString a=QString::number(v);
        text.setText("Please write this code on the app.\n"+a);
        message.addPart(&text);

        // Now we can send the mail
        SmtpClient smtp("smtp.dreamhost.com", 465, SmtpClient::SslConnection);

        smtp.connectToHost();
        if (!smtp.waitForReadyConnected())
        {
               qDebug() << "Failed to connect to host!";
               QMessageBox::information(this, "Error", "Please, Check your internet connection.");
               return;
        }

        smtp.login("cp_project-no-reply@mart.dreamhosters.com", "cpproject534");
        if (!smtp.waitForAuthenticated())
        {
               qDebug() << "Failed to login!";
               QMessageBox::information(this, "Error", "Please, Check your internet connection.");
               return;
        }

        smtp.sendMail(message);
        if (!smtp.waitForMailSent())
        {
               qDebug() << "Failed to send mail!";
               QMessageBox::information(this, "Error", "Invalid E-mail address.");
               return;
        }
        smtp.quit();
        hide();
        entercode *newwin = new entercode(0,a,email);
        newwin->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
        newwin->show();

        }

   }

void resetpassword::on_pushButton_clicked()
{
    hide();
    login *a = new login();
    a->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    a->show();
}

