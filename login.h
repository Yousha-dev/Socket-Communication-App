#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>

QT_BEGIN_NAMESPACE
namespace Ui { class login; }
QT_END_NAMESPACE

class login : public QMainWindow
{
    Q_OBJECT

public:
    QSqlDatabase database;
    login(QWidget *parent = nullptr);
    ~login();

private slots:
    void on_showPasswordButton_toggled(bool checked);

    void on_forgotPasswordButton_clicked();

    void on_loginButton_clicked();

    void on_createAccountButton_clicked();

private:
    Ui::login *ui;
};
#endif // LOGIN_H
