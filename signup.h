#ifndef SIGNUP_H
#define SIGNUP_H

#include <QDialog>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFileInfo>
#include <QMessageBox>


namespace Ui {
class signup;
}

class signup : public QDialog
{
    Q_OBJECT

public:
    explicit signup(QWidget *parent = nullptr);
    ~signup();

private slots:
    void on_signupButton_clicked();

    void on_showPasswordButton_toggled(bool checked);

    void on_backToLoginButton_clicked();

private:
    Ui::signup *ui;
};

#endif // SIGNUP_H
