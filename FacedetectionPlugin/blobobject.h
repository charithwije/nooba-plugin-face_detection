#ifndef BLOBOBJECT_H
#define BLOBOBJECT_H

class blobobject
{

private:
    int topleftX;
    int topLeftY;
    int hieght;
    int width;

public:
    blobobject(int a,int b,int c, int d);

    void setBlobParams(int,int,int,int);
    int getBlobTopLeftX();
    int getBlobTopLeftY();
    int getBlobHeight();
    int getBlobWidth();

};

#endif // BLOBOBJECT_H
