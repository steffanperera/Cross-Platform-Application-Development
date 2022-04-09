#include "redyboard.h"

RedyBoard::RedyBoard(QWidget *parent)
    : QWidget(parent)
{
    this->setGeometry(0,0,380,500);
    label=new QLabel(tr("<font color=red>Control: left and right mouse button or space or up button</font>"),this);
    label->move(80,480);
    versionLabel=new QLabel(tr("Flappy Bird clone with C++"),this);
    versionLabel->move(145,320);
}

RedyBoard::~RedyBoard()
{

}

void RedyBoard::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    painter.setWindow(0,0,380,500);
    pix.load(":/Images/ready.png");
    painter.drawPixmap(0,0,380,500,pix);
}
