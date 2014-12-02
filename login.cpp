#include "login.h"
#include "ui_login.h"
#include <QMediaPlayer>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMediaPlaylist>
#include <mainwindow.h>
Login::Login() :
    ui(new Ui::Login)
{
    Qt::WindowFlags flags = Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);
    QMediaPlaylist *playlist = new QMediaPlaylist();
    playlist->addMedia(QMediaContent(QUrl::fromLocalFile(QDir::toNativeSeparators(QCoreApplication::applicationDirPath()+"/song.mp3"))));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    QMediaPlayer *player = new QMediaPlayer();
    player->setPlaylist(playlist);
    player->setVolume(100);
    player->play();
    ui->setupUi(this);
    QFile file(QDir::toNativeSeparators(QCoreApplication::applicationDirPath()+"/loginData.txt"));
    if (file.open(QIODevice::ReadOnly))
    {
        QRegExp logExp("login:(.+)");
        QRegExp passExp("password:(.+)");
        QTextStream in(&file);
        QString firstLine = in.readLine();
        QString secondLine = in.readLine();
        logExp.indexIn(firstLine,0);
        passExp.indexIn(secondLine,0);
        ui->lineLogin->setText(logExp.cap(1));
        ui->linePassword->setText(passExp.cap(1));
        file.close();
    }
}

Login::~Login()
{
    delete ui;
}

void Login::on_cancel_clicked()
{
    this->close();
}

void Login::on_submit_clicked()
{
    if (ui->saveLoginData->isChecked()) {
        QFile file(QDir::toNativeSeparators(QCoreApplication::applicationDirPath()+"/loginData.txt"));
        file.open(QIODevice::WriteOnly);
        QTextStream out(&file);
        out << "login:" << ui->lineLogin->text() << "\r" << endl;
        out << "password:" << ui->linePassword->text();
        file.close();
    }
    //this->close();
    MainWindow *mw = new MainWindow;
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    QNetworkRequest *request = new QNetworkRequest;
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), mw, SLOT(replyFinished(QNetworkReply*)));
    QString pass = QString(QCryptographicHash::hash((ui->linePassword->text().toLocal8Bit()),QCryptographicHash::Md5).toHex());
   // QString pass = QString(QCryptographicHash::hash(("metallist1925"),QCryptographicHash::Md5).toHex());
    QList<QNetworkCookie> cookie_list;
    cookie_list.append(QNetworkCookie("login", ui->lineLogin->text().toLocal8Bit()));
  //  cookie_list.append(QNetworkCookie("login", "Fanboy"));
    cookie_list.append(QNetworkCookie("password", pass.toLocal8Bit()));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request->setUrl(QUrl("http://www.soccerlife.ru/team4.php"));
    request->setHeader(QNetworkRequest::CookieHeader, QVariant::fromValue(cookie_list));
    manager->post(*request,"");
}
