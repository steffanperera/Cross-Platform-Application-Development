#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
//#include <QLCDNumber>
#include <QTimer>
#include <QDataStream>
#include <QLabel>
#include <QPainter>
#include <QPixmap>

#include <Module/bird/bird.h>
#include <Module/ground/ground.h>
#include "Module/pipe/pipe.h"
#include "Module/scoreboard/scoreboard.h"
#include "Module/fbnumLCD/fbnumlcd.h"
#include "Module/redyboard/redyboard.h"

#include <QtGui>
#include <QMouseEvent>
#include <math.h>
//#include <QSettings>
#include <QDataStream>
#include <QFile>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia>
#include <QUrl>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *);    // draw a background picture
    void mousePressEvent(QMouseEvent *event);	// handling of mouse click time
    void keyPressEvent(QKeyEvent *event);	// keyboard event handling

private slots:				
    void birdAction();			// bird's action processing
    void pipeAction();			// pipe action processing
    void collisDete();			// impact checking
    void scoreDete();			// scoring check function
    void scbAction();			// scoreboard animation
    void playWingSound();
    void playSound(int flag);

private:
    void mainAction();			// main trigger As the execution function of mouse and keyboard event processing

    void createPipe();			// initialize to create a pipeline
    void gameLose();			// handling when the game fails
    void gameStart();			// processing at the beginning of the game
    void gameRedy();

    void saveTop();			// store the highest score
    void loadTop();			// read the highest score

private:	
    Bird *birds;			// create bird
    double birdV;			// bird speed
    double timedata;			// bird's timer timestamp
    QTimer *birdTimer;			// timer to control bird movement
    double fpV[15];
    double fpp;
   // QTimer *pipeTimer;
    enum{stop=0,start=1,redy=2};		// 3 enumerated values ​​represent the current game state
    int gamemod;		// game status

    RedyBoard *myredyBoard;	// preparation panel at the beginning

    enum{pipeCount=5};		// constant for the number of pipes
    Pipe *pipe[pipeCount];		// create pipeCount pipes
    QTimer *pipeTimer;		// the remote processing of the pipeline Timer and the timer undertakes multiple main processing tasks of the window
    int pipeTValue;
    int pipeXgap;		// spacing between pipes
    int lastPipe;		// the rightmost pipe mark

    Ground *thisGround;		// scroll animation at the bottom

    ScoreBoard *scoreBoard;	// scoreboard
    bool isScobOk;		// display status of the scoreboard
    int score;			// fraction
    int top;			// highest record
    bool cx;			// prevent the status indicator of repeated points
    QTimer *scobTimer;		// scoreboard processing is triggered when the game is over
    FBNumLCD *scoreLCD;		// small scoreboard
    QLabel *replay;		// start button under the final scoreboard

    QMediaPlayer *media;
    QMediaPlayer *media2;
    enum{s_wing=0,s_point=1,s_die=2,s_hit=3,s_sw=4};  
    QMediaPlaylist *playList;
    QMediaPlaylist *playList_wing;
    int isFlag;
    bool isHit;
};

#endif // MAINWINDOW_H
