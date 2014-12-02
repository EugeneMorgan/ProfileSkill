#include "mainwindow.h"
#include "login.h"
#include <QApplication>
#include <QtNetwork>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Login *log = new Login();
    log->show();
    return app.exec();
}
