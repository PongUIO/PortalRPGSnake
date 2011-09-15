#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H
#include<QtGui>
#include<Qt>
#include<stdlib.h>
#include<typeinfo>

#include "snake.h"
#include "render.h"
#include "customlabel.h"
#include "gamebutton.h"
#include "world.h"

#ifndef RATE
#define RATE 60
#endif

#ifndef GAMERATE
#define GAMERATE 4
#endif

void something() {
	qDebug() << "works.";
}

class GameController {
public:
	QWidget *main;
        QTimer *glTimer, *camTimer, *timer, *gameTimer;
        Camera *cam;
	World *world;
	Snake *snake;
#ifdef WIN32
	QPalette *p;
#endif
        GameController(QWidget *parent = 0) {
		snake = new Snake();
		main = new QWidget(parent);
                world = new World(16, 16, snake);
                main->showFullScreen();
		main->resize(parent->geometry().width(), parent->geometry().height());
		cam = new Camera(-5, -5, main->width(), main->height());
		MyGLDrawer *drawer = new MyGLDrawer(cam, world, main);
		glTimer = new QTimer(main);
		drawer->connect(glTimer, SIGNAL(timeout()), drawer, SLOT(redraw()));
		glTimer->start(0);

		camTimer = new QTimer(main);
                drawer->connect(camTimer, SIGNAL(timeout()), drawer, SLOT(moveMouseCheck()));
                camTimer->start(1000./RATE);

                gameTimer = new QTimer(main);
                drawer->connect(gameTimer, SIGNAL(timeout()), world, SLOT(step()));


                main->setWindowTitle(QApplication::translate("childwidget", "Child widget"));
		drawer->setMouseTracking(true);

#ifdef _WIN32
		drawer->resize(main->width(), main->height()-1);
		p = new QPalette( main->palette() );
		p->setColor( QPalette::Window, Qt::black );
		main->setPalette( *p );
#else
		drawer->resize(main->width(), main->height());
#endif
		drawer->show();
	}
	void showAll() {
		main->show();
	}
	void hideAll() {
		main->hide();
	}
};

#endif // GAMEWIDGET_H
