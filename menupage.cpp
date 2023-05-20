#include "menupage.h"
#include "ui_menupage.h"
#include "login.h"
#include "message.h"
#include "signup.h"
#include <QtGui>
#include <QtCore>
#include <QMessageBox>
#include <string>
QString email_para;
menupage::menupage(QWidget *parent,QString em) :
    QDialog(parent),
    ui(new Ui::menupage)
{
    email_para=em;
    ui->setupUi(this);
}

menupage::~menupage()
{
    delete ui;
}

void menupage::on_backtToLoginButton_clicked()
{
    hide();
    login *res = new login();
    res->show();
}


void menupage::on_contactListButton_clicked()
{
    hide();
    QString name=email_para.left(email_para.lastIndexOf('@'));
    Message *new_win = new Message(0,name);
    new_win->setModal(true);
    new_win->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    new_win->exec();
}


void menupage::on_quitButton_clicked()
{
    close();
}


void menupage::on_pushButton_clicked()
{
    hide();
    signup signup_win;
    signup_win.setModal(true);
    signup_win.exec();
}


void menupage::on_infoButton_clicked()
{
    QMessageBox::information(this, "Project Info", """OS: Operating Systems\n"
                                                   "\nYousha Masood\nMuhammad Umer\nZeeshan""");
}

