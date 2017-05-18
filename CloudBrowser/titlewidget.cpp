#include "titlewidget.h"
#include <QIcon>
#include <QHBoxLayout>

TitleWidget::TitleWidget(QWidget *parent):
    QWidget(parent)
{
    setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(65,105,225));
    //palette.setBrush(QPalette::Background, QBrush(QPixmap(":/background.png")));
    setPalette(palette);

    version_title = new QLabel();
    main_menu_button = new QToolButton(this);
    min_button = new QToolButton(this);
    reflash_button = new QToolButton(this);
    close_button = new QToolButton(this);
    QIcon minPix(":/myImages/images/min.png");
    QIcon reflashPix(":/myImages/images/reflash.png");
    QIcon menuPix(":/myImages/images/max.png");
    QIcon closePix(":/myImages/images/close.png");

    version_title->setStyleSheet("QLabel{color:white;font-size:18px;font-family:幼圆;font-weight:bold;}");
    version_title->setText(tr("云浏览"));
    //设置图片
    main_menu_button->setIcon(menuPix);
    min_button->setIcon(minPix);
    reflash_button->setIcon(reflashPix);
    close_button->setIcon(closePix);

    setStyleSheet("QToolButton{border:0;height:120px;width:20px;}");
    main_menu_button->setToolTip(tr("全  屏"));
    main_menu_button->setStyleSheet("QToolButton:hover{background-color:#7693EA;}");
    reflash_button->setToolTip(tr("刷  新"));
    reflash_button->setStyleSheet("QToolButton:hover{background-color:#7693EA;}");
    min_button->setToolTip(tr("最小化"));
    min_button->setStyleSheet("QToolButton:hover{background-color:#7693EA;}");
    close_button->setToolTip(tr("退 出"));
    close_button->setStyleSheet("QToolButton:hover{background-color:#FF0000;}");
/*
    main_menu_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
    min_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
    reflash_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
    close_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
*/
    hbox = new QHBoxLayout(this);

    hbox->addWidget(version_title,0,Qt::AlignLeft);
    hbox->addStretch();
    hbox->addWidget(reflash_button,0,Qt::AlignBottom);
    hbox->addWidget(min_button,0,Qt::AlignBottom);
    hbox->addWidget(main_menu_button,0,Qt::AlignBottom);
    hbox->addWidget(close_button,0,Qt::AlignBottom);
    version_title->setContentsMargins(15,0,0,0);

    hbox->insertStretch(1, 500);
    hbox->setSpacing(0);
    hbox->setMargin(0);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setLayout(hbox);
    setFixedHeight(30);
    is_move = false;



    connect(main_menu_button, SIGNAL(clicked()), this, SIGNAL(showMainMenu()));
    connect(min_button, SIGNAL(clicked()), this, SIGNAL(showMin()));
    connect(reflash_button,SIGNAL(clicked()), this, SIGNAL(reflash()));
    connect(close_button, SIGNAL(clicked()), this, SIGNAL(closeWidget()));
}
        /*
void TitleWidget::translateLanguage()
{
 version_title->setText(tr("title"));
 skin_button->setToolTip(tr("change skin"));
 main_menu_button->setToolTip(tr("main menu"));
 min_button->setToolTip(tr("minimize"));
 max_button->setToolTip(tr("maximize"));
 close_button->setToolTip(tr("close"));
 button_list.at(0)->setText(tr("power"));
 button_list.at(1)->setText(tr("mummy"));
 button_list.at(2)->setText(tr("hole"));
 button_list.at(3)->setText(tr("repair"));
 button_list.at(4)->setText(tr("clear"));
 button_list.at(5)->setText(tr("optimize"));
 button_list.at(6)->setText(tr("expert"));
 button_list.at(7)->setText(tr("software"));
}
    */

TitleWidget::~TitleWidget()
{
    delete version_title;
    delete main_menu_button;
    delete min_button;
    delete reflash_button;
    delete close_button;
    delete hbox;
}

void TitleWidget::mousePressEvent(QMouseEvent *e)
{
    press_point = e->pos();
    is_move = true;
}

void TitleWidget::mouseMoveEvent(QMouseEvent *e)
{
    /*
    if((e->buttons() == Qt::LeftButton) && is_move)
    {
        static QWidget* parent_widget = this->parentWidget();
        QPoint parent_point = parent_widget->pos();
        parent_point.setX(parent_point.x() + e->x() - press_point.x());
        parent_point.setY(parent_point.y() + e->y() - press_point.y());
        parent_widget->move(parent_point);
    }
    */
}
void TitleWidget::mouseReleaseEvent(QMouseEvent *)
{
    if(is_move)
    {
        is_move = false;
    }
}

void TitleWidget::setmenu(QMenu *menu)
{
    //main_menu_button->setMenu(menu);
    QRect rect = main_menu_button->geometry();
    rect.setY(rect.y()+20);
    menu->setGeometry(rect);
}

void TitleWidget::setTip(bool tip)
{
    if(tip)
    {
        QIcon menuPix("images/max.png");
        //设置图片
        main_menu_button->setIcon(menuPix);
        this->main_menu_button->setToolTip(tr("全  屏"));
    }else{
        QIcon menuPix("images/change.png");
        //设置图片
        main_menu_button->setIcon(menuPix);
        this->main_menu_button->setToolTip(tr("还  原"));
    }
}

void TitleWidget::setTitle(const QString title)
{
    version_title->setText(title);
}
