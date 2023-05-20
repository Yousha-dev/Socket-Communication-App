#ifndef NEWPASSWORD_H
#define NEWPASSWORD_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class newpassword;
}

class newpassword : public QDialog
{
    Q_OBJECT

public:
    QSqlDatabase database;
    explicit newpassword(QWidget *parent = nullptr,QString em="");
    ~newpassword();

private slots:
    void on_resetPasswordButton_clicked();

private:
    Ui::newpassword *ui;
};

#endif // NEWPASSWORD_H
