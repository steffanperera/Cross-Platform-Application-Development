#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QLCDNumber>
#include "Module/fbnumLCD/fbnumlcd.h"

// scoreboard class is purely used to display results
class ScoreBoard : public QWidget
{
    Q_OBJECT

public:
    ScoreBoard(QWidget *parent = 0);
    ~ScoreBoard();

    void setScore(int score,int top);	
    // set the displayed value: score, highest score.

protected:
    void paintEvent(QPaintEvent *);

private:
    int Score;		// score
    int top;		// top score
    QString medal[3];	// road strength for medals
    FBNumLCD *scoreLcd;	// score display
    FBNumLCD *topLcd;		// top score display
};

#endif // SCOREBOARD_H
