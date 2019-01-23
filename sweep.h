#ifndef SWEEP_H
#define SWEEP_H
#include <QTime>
#include <QThread>
#include <QTimer>
#include <QObject>
#include <QDebug>
#include <QMessageBox>
class Sweep : public QObject
{
    Q_OBJECT
public:
    explicit Sweep(QObject *parent = nullptr);
    void setData(int **&sweep,bool **&isOpen,bool **&isFlag,int &height,int &width,int &bombNumber);
    void run(void);
    void calc(void);
    bool isArea(int x,int y);
    void openValue(int x,int y);
private:
    int **sweep;
    bool **isOpen;
    bool **isFlag;
    int height,width;
    int bombNumber;
    int bombCount;
signals:
    void sweepSignal(void);
    void SuccessSignal(int x);
public slots:
    void getValue(int x,int y);
};

#endif // SWEEP_H
