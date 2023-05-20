#include "login.h"
#include "ui_login.h"
#include "signup.h"
#include "resetpassword.h"
#include "menupage.h"
#include <QtGui>
#include <QtCore>
#include <QMessageBox>
#include <string>

login::login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("/home/ubuntu/OS_Project/register.sqlite");
}

login::~login()
{
    delete ui;
}

bool checkEmail_login(QString e)
{
    int e1 = e.compare("@"), e2 = e.compare(".");

    if (e1 > 0 && e2 > 0)
        return true;

    return false;
}

void login::on_loginButton_clicked()
{
    QString email = ui->lineEdit_email->text();
    QString password = ui->lineEdit_password->text();

    if (checkEmail_login(email) != true)
        QMessageBox::information(this, "Incorrect Email Address", "Email address format is incorrect");

    else if (password.size() < 7)
        QMessageBox::information(this, "Incorrect Password Length", "Character length should exceed 7 characters");

    else if (password.size() > 15)
        QMessageBox::information(this, "Incorrect Password Length", "Character length should not exceed 15 characters");

    else {
        database.open();
        QSqlQuery q;
        q.exec("SELECT * From users");
        while(q.next())
        {
            if(email==q.value(0).toString() && password==q.value(1).toString())
            {
                hide();
                q.clear();
                database.close();
                menupage *menu_win = new menupage(0,email);
                menu_win->setModal(true);
                menu_win->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
                menu_win->exec();
                return;
            }
        }
        q.clear();
        database.close();
        QMessageBox::information(this, "Error", "Incorrect Email or password.");

    }
}

void login::on_showPasswordButton_toggled(bool checked)
{
    if (checked == true)
        ui->lineEdit_password->setEchoMode(QLineEdit::Normal);
    else
        ui->lineEdit_password->setEchoMode(QLineEdit::Password);
}

void login::on_forgotPasswordButton_clicked()
{
    resetpassword resetpass_win;
    resetpass_win.setModal(true);
    resetpass_win.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    resetpass_win.exec();
}

void login::on_createAccountButton_clicked()
{
    hide();
    signup signup_win;
    signup_win.setModal(true);
    signup_win.exec();
}
