#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QTableWidget>

class MainWindow : public QObject
{
    Q_OBJECT

public slots:
    void replyFinished(QNetworkReply *reply);

public:
    MainWindow();
    ~MainWindow();
    //QByteArray getHtml(QUrl *url, )
private:
    QMap< QString, QList<qint32> > skills;
    void setSkills();
    QTableWidget table;
    void setColumn(QString name, QString pos1, double skill1, QString pos2, double skill2, QString pos3, double skill3);
    int row;
};

#endif // MAINWINDOW_H
