#include "blobobject.h"

blobobject::blobobject(int a,int b,int c, int d)
{
    this->topleftX =a;
    this->topLeftY =b;
    this->hieght = c;
    this->width =d;
}

void blobobject::setBlobParams(int a,int b,int c, int d){

    this->topleftX =a;
    this->topLeftY =b;
    this->hieght = c;
    this->width =d;

}

int blobobject::getBlobTopLeftX(){

   return this->topleftX;
}
int blobobject::getBlobTopLeftY(){

    return this->topLeftY;
}
int blobobject::getBlobHeight(){

    return this->hieght;
}
int blobobject::getBlobWidth(){

    return this->width;
}
