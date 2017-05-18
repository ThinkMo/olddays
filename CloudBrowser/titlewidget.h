#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QMenu>
#include <QString>


class TitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TitleWidget(QWidget *parent = 0);
    ~TitleWidget();
    void setmenu(QMenu *menu);
    void setTip(bool tip);
    void setTitle(const QString title);
signals:
    void showMainMenu();
    void showMin();
    void reflash();
    void closeWidget();
public slots:
    //鼠标按下事件
    void mousePressEvent(QMouseEvent *event);
    //鼠标释放事件
    void mouseReleaseEvent(QMouseEvent *event);
    //鼠标移动事件
    void mouseMoveEvent(QMouseEvent *event);
private:
    QPoint press_point; //移动的距离
    bool is_move; //鼠标按下
    QLabel *version_title;
    QToolButton *main_menu_button;
    QToolButton *min_button ;
    QToolButton *reflash_button;
    QToolButton *close_button;
    QHBoxLayout *hbox;

};

#endif // TITLEWIDGET_H
