#ifndef MENUPAGE_H
#define MENUPAGE_H

#include <QDialog>

namespace Ui {
class menupage;
}

class menupage : public QDialog
{
    Q_OBJECT

public:
    explicit menupage(QWidget *parent = nullptr, QString em="");
    ~menupage();

private slots:
    void on_backtToLoginButton_clicked();

    void on_contactListButton_clicked();

    void on_quitButton_clicked();

    void on_pushButton_clicked();

    void on_infoButton_clicked();

private:
    Ui::menupage *ui;
};

#endif // MENUPAGE_H
