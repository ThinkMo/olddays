#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtCore/QUrl>
#include <QVBoxLayout>
#include <QWidget>
#include <QString>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QLineEdit>

#include "webview.h"
#include "titlewidget.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
 //   void moveEvent(QMoveEvent *event);
public:
    MainWindow(char **arg,int flag,QWidget *parent = 0);
    ~MainWindow();
public slots:
    void slotUpdateWindowTitle(const QString &title);
//    void showMainMenu();
    void showMin();
    void reflash();
    void changeSize();
    void changeLineEdit(QUrl url);
    void openUrl();
//    void closeWidget();
//    void changeUser();
//    void hangDisk();
private:
    int flag;
    bool flagSize;
//    QMessageBox *mBox;
    TitleWidget *m_titleWidget;
    webView *m_webWidget;
    QWidget *m_centralWidget;
    QVBoxLayout *m_layout;
    QUrl *m_url;
    QString m_title;
    QLineEdit *m_lineEdit;
    /*
    QMenu *menu;
    QAction *change_user;
    QAction *hang_disk;
    */
};

#endif // MAINWINDOW_H
