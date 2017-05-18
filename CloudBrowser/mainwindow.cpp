#include <QApplication>
#include <QDesktopWidget>
#include <QMoveEvent>
#include <QToolButton>
#include <QStyle>
#include <QProcess>
#include <QDir>
#include <windows.h>
#include <tlhelp32.h>

#include "mainwindow.h"

MainWindow::MainWindow(char **arg,int flag, QWidget *parent)
    : QMainWindow(parent,Qt::X11BypassWindowManagerHint)
{
    this->flag = flag;
    this->flagSize = false;
    QString myUrl(arg[1]);
    if(!myUrl.contains("http://"))
        myUrl.insert(0,"http://");

    RECT rt;
    SystemParametersInfo(SPI_GETWORKAREA,0,&rt,0);

    int width=QApplication::desktop()->width();
    int height=rt.bottom;

    setGeometry(0,0,width,height);
    //setWindowFlags(Qt::Window | Qt::WindowMinMaxButtonsHint);
    //setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);  //只有closebutton

    //设置窗体标题栏隐藏并设置位于顶层
    //setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    //可获取鼠标跟踪效果
    setMouseTracking(true);

    /*
    if(flag != 5) //not win7
    {
        setWindowFlags(Qt::CustomizeWindowHint);
        setWindowFlags(Qt::WindowSystemMenuHint);
    }
    else  //win7
    {
        setWindowFlags(Qt::FramelessWindowHint);
    }
    */
    //mBox = new QMessageBox(this);
    m_lineEdit = new QLineEdit;
    m_lineEdit->setStyleSheet("QLineEdit {height:20px; border: 2px solid #97FFFF; font:14px Arial; padding:3px 6px 3px 6px;}");

    m_centralWidget = new QWidget(this);
    m_url = new QUrl(myUrl);
    m_webWidget = new webView(*m_url);

    m_titleWidget = new TitleWidget(this);

    m_layout = new QVBoxLayout;
    m_layout->setSpacing(0);
    m_layout->setMargin(0);

    m_layout->addWidget(m_titleWidget);
    m_layout->addSpacing(3);
    m_layout->addWidget(m_lineEdit);
    m_layout->addSpacing(3);
    m_layout->addWidget(m_webWidget);
    m_centralWidget->setLayout(m_layout);
    setCentralWidget(m_centralWidget);
    connect(m_webWidget,SIGNAL(titleChanged(QString)),
        this, SLOT(slotUpdateWindowTitle(QString )));

    connect(m_titleWidget,SIGNAL(closeWidget()),this,SLOT(close()));
    connect(m_titleWidget,SIGNAL(reflash()),this,SLOT(reflash()));
    connect(m_titleWidget,SIGNAL(showMainMenu()),this,SLOT(changeSize()));
    connect(m_titleWidget,SIGNAL(showMin()),this,SLOT(showMin()));

    connect(m_lineEdit,SIGNAL(returnPressed()),this,SLOT(openUrl()));
    connect(m_webWidget,SIGNAL(urlChanged(QUrl)),this,SLOT(changeLineEdit(QUrl)));
    //connect(m_titleWidget,SIGNAL(showMin()),this,SLOT(showMin()));

}

MainWindow::~MainWindow()
{
    delete this->m_layout;
    delete this->m_titleWidget;
    delete this->m_webWidget;
    delete this->m_url;
    delete this->m_centralWidget;
}


void MainWindow::slotUpdateWindowTitle(const QString &title)
{
    m_title = title;

    if (title.isEmpty())
        m_titleWidget->setTitle(tr("Cloud Browser"));
    else
        m_titleWidget->setTitle(QString("%1 - Cloud Browser").arg(title));
}

void MainWindow::showMin()
{
    this->showMinimized();
}

void MainWindow::changeSize()
{
    if(flagSize)
    {
        this->showNormal();
        m_titleWidget->setTip(true);
        flagSize = false;
    }else
    {
        this->showFullScreen();
        m_titleWidget->setTip(false);
        flagSize = true;
    }
}

void MainWindow::reflash()
{
    m_webWidget->reload();
}

void MainWindow::changeLineEdit(QUrl url)
{
    m_lineEdit->setText(url.toString());
}

void MainWindow::openUrl()
{
    QString myUrl(m_lineEdit->text());

    if(myUrl.size() > 0)
    {
        if(!myUrl.contains("http://") && !myUrl.contains("https://"))
            myUrl.insert(0,"http://");
        m_webWidget->load(QUrl(myUrl));
    }
}
