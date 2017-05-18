#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QWebView>
#include <QWebSettings>
#include "obj_openvlc.h"

class webView : public QWebView
{
    Q_OBJECT
public slots:
    void doOpenUrl(QUrl url);
public:
    webView(QUrl url);
private slots:
    void addJavaScriptObject();
private:
    Obj_OpenVlc *obj_openVlc; //将暴露给javascript的对象
};

#endif // WEBVIEW_H
