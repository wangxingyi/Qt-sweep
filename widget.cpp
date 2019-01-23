#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    imageBuf[0] = ":/new/prefix1/num/show.png";
    imageBuf[1] = ":/new/prefix1/num/01.png";
    imageBuf[2] = ":/new/prefix1/num/02.png";
    imageBuf[3] = ":/new/prefix1/num/03.png";
    imageBuf[4] = ":/new/prefix1/num/04.png";
    imageBuf[5] = ":/new/prefix1/num/05.png";
    imageBuf[6] = ":/new/prefix1/num/06.png";
    imageBuf[7] = ":/new/prefix1/num/07.png";
    imageBuf[8] = ":/new/prefix1/num/08.png";
    imageBuf[9] = ":/new/prefix1/num/09.png";
    imageBuf[10]= ":/new/prefix1/num/pressDown.png";
    imageBuf[11]= ":/new/prefix1/num/bomb.png";
    imageBuf[12]= ":/new/prefix1/num/bombflag.png";

    height = 10;
    width = 10;
    blockSize = 32;
    imageSizeMax = 13;
    bombNumber = 1;

    setGeometry(0,0,width*blockSize,height*blockSize);
    //申请内存并初赋值
    mine = new int*[height];
    isOpen = new bool*[height];
    isFlag = new bool*[height];
    for(int i = 0; i < height;i++){
        mine[i] = new int[width];
        isOpen[i] = new bool[width];
        isFlag[i] = new bool[width];
    }
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            mine[i][j] = 0;
            isOpen[i][j] = false;
            isFlag[i][j] = false;
        }
    }
    int count = 0;
    for(int i = 0;i < bombNumber; i++){
        int rand = (QTime::currentTime().msec() + qrand() + count)%(height*width);
        while(mine[count/width][count%width] != 0)count++;
        mine[count/width][count%width] = 11;
        int temp = mine[rand/height][rand%width];
        mine[rand/height][rand%width] = mine[count/width][count%width];
        mine[i/width][i%width] = temp;
        count++;
    }

    sweep = new Sweep();
    sweep->setData(mine,isOpen,isFlag,height,width,bombNumber);
    thread = new QThread(this);
    sweep->moveToThread(thread);//初始化线程
    connect(sweep,&Sweep::sweepSignal,[=](){
        repaint();
    });
    connect(sweep,&Sweep::SuccessSignal,this,&Widget::dealSucc);
    connect(this,&Widget::flag,sweep,&Sweep::run);
    connect(this,&Widget::value,sweep,&Sweep::getValue);
    thread->start();    //线程开始
    connect(this,&Widget::destroyed,[=](){
        thread->quit();
        thread->wait();
    });
}
void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter;
    if(mine == nullptr || isOpen == nullptr || isFlag == nullptr)
        throw "size is large";
    painter.begin(this);
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            QRect r(QPoint(i*blockSize,j*blockSize),
                    QPoint((i+1)*blockSize,(j+1)*blockSize));
            if(mine[i][j] >= imageSizeMax)
                throw "mine >= imageSize";
            if(isOpen[i][j]){
                if(mine[i][j] == 0)
                    painter.drawImage(r,QImage(imageBuf[10]));
                else
                    painter.drawImage(r,QImage(imageBuf[mine[i][j]]));
            }
            else if(isFlag[i][j])
                painter.drawImage(r,QImage(imageBuf[12]));
            else
                painter.drawImage(r,QImage(imageBuf[0]));
        }
    }
    painter.end();

}

void Widget::mousePressEvent(QMouseEvent *m)
{
    if(m->button() == Qt::LeftButton){
        int x = m->x()/blockSize;
        int y = m->y()/blockSize;
        if(!isOpen[x][y]){
            isOpen[x][y] = true;
            if(mine[x][y] == 11){
                QMessageBox::warning(this,"warning","Game Over");
                this->close();
            }
            else if(mine[x][y] == 0)
                emit value(x,y);//发出信号，在子线程中进行将空白打开
        }
    }
    else if(m->button() == Qt::RightButton){
        int x = m->x()/blockSize;
        int y = m->y()/blockSize;
        if(!isOpen[x][y])
            isFlag[x][y] = !isFlag[x][y];
        emit flag();
    }
    QThread::msleep(10);
    repaint();
}
void Widget::dealSucc(int x)
{
    if(x == bombNumber){
        QMessageBox::warning(this,"Suc","Nice");
        this->close();
    }
}
Widget::~Widget()
{
    delete sweep;
    delete ui;
}
