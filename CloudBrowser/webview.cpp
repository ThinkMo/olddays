#include <QMessageBox>
#include <QWebFrame>
#include <QDebug>
#include <QProcess>
#include <QUrl>
#include "webview.h"

webView::webView(QUrl url)
{
    obj_openVlc = new Obj_OpenVlc(this);
    this->settings()->setAttribute(QWebSettings::JavascriptEnabled, true);//JavaScript
    this->settings()->setAttribute(QWebSettings::PluginsEnabled, true);//插件使能
    this->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);//
    this->settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);//JavaScript打开网页使能
    //this->settings()->setAttribute(QWebSettings::JavascriptCanCloseWindows, true);//JavaScript打开网页使能
    //this->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal,Qt::ScrollBarAlwaysOff);//设置滚动条
    this->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(this,SIGNAL(linkClicked(QUrl)),this,SLOT(doOpenUrl(QUrl)));
    connect(this->page()->mainFrame(),SIGNAL(javaScriptWindowObjectCleared()),this, SLOT(addJavaScriptObject()));
    if(!url.isEmpty())
    {
        this->load(url);
    }
}
void webView::doOpenUrl(QUrl url)
{
    QString sUrl(url.toString());
    if(sUrl.contains(QString("crp:"),Qt::CaseInsensitive))
    {
        QProcess *process = new QProcess;
        QString cmd("cthandler.exe ");
        cmd += sUrl.section(":",1,1);
        process->start(cmd);
        if(process->state()==QProcess::NotRunning)
            {
                QMessageBox msgBox;
                msgBox.setText(QString("the arphandled not run"));
                msgBox.show();
            }
    }
    else if(sUrl.contains(QString("mpw:"),Qt::CaseInsensitive))
    {
        QProcess *process = new QProcess;
        QString cmd("mpw.exe ");
        cmd += sUrl.section(":",1,1);
        process->start(cmd);
        if(process->state()==QProcess::NotRunning)
            {
                QMessageBox msgBox;
                msgBox.setText(QString("the mpw not run"));
                msgBox.show();
            }
    }else if(sUrl.contains(QString("appreg:"),Qt::CaseInsensitive))
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
    }else
    {
        this->load(url);
    }
}

void webView::addJavaScriptObject(){
    qDebug()<<"addJavaScriptObject";
    //javascript可以通过对象名obj_open_vlc访问obj_openVlc对象。
    page()->mainFrame()->addToJavaScriptWindowObject(QString("obj_open_vlc"),this->obj_openVlc);
}
