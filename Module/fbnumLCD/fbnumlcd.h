#ifndef FBNUMLCD_H
#define FBNUMLCD_H

#include <QWidget>
#include <QtGui>
#include <QPainter>
#include <QPixmap>


// this is a kind of flappybird font that displays numbers
class FBNumLCD : public QWidget
{
    Q_OBJECT

public:
    FBNumLCD(QWidget *parent = 0);
    ~FBNumLCD();

    void setValue(int v); // settings
    int value() const; // read value
    void setShowHead(bool s); // set whether to display the prefix 0 bit

protected:
    void paintEvent(QPaintEvent *);

private:
    void runtime(); // perform a conversion operation

private:
    QString lcdList[11];
    int data;
    enum{MaxSize=4};
    int num[MaxSize];
    int len;
    bool showHead;
};

#endif // FBNUMLCD_H
