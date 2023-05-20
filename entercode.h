#ifndef ENTERCODE_H
#define ENTERCODE_H

#include <QDialog>

namespace Ui {
class entercode;
}

class entercode : public QDialog
{
    Q_OBJECT

public:
    explicit entercode(QWidget *parent = nullptr, QString b="0", QString email="");
    ~entercode();

private slots:
    void on_enterCodeButton_clicked();

    void on_resendCodeButton_clicked();

private:
    Ui::entercode *ui;
};

#endif // ENTERCODE_H
