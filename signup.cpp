#include "signup.h"
#include "ui_signup.h"
#include "login.h"
#include <QtGui>
#include <QtCore>

signup::signup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::signup)
{
    ui->setupUi(this);
}

signup::~signup()
{
    delete ui;
}

bool checkEmail_signup(QString e)
{
    int e1 = e.compare("@"), e2 = e.compare(".");

    if (e1 > 0 && e2 > 0)
        return true;

    return false;
}


void signup::on_signupButton_clicked()
{
    QString email = ui->lineEdit_email->text();
    QString password = ui->lineEdit_password->text();
    QString confpassword = ui->lineEdit_confPassword->text();

    if (checkEmail_signup(email) != true)
    {
        QMessageBox::information(this, "Incorrect Email Address", "Email address format is incorrect");
        return;
    }

    else if (password.size() < 7)
    {
        QMessageBox::information(this, "Incorrect Password Length", "Character length should exceed 7 characters");
        return;
    }
    else if (password.size() > 16)
    {
        QMessageBox::information(this, "Incorrect Password Length", "Character length should be less than 15 characters");
        return;
    }
    else if (password != confpassword)
    {
        QMessageBox::information(this, "Password", "Confirm Password is not the same as Password text");
        return;
    }
    else
    {
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("/home/ubuntu/OS_Project/register.sqlite");
        database.open();
        QFileInfo checkFile("/home/ubuntu/OS_Project/register.sqlite");
        if(checkFile.isFile())
        {
            if(!database.open())
             {
                QMessageBox::information(this, "Registration", "Registration failed");
                database.close();
                return;
             }
             else
             {
                QSqlQuery qry;
                QSqlQuery qry2;
                qry.prepare("INSERT INTO users (email, password)"
                            "VALUES (:email, :password)");
                qry.bindValue(":email",email);
                qry.bindValue(":password",password);
                QString name=email.left(email.lastIndexOf('@'));
                QString query="CREATE TABLE IF NOT EXISTS "+name+" ("
                              "Name VARCHAR(50));";
                if(qry.exec()&&qry2.exec(query))
                    {
                        QMessageBox::information(this, "Registration", "Registration Successful");
                        database.close();
                        hide();
                        login *newwin = new login();
                        newwin->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
                        newwin->show();
                    }
                else
                    {
                        QMessageBox::information(this, "Error", "Wrong username or password");
                        database.close();
                        return;
                    }
            }

        }
        else
        {
                QMessageBox::information(this, "Error", "Database file not present");
                database.close();
                return;
        }
    }

}


void signup::on_showPasswordButton_toggled(bool checked)
{
    if (checked == true) {
        ui->lineEdit_password->setEchoMode(QLineEdit::Normal);
        ui->lineEdit_confPassword->setEchoMode(QLineEdit::Normal);
    }
    else {
        ui->lineEdit_password->setEchoMode(QLineEdit::Password);
        ui->lineEdit_confPassword->setEchoMode(QLineEdit::Password);
    }
}

void signup::on_backToLoginButton_clicked()
{
    hide();
    login *res = new login();
    res->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    res->show();
}
