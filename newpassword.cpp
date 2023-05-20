#include "newpassword.h"
#include "ui_newpassword.h"
#include "login.h"
#include <QMessageBox>
QString email;
newpassword::newpassword(QWidget *parent, QString em) :
    QDialog(parent),
    ui(new Ui::newpassword)
{
    email=em;
    ui->setupUi(this);
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("/home/ubuntu/OS_Project/register.sqlite");
}

newpassword::~newpassword()
{
    delete ui;
}

void newpassword::on_resetPasswordButton_clicked()
{
    QString password = ui->lineEdit_password->text();
    database.open();
    QSqlQuery q;
    q.exec("SELECT * From users");
    while(q.next())
    {
        if(email==q.value(0).toString() )
        {
           q.prepare( "UPDATE users SET password = '"+password+"' WHERE email = '"+email+"'");
            if(q.exec())
            {
                QMessageBox::information(this, "Updated", "Successfully updated");
                hide();
                q.clear();
                database.close();
                return;
            }
            else
            {
                QMessageBox::information(this, "Error", "failed");
                return;
            }
        }
    }
    q.clear();
    database.close();
    hide();
    login *newwin = new login();
    newwin->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    newwin->show();
}
