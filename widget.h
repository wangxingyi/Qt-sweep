#ifndef WIDGET_H
#define WIDGET_H
#include <QPaintDevice>
#include <QPainter>
#include <QPen>
#include <QThread>
#include <QTimer>
#include <QWidget>
#include <QVector>
#include <QTime>
#include <QMouseEvent>
#include "sweep.h"
#include <QMessageBox>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
signals:
    void flag(void);        //当又标记雷时，发出该信号，让子线程判断是否全部标记完
    void value(int x,int y);//当打开方块时空白时，打开附近的空白方块
public slots:
    void dealSucc(int x);   //处理是否全部标记
public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *m);
private:
    Ui::Widget *ui;
    int height,width;   //长宽的blocksize数
    int blockSize;      //每个方块的宽
    int imageSizeMax;   //图片的最大数量
    int bombNumber;     //雷的数量

    QString imageBuf[13];//图片存储

    int** mine;         //每个方块的值
    bool ** isOpen;     //表示方标是否被打开过
    bool ** isFlag;     //表示方块是否被标记过
    Sweep *sweep;

    QThread *thread;
};

#endif // WIDGET_H
