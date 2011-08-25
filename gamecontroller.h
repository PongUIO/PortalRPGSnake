#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H
#include<QtGui>
#include<stdlib.h>
#include<typeinfo>

#include "render.h"
#include "customlabel.h"
#include "gamebutton.h"

#ifndef RATE
#define RATE 60
#endif

void something() {
	qDebug() << "works.";
}

class GameController {
public:
	QWidget *main;
	QTimer *glTimer, *camTimer, *timer;
	Camera *cam;
#ifdef WIN32
	QPalette *p;
#endif
        GameController(QWidget *parent = 0) {

		main = new QWidget(parent);
		main->showFullScreen();
                main->resize(parent->geometry().width(), parent->geometry().height());
		cam = new Camera(0, 0, main->width(), main->height());
                MyGLDrawer *drawer = new MyGLDrawer(cam, main);

		glTimer = new QTimer(main);
		drawer->connect(glTimer, SIGNAL(timeout()), drawer, SLOT(redraw()));
		glTimer->start(0);

		camTimer = new QTimer(main);
                drawer->connect(camTimer, SIGNAL(timeout()), drawer, SLOT(moveMouseCheck()));
		camTimer->start(1./RATE);

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
