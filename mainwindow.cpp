#include "mainwindow.h"
#include <QDesktopWidget>
#include <QMessageBox>
#include <ctime>
#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    // the constructor creates the entire game interface and time handling rules
    // the creation order of the objects is the final window hierarchy order
    // this->setMaximumSize(380,500);
    //  this->setMinimumSize(380,500);

    this->setFixedSize(380,500);	// the size of the locked window
    this->setWindowIcon(QIcon(":/Images/bird1.png"));
    this->move((QApplication::desktop()->width()-this->width())/2,(QApplication::desktop()->height()-this->height())/2);	// make the window centered on the screen, using QDesktopWidget

    for(int i=0;i<pipeCount;i++)	// create pipeline object
        pipe[i]=new Pipe(this);

    createPipe();
    pipeTimer =new QTimer(this);	
    // timer that creates the pipeline
    connect(pipeTimer,SIGNAL(timeout()),this,SLOT(pipeAction()));
    connect(pipeTimer,SIGNAL(timeout()),this,SLOT(collisDete()));
    connect(pipeTimer,SIGNAL(timeout()),this,SLOT(scoreDete()));
    pipeTValue=8;

    birds=new Bird(this);
    birds->move(60,250);

    this->fpV[0]=-3;
    this->fpV[1]=-4;
    this->fpV[2]=-3;
    this->fpV[3]=-2;
    this->fpV[4]=-1;
    this->fpV[5]=-1;
    this->fpV[6]=-1;
    this->fpV[7]=0;
    this->fpV[8]=1;
    this->fpV[9]=1;
    this->fpV[10]=2;
    this->fpV[11]=2;
    this->fpV[12]=2;
    this->fpV[13]=3;
    this->fpV[14]=3;

    this->fpp=0;


    birdTimer=new QTimer(this);
    connect(birdTimer,SIGNAL(timeout()),this,SLOT(birdAction()));
    //birdTimer->start(20);
    birdV=0;
    gamemod=redy;		// game state is changed to redy

    score=0;
    top=0;
    loadTop();		// load score record

    scoreLCD=new FBNumLCD(this);	// implementation of the scoreboard
    scoreLCD->setShowHead(false);	// to stop display extra digits
    scoreLCD->setValue(0);
    //scoreLCD->setFixedSize(28*4,36);
    scoreLCD->move(140,-100);

    scoreBoard=new ScoreBoard(this);
    scoreBoard->move(-350,100);
    scobTimer=new QTimer(this);
    connect(scobTimer,SIGNAL(timeout()),this,SLOT(scbAction()));


    thisGround=new Ground(this);
    thisGround->move(0,450);

    myredyBoard=new RedyBoard(this);
    myredyBoard->setFixedSize(380,500);
    myredyBoard->move(0,0);

    replay=new QLabel(this);
    QPixmap pix;
    pix.load(":/Images/replay.png");		// all the materials are written in the resource file and will be added to the application when compiling
    replay->setPixmap(pix);
    replay->setFixedSize(140,80);
    replay->move(120,-400);

    //////sound
    playList=new QMediaPlaylist;
    QFileInfo info;
    info.setFile("sounds/sfx_wing.mp3");
    playList->addMedia(QUrl::fromLocalFile(info.absoluteFilePath()));
    info.setFile("sounds/sfx_point.mp3");
    playList->addMedia(QUrl::fromLocalFile(info.absoluteFilePath()));
    info.setFile("sounds/sfx_die.mp3");
    playList->addMedia(QUrl::fromLocalFile(info.absoluteFilePath()));
    info.setFile("sounds/sfx_hit.mp3");
    playList->addMedia(QUrl::fromLocalFile(info.absoluteFilePath()));
    info.setFile("sounds/sfx_swooshing.mp3");
    playList->addMedia(QUrl::fromLocalFile(info.absoluteFilePath()));

    playList->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);

    playList_wing=new QMediaPlaylist;
    info.setFile("sounds/sfx_wing.mp3");
    playList_wing->addMedia(QUrl::fromLocalFile(info.absoluteFilePath()));
    playList_wing->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
    playList_wing->setCurrentIndex(1);

    media=new QMediaPlayer;
    media->setVolume(100);
    media->setPlaylist(playList_wing);

    isFlag=-1;
    media2=new QMediaPlayer;
    media2->setPlaylist(playList);
    media2->setVolume(100);
    isHit=0;

    this->setWindowTitle("Qt FlappyBird");		// set title
}

void MainWindow::createPipe()		// initialize the pipe and make it out of the map in a certain order
{
    int startx=500+380;		// first pipe location
    pipeXgap=200;		// pipe spacing
    int pipR;			// generate a random number in the vertical position
    qsrand(time(NULL));		// qt random number function (qrand)

    for(int i=0;i<pipeCount;i++)
    {
        pipR=qrand()%200;

        pipe[i]->move(startx+i*pipeXgap,-200+pipR);
        lastPipe=i;		// set last pipe number
    }
}

MainWindow::~MainWindow()
{

}

void MainWindow::paintEvent(QPaintEvent *)		// overloaded drawing events. Used to generate background
{
    QPainter painter(this);
    QPixmap pixmap;
    pixmap.load(":/Images/bg.png");
    painter.drawPixmap(0,0,380,450,pixmap);
}

void MainWindow::mousePressEvent(QMouseEvent *event)	// mouse event
{
    // both the left and right buttons can control the movement of the birds, and to prevent flying out of the map, birds->pos().y()>0 has been added.
    if((event->button()==Qt::LeftButton||event->button()==Qt::RightButton)&&birds->pos().y()>0)
    {
        if(gamemod==stop)		// if the game is stopped, that is, in a failed state, the event can only be triggered after the scoreboard is in place.
        {
            if(isScobOk)
                if((event->pos().x()>=120&&event->pos().x()<=260)&&(event->pos().y()>=400&&event->pos().y()<=480))
                {
                    // here When you click the start button, the scoreboard will exit the map, the bird will return, the pipe will return, the game state will change to redy, the scoreboard will be cleared, and the road will begin to move
                    gameRedy();
                    playSound(s_sw);
                }

        }
        else
        {
            mainAction();  // the rest of the event processing is written in the function because it is universal for keyboard and mouse.
        }

    }
}



void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // keyboard handling, space, up key can be manipulated
    if((event->key()==Qt::Key_Space||event->key()==Qt::Key_Up)&&birds->pos().y()>0)
    {
        mainAction();
    }
}

void MainWindow::mainAction()
{
    // general event handling
    if(gamemod==redy)	// if it is redy: start the game, the bird starts to move, the pipe starts to move
    {
        gameStart();
        timedata=8;
        birdTimer->start(timedata);
        pipeTimer->start(pipeTValue);
        fpp=0.0;
        birdV=fpV[int(fpp)];		// every time it is triggered, the bird will fly upwards, which means the speed is negative.
        emit birds->fly();	// this signal is of the Bird type to make the bird raise its head
    }
    else if(gamemod==start)	// if the game is in the starting state, only the bird's flying posture is processed
    {
        fpp=0.0;
        birdV=fpV[int(fpp)];
        timedata=8;
        birdTimer->setInterval(timedata);
        emit birds->fly();
    }
    playWingSound();
}

void MainWindow::birdAction()
{
    // the movement time of the bird. This is where the degree of difficulty of bird movement is determined.
    birds->move(birds->pos().x(),birds->pos().y()+birdV);

    if(fpp<14.0)
        fpp+=0.2;
    else
        fpp=14.0;
    birdV=fpV[int(fpp)];

    if(birds->pos().y()+birds->height()>=450)
    {
        birds->move(birds->pos().x(),450-birds->height()+5);
        birdTimer->stop();
        isHit=true;
        playSound(s_hit);
        gameLose();
        birds->setRale(90);
    }

}

void MainWindow::pipeAction()
{
    // animation of the pipe, the focus is that after the pipeline leaves the map, it will return to the right side and take over the position of lastPipe, generating a new height
    int pipR;
    for(int i=0;i<pipeCount;i++)
    {
        pipe[i]->move(pipe[i]->pos().x()-1,pipe[i]->pos().y());
        if(pipe[i]->pos().x()<-100)
        {
            pipR=qrand()%200;
            pipe[i]->move(pipe[lastPipe]->pos().x()+pipeXgap,-200+pipR);
            lastPipe=i;
        }
    }
}

void MainWindow::collisDete()
{
    // for collision detection
    int birdRx=birds->pos().x()+30;
    int birdDy=birds->pos().y()+30;
    for(int i=0;i<pipeCount;i++)
    {
        if(birdRx>=pipe[i]->x()&&birds->pos().x()<=pipe[i]->pos().x()+pipe[i]->width()-10)
        {
            if(birds->y() <= (pipe[i]->y()+pipe[i]->getH1()) || birdDy >= (pipe[i]->y()+pipe[i]->getH1()+pipe[i]->getGap()))
                gameLose();
        }
    }
}

void MainWindow::scoreDete()
{
    // score detection is essentially a collision detection
    for(int i=0;i<pipeCount;i++)
    {
        if(birds->pos().x()+birds->width()>=pipe[i]->pos().x()+35&&birds->pos().x()+birds->width()<=pipe[i]->pos().x()+40&&cx)
        {
            playSound(s_point);
            this->score+=1;
            scoreLCD->setValue(score);
            //birds->fly();
            if(score>=1000)
            {
                scoreLCD->move(90+14*3,50);
            }
            else if(score>=100)
            {
                scoreLCD->move(90+14*2,50);
            }
            else if(score>=10)
            {
                scoreLCD->move(90+14,50);
            }
            cx=0;
        }

        if(birds->pos().x()>=pipe[i]->pos().x()+68&&birds->pos().x()<=pipe[i]->pos().x()+73)
            cx=1;
    }
}


void MainWindow::scbAction()
{
    // scoreboard animation on failure
    scoreBoard->move(scoreBoard->pos().x()+1,scoreBoard->pos().y());
    if(scoreBoard->pos().x()>=40)
    {
        scoreBoard->move(40,scoreBoard->pos().y());
        scobTimer->stop();
        replay->move(120,400);
        isScobOk=1;
    }
}

void MainWindow::gameRedy()
{
    myredyBoard->show();
    scoreBoard->move(-350,100);
    birds->move(60,250);
    replay->move(120,-400);
    createPipe();
    birds->setRale(-50);
    gamemod=redy;
    scoreLCD->setValue(0);
    thisGround->play();
    birds->play();
}

void MainWindow::gameLose()
{
    // handling when the game fails: the scoreboard slides out, the game state fails, the ground stops, the mouse and keyboard time is locked, and the score is calculated
    isScobOk=0;
    gamemod=stop;
    birds->stop();
    pipeTimer->stop();
    thisGround->stop();
    if(!isHit)
    {
        playSound(s_die);
    }
    else
        isHit=false;

    if(score>top)	// if a new record is created, save the record
    {
        top=score;
        saveTop();
    }
    scoreBoard->setScore(score,top);
    scobTimer->start(3);
    scoreLCD->move(140,-100);

}

void MainWindow::gameStart()
{
    // action to start the game
    gamemod=start;
    myredyBoard->close();
    cx=1;
    score=0;
    scoreLCD->move(90,50);

}

void MainWindow::saveTop()
{
    // save the record, save it in binary, and store it in the top.d file.
    QFile file("top.d");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out<<this->top;
}

void MainWindow::loadTop()
{
    // read record is triggered in the constructor
    QFile file("top.d");
    if(file.open(QIODevice::ReadOnly))
    {
        QDataStream in(&file);
        in>>this->top;
    }
    else
        top=0;
}

void MainWindow::playWingSound()
{
    media->stop();

    media->play();
}

void MainWindow::playSound(int flag)
{
    if(isFlag!=flag)
    {
        playList->setCurrentIndex(flag);
    }
    media2->play();
}
