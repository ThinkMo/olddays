#ifndef OBJ_OPENVLC_H
#define OBJ_OPENVLC_H
#include <QObject>
class Obj_OpenVlc : public QObject
{
    Q_OBJECT
public:
    explicit Obj_OpenVlc(QObject *parent = 0);
signals:
public slots:
    //��javascript���õĲ�
    void openVLC(QString url);
};
#endif // OBJ_OPENVLC_H
