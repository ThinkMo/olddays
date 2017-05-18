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
    //��갴���¼�
    void mousePressEvent(QMouseEvent *event);
    //����ͷ��¼�
    void mouseReleaseEvent(QMouseEvent *event);
    //����ƶ��¼�
    void mouseMoveEvent(QMouseEvent *event);
private:
    QPoint press_point; //�ƶ��ľ���
    bool is_move; //��갴��
    QLabel *version_title;
    QToolButton *main_menu_button;
    QToolButton *min_button ;
    QToolButton *reflash_button;
    QToolButton *close_button;
    QHBoxLayout *hbox;

};

#endif // TITLEWIDGET_H
