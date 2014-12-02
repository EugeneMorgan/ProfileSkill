#include "mainwindow.h"
#include <QDebug>
#include <QTextCodec>
#include <QWebElement>
#include <QList>
#include <QWebPage>
#include <QWebFrame>
#include <QTableWidget>

MainWindow::MainWindow() {
    setSkills();
    table.setColumnCount(7);
    table.setHorizontalHeaderLabels(QStringList() << "Фамилия, Имя" << "Амплуа #1" << "Амплуа №1(проф.скил)" << "Амплуа #2" << "Амплуа №2(проф.скил)" << "Доп.амплуа" << "Доп.амплуа(проф.скил)");
    row = 0;
    table.setMinimumHeight(800);
    table.setMinimumWidth(800);
}

MainWindow::~MainWindow() {
    delete this;
}

void MainWindow::replyFinished(QNetworkReply *reply) {
    this->table.show();
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    QString strf = codec->toUnicode(reply->readAll());
    QWebPage page;
    page.mainFrame()->setHtml(strf);
    QWebElement table = page.mainFrame()->findFirstElement("#generallist");
    QWebElementCollection players = table.findAll(".pl-0");
    this->table.setRowCount(players.count());
    foreach (QWebElement paraElement, players) {
       QString amp1 = paraElement.findAll("td").at(1).toPlainText().trimmed();
       QString amp2 = paraElement.findAll("td").at(2).toPlainText().trimmed();
       QString name = paraElement.findAll("td").at(3).toPlainText().trimmed();
       QWebElementCollection skillsPlayers = paraElement.findAll(".t1.pp4");
       double total = 0.0;
       qDebug() << name;
       if (amp1 == "CD" && amp2 == "DM") {
           double total1 = 0.0;
           QList<qint32> proSkill1 = skills[amp1];
           qDebug() << "amp1 -" << amp1;
           foreach (qint32 var, proSkill1) {
               double skill = skillsPlayers[var].toPlainText().toDouble();
               total += skill;
               qDebug() << skill << ' ';
           }
           qDebug() << "profile skill -" << total / proSkill1.length();
           QList<qint32> proSkill2 = skills["CD/DM"];
           qDebug() << "amp2 -" << amp2;
           foreach (qint32 var, proSkill2) {
               double skill = skillsPlayers[var].toPlainText().toDouble();
               total1 += skill;
               qDebug() << skill << ' ';
           }
           setColumn(name, amp1, total / proSkill1.length(), amp2, total1 / proSkill2.length(), "", 0.0);
           qDebug() << "profile skill -" << total / proSkill2.length();
       }
       else if (amp1 == "DM" && amp2 == "CM") {
           qDebug() << "amp1 -" << amp1;
           QList<qint32> proSkill1 = skills["CD/DM"];
           foreach (qint32 var, proSkill1) {
               double skill = skillsPlayers[var].toPlainText().toDouble();
               total += skill;
               qDebug() << skill << ' ';
           }
           qDebug() << "profile skill -" << total / proSkill1.length();
           qDebug() << "amp2 -" << amp2;
           total = 0.0;
           QList<qint32> proSkill2 = skills[amp2];
           foreach (qint32 var, proSkill2) {
               double skill = skillsPlayers[var].toPlainText().toDouble();
               total += skill;
               qDebug() << skill << ' ';
           }
           qDebug() << "profile skill -" << total / proSkill2.length();
           setColumn(name, amp1, total / proSkill1.length(), amp2, total / proSkill2.length(), "", 0.0);
       }
       else {
           QList<qint32> proSkill1 = skills[amp1];
           QString amp3 = "";
           double total3 = 0.0;
           double skill3 = 0.0;
           if (amp1 == "LD" || amp1 == "RD" || amp2 == "LD" || amp2 == "RD") {
               amp3 = "LWD/RWD";
               QList<qint32> proSkill3 = skills[amp3];
               qDebug() << "LWD/RDW";
               foreach (qint32 var, proSkill3) {
                   double skill = skillsPlayers[var].toPlainText().toDouble();
                   total3 += skill;
                   qDebug() << skill << ' ';
               }
               skill3 = total3 / proSkill3.length();
               qDebug() << "profile skill -" << total3 / proSkill3.length();
           }
           total = 0.0;
           double total1 = 0.0;
           qDebug() << "amp1 -" << amp1;
               foreach (qint32 var, proSkill1) {
                   double skill = skillsPlayers[var].toPlainText().toDouble();
                   total += skill;
                   qDebug() << skill << ' ';
               }
               qDebug() << "profile skill -" << total / proSkill1.length();
           if (!amp2.isEmpty()) {
               qDebug() << "amp2 - " << amp2;
               QList<qint32> proSkill2 = skills[amp2];
               foreach (qint32 var, proSkill2) {
                   double skill = skillsPlayers[var].toPlainText().toDouble();
                   total1 += skill;
                   qDebug() << skill << ' ';
               }
               qDebug() << "profile skill -" << total1 / proSkill2.length();
               setColumn(name, amp1, total / proSkill1.length(), amp2, total1 / proSkill2.length(), amp3, skill3);
           }
           else setColumn(name, amp1, total / proSkill1.length(), "", 0.0, amp3, skill3);
        }
       qDebug();
    }
    this->table.resizeColumnsToContents();
    this->table.setStyleSheet("QTableWidget {background: #E2FFD2} QHeaderView::section { background: #67A445 } QHeaderView { background: #67A445 } QTableCornerButton::section { background: #67A445 } QHeaderView {color: #FFFFFF}");
}

void MainWindow::setSkills() {
    QList<qint32> temp;
    QFile file(QDir::toNativeSeparators(QCoreApplication::applicationDirPath()+"/skills.txt"));
    if (file.open(QIODevice::ReadOnly))
    {
        QRegExp ampExp("\\[(.+)\\]");
        QRegExp skillsExp("(?:(\\d+):)+");
        ampExp.setMinimal(true);
        skillsExp.setMinimal(true);
        int pos = 0;
        QTextStream in(&file);
        while ( !in.atEnd() )
        {
            QString line = in.readLine();
            pos = 0;
            ampExp.indexIn(line,0);
            while ((pos = skillsExp.indexIn(line, pos)) != -1) {
                temp << skillsExp.cap(1).toInt();
                pos += skillsExp.matchedLength();
            }
            QString ampl = ampExp.cap(1);
            if (!ampl.isEmpty()) {
                skills[ampExp.cap(1)].append(temp);
            }
            temp.clear();
        }
        file.close();
    }
    qDebug() << skills.size();
//    temp << 5 << 6 << 9;
//    skills["CD"].append(temp);
//    temp.clear();
//    temp << 9;
//    skills["GK"].append(temp);
//    temp.clear();
//    temp << 3 << 4 << 5;
//    skills["RD"].append(temp);
//    skills["LD"].append(temp);
//    temp.clear();
//    temp << 0 << 3 << 9;
//    skills["RM"].append(temp);
//    skills["LM"].append(temp);
//    temp.clear();
//    temp << 0 << 4 << 9 << 7 << 6;
//    skills["CM"].append(temp);
//    temp.clear();
//    temp << 0 << 1 << 2;
//    skills["AM"].append(temp);
//    temp.clear();
//    temp << 1 << 2;
//    skills["ST"].append(temp);
//    temp.clear();
//    temp << 10 << 3 << 4 << 5;
//    skills["LWD/RWD"].append(temp);
//    temp.clear();
//    temp << 0 << 5 << 4 << 9;
//    skills["CD/DM"].append(temp);
//    skills["DM"].append(temp);
//    temp.clear();
//    temp << 0 << 4 << 9 << 5 << 6 << 7;
//    skills["DM/CM"].append(temp);
}

void MainWindow::setColumn(QString name, QString pos1, double skill1, QString pos2, double skill2, QString pos3, double skill3) {
    table.setItem(row,0,new QTableWidgetItem(name));
    table.setItem(row,1,new QTableWidgetItem(pos1));
    table.setItem(row,2,new QTableWidgetItem(QString::number(skill1)));
    table.setItem(row,3,new QTableWidgetItem(pos2));
    table.setItem(row,4,new QTableWidgetItem(QString::number(skill2)));
    table.setItem(row,5,new QTableWidgetItem(pos3));
    table.setItem(row,6,new QTableWidgetItem(QString::number(skill3)));
    row++;
}
