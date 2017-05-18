#include "obj_openvlc.h"
#include <QtDebug>
#include <QMessageBox>
#include <QString>
#include <QProcess>
Obj_OpenVlc::Obj_OpenVlc(QObject *parent) :
    QObject(parent)
{
}

void Obj_OpenVlc::openVLC(QString url)
{
    qDebug() << "run";
    if(url.contains(QString("crp:"),Qt::CaseInsensitive))
    {
        QProcess *process = new QProcess;
        QString cmd("cthandler.exe ");
        cmd += url.section(":",1,1);
        process->start(cmd);
        if(process->state()==QProcess::NotRunning)
        {
            QMessageBox msgBox;
            msgBox.setText(QString("the arphandled not run"));
            msgBox.show();
        }
    }else if(url.contains(QString("mpw:"),Qt::CaseInsensitive))
    {
        QProcess *process = new QProcess;
        QString cmd("mpw.exe ");
        cmd += url.section(":",1,1);
        process->start(cmd);
        if(process->state()==QProcess::NotRunning)
        {
            QMessageBox msgBox;
            msgBox.setText(QString("the mpw not run"));
            msgBox.show();
        }
    }else if(url.contains(QString("appreg:"),Qt::CaseInsensitive))
    {
        QProcess *process = new QProcess;
        QString cmd("Srv.exe");
        //cmd += url.section(":",1,1);
        process->start(cmd);
        if(process->state()==QProcess::NotRunning)
        {
            QMessageBox msgBox;
            msgBox.setText(QString("the Srv not run"));
            msgBox.show();
        }
    }
}
