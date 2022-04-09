#include "bird.h"

Bird::Bird(QWidget *parent)
    : QWidget(parent)
{
    setMaximumSize(35,35);
    setMinimumSize(35,35);
    co=0;
    timer=new QTimer(this);
    src[0]=":/Images/bird1.png";
    src[1]=":/Images/bird2.png";
    src[2]=":/Images/bird3.png";
    this->rale=-31;
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    connect(this,SIGNAL(fly()),this,SLOT(updateRale()));
    timer->start(200);

}

Bird::~Bird()
{

}

void Bird::play()
{
    timer->start(200);
}

void Bird::stop()
{
    timer->stop();
}

void Bird::paintEvent(QPaintEvent *)
{

    // the rotation of the bird is actually the entire window world rotating,
    // which is rotating the canvas and then updating the picture.
    QPainter painter(this);
    QPixmap pix;

    painter.setWindow(-20,-20,40,40);	
    // Set the canvas equal to the offset center coordinate
    // so that the absolute coordinate center axis is 20,0
    pix.load(src[co]);
    if(zt==1)
    {
        painter.rotate(ztr);
        ztr-=10;
        if(ztr<=30)
        {
            zt=0;
        }
    }
    else
    {
        if(rale>=-30&&rale<=90)
        {
            painter.rotate(rale);
            if(rale<10)
            {
                rale=0.01*x*x-30;
                x+=1;
            }
            else
            {
                rale+=2;
            }
        }
        else if(rale>=90)
        {
            rale=90;
            painter.rotate(rale);
        }
        else if(rale<-30)
        {
            painter.rotate(0);
        }
    }
    co++;
    if(co>=3)
        co=0;
    painter.rotate(0);
    painter.drawPixmap(-20,-20,40,40,pix);
}

void Bird::updateRale()
{
    ztr=rale;
    this->rale=-30;
    x=0;
    zt=1;

}

void Bird::setRale(int rale)
{
    this->rale=rale;
}
