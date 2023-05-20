#ifndef RESETPASSWORD_H
#define RESETPASSWORD_H

#include <QDialog>

namespace Ui {
class resetpassword;
}

class resetpassword : public QDialog
{
    Q_OBJECT

public:
    explicit resetpassword(QWidget *parent = nullptr);
    ~resetpassword();

private slots:
    void on_sendButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::resetpassword *ui;
};

#endif // RESETPASSWORD_H
