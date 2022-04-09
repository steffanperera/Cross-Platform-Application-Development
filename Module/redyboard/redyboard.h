#ifndef REDYBOARD_H
#define REDYBOARD_H

#include <QWidget>
#include <QtGui>
#include <QPainter>
#include <QPixmap>
#include <QLabel>

// preparation status prompt board
class RedyBoard : public QWidget
{
    Q_OBJECT

public:
    RedyBoard(QWidget *parent = 0);
    ~RedyBoard();
protected:
    void paintEvent(QPaintEvent *);

private:
    QLabel *label;
    QLabel *versionLabel;	// version Information
};

#endif // REDYBOARD_H
