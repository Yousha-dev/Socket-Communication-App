#include "entercode.h"
#include "ui_entercode.h"
#include "newpassword.h"
#include "resetpassword.h"
#include <QMessageBox>
QString code,em;
entercode::entercode(QWidget *parent, QString b,QString email) :
    QDialog(parent),
    ui(new Ui::entercode)
{
    code=b;
    em=email;
    ui->setupUi(this);
}

entercode::~entercode()
{
    delete ui;
}

void entercode::on_enterCodeButton_clicked()
{
    QString code_line = ui->lineEdit_code->text();
    if(code_line==""){
        QMessageBox::information(this, "Error", "Enter Code");
        return;
    }

    if(code_line!=code){
        QMessageBox::information(this, "Error", "Invalid Code");
        return;
    }
    hide();
    newpassword *newwin = new newpassword(0,em);
    newwin->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    newwin->show();
}

void entercode::on_resendCodeButton_clicked()
{
    hide();
    resetpassword *newwin = new resetpassword();
    newwin->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    newwin->show();
}

