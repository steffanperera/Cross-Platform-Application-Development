#ifndef BIRD_H
#define BIRD_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <math.h>

// bird class
class Bird : public QWidget
{
    Q_OBJECT

public:
    Bird(QWidget *parent = 0);
    ~Bird();
    void setRale(int rale);	// rotation angle
    void play();
    void stop();

signals:
    void fly(); // signal -> trigger head-up motion

private slots:
    void updateRale(); // update rotation angle

protected:
    void paintEvent(QPaintEvent *);
private:
    double rale; // angle parameter
    double x; // unit offset of rotation angle
    int zt; // angle of 30 degrees -> better bird state handling
    double ztr;
    int co;
    QTimer *timer;
    QString src[3]; // path of the bitmap
};

#endif // BIRD_H
