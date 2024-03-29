#include <QtGui>
#include <QWidget>
#include <QAbstractButton>
#include <QIcon>
#include <QDebug>

#include "main.h"
#include "customlabel.h"
#include "render.h"
#include "camera.h"
#include "menubutton.h"
#include "gamecontroller.h"

#ifndef RATE
#define RATE 60
#endif

GameController *gc;

void gameStart() {
        gc->gameTimer->start(1000/GAMERATE);
        gc->showAll();
}

int main(int argc, char *argv[]) {
        QApplication app(argc, argv);
        QWidget *menu = new QWidget();
        //menu->showFullScreen();
        //menu->resize(QApplication::desktop()->screenGeometry().width(),QApplication::desktop()->screenGeometry().height());
	menu->resize(700,700);
        menu->show();
	CustomLabel *gameButton = new CustomLabel(&gameStart, menu);
        gameButton->setPixmap(QPixmap(":/graphics/startgame.png"));
        gameButton->move(menu->geometry().width()/2-gameButton->pixmap()->width()/2, menu->geometry().height()/2-gameButton->pixmap()->height()/2);
        gameButton->show();
        gc = new GameController(menu);
        gc->hideAll();
        app.exec();
        delete gc;
        delete gameButton;
        delete menu;
        return 0;//main.exec();
}
