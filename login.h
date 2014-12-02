#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login();
    ~Login();

private slots:
    void on_cancel_clicked();

    void on_submit_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
