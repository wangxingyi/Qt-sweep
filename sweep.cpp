#include "sweep.h"

Sweep::Sweep(QObject *parent) : QObject(parent)
{
    bombCount = 0;
}

void Sweep::setData(int **&sweep, bool **&isOpen, bool **&isFlag, int &height, int &width, int &bombNumber)
{
    this->isFlag = isFlag;
    this->isOpen = isOpen;
    this->sweep = sweep;
    this->height = height;
    this->width = width;
    this->bombNumber = bombNumber;
    calc();
}

void Sweep::run()
{
    bombCount = 0;
    for(int i = 0;i < height;i++)
        for(int j=0;j < width; j++){
            if(isFlag[i][j] && sweep[i][j] == 11)
                bombCount ++;
        }
    emit SuccessSignal(bombCount);
}

void Sweep::calc()
{
    for(int i = 0;i < height;i++){
        for(int j = 0;j < width;j++){

            if(sweep[i][j] < 9)
                for(int n =  i-1; n <= i+1; n++){
                    for(int m= j-1; m <= j+1; m++){
                        if(isArea(n,m) && sweep[n][m] == 11){
                            sweep[i][j] ++;
                        }
                    }
                }
        }
    }
}

bool Sweep::isArea(int x, int y)
{
    return x>=0 && x<height&&y>=0 && y<width;
}

void Sweep::openValue(int x, int y)
{
    isOpen[x][y] = true;
    if(sweep[x][y] != 0)
        return ;
    for(int i = x - 1;i <= x+1;i++){
        for(int j = y-1;j <= y+1; j++){
            if(isArea(i,j) && sweep[i][j] != 11&& !isOpen[i][j])
                openValue(i,j);
        }
    }
}

void Sweep::getValue(int x, int y)
{
    openValue(x,y);
}
