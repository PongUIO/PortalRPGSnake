#ifndef RENDER_H
#define RENDER_H
#include<QtGui>
#include<iostream>
#include<QGLWidget>
#include<stdlib.h>

#include "camera.h"
#include "world.h"
#include "tiles.h"
#include "snake.h"

class MyGLDrawer : public QGLWidget {
        Q_OBJECT

public slots:
        void redraw() {
                paintGL();
        }

        void moveMouseCheck() {

                cam->addVel(lastX, lastY);
        }

public:
        int lastX, lastY;
        Camera *cam;
        float colors[13][3];
        World *world;
        MyGLDrawer(Camera *cam, World *world, QWidget *parent = 0)
                        : QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
                setFocusPolicy(Qt::StrongFocus);
                this->world = world;
                this->cam = cam;
                lastX = width()/2;
                lastY = height()/2;
                for (int i = 0; i < 4; i++) {
                        colors[i][0] = 1;
                        colors[i][1] = 1;
                        colors[i][2] = 0;
                }
                colors[GRASS][0] = 0;
                colors[GRASS][1] = 0.9;
                colors[GRASS][2] = 0;
                colors[WALL][0] = 0.6;
                colors[WALL][1] = 0.25;
                colors[WALL][2] = 0;
                colors[PORTALORANGE][0] = 1;
                colors[PORTALORANGE][1] = 0.5;
                colors[PORTALORANGE][2] = 0;
                colors[PORTALBLUE][0] = 0;
                colors[PORTALBLUE][1] = 0;
                colors[PORTALBLUE][2] = 1;
                colors[DEADSNAKE][0] = 0.6;
                colors[DEADSNAKE][1] = 0.6;
                colors[DEADSNAKE][2] = 0;
                colors[APPLE][0] = 1;
                colors[APPLE][1] = 0;
                colors[APPLE][2] = 0;
                colors[WALLOUTSIDE][0] = 0.4;
                colors[WALLOUTSIDE][1] = 0.16;
                colors[WALLOUTSIDE][2] = 0;
		colors[GLASS][0] = 0.8;
		colors[GLASS][1] = 0.8;
                colors[GLASS][2] = 1;
                colors[CRATE][0] = 0.4;
                colors[CRATE][1] = 0.4;
                colors[CRATE][2] = 1;

        }

protected:
        // overridden
        void keyPressEvent (QKeyEvent *event) {
                world->handleInput(event->key());
        }

        // overriden
        void mouseMoveEvent(QMouseEvent * event) {
                lastX = event->pos().x();
                lastY = event->pos().y();
                //qDebug() << event->pos().x() << " " << event->pos().y();
        }

        // overridden  
        void mousePressEvent(QMouseEvent * event) {
                int w = width();
                int h = height();
        }



	// overridden
        void initializeGL()
        {
                // Set up the rendering context, define display lists etc.:
                glClearColor( 0.0, 0.0, 0.0, 0.0 );
                //glEnable(GL_DEPTH_TEST);
                glEnable(GL_DOUBLE);
                /*data[0].load(":/graphics/tiles/empty.png");
                textures[0] = bindTexture(data[0].scaled(64,64));
		data[1].load(":/graphics/tiles/metal.png");
                textures[1] = bindTexture(data[1].scaled(64,64));
		data[2].load(":/graphics/tiles/metal2surf.png");
                textures[2] = bindTexture(data[2].scaled(64,64));
                characters[0].load(":/graphics/characters/character1.png");
                chartextures[0] = bindTexture(characters[0].scaled(128,320).mirrored(true, false));

                weapons[0].load(":/graphics/weapons/testweapon.png");
                weaponstextures[0] = bindTexture(weapons[0].scaled(32,64));

                                glEnable(GL_TEXTURE_2D);
                glGenTextures(3,&texture[0]);
                glBindTexture(GL_TEXTURE_2D,texture[0]);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glBindTexture(GL_TEXTURE_2D, texture[0]);
                glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, gldata.width(),
                              gldata.height(),
                              0, GL_RGBA, GL_UNSIGNED_BYTE, gldata.bits() );

                glDisable(GL_TEXTURE_2D);*/

        }

        // overridden
        void resizeGL( int w, int h )
        {
                // setup viewport, projection etc.:
                glViewport( 0, 0, (GLint)w, (GLint)h );
                //glFrustum( ... );
        }

        void wheelEvent(QWheelEvent *event) {
                cam->dzoom -= event->delta()/500000.0;//pow(10, cam->zoom);
        }

        // overridden
        void paintGL()
	{
		if (world->snake->x != -1) {
			cam->x = -world->snake->x;
			cam->y = -world->snake->y;
		}
                cam->iter();
                int i, j;
                glClear(GL_COLOR_BUFFER_BIT);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();

		glOrtho(-cam->zoom, cam->zoom, -cam->zoom*cam->ratio, cam->zoom*cam->ratio, 0.01, 1000);
		glTranslatef(cam->x,cam->y,-1);

                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                // draw the scene:
                //glRotatef( ... );
                //glMaterialfv( ... );
		//glDrawPixels(data.width(), data.height(), GL_RGBA, GL_UNSIGNED_BYTE, gldata.bits());
                glEnable(GL_TEXTURE_2D);

                //       glViewport(cam->x, cam->y, getWidth(), getHeight());
                int fx = -cam->x-cam->zoom-1;
                int tx = -cam->x+cam->zoom+1;
                int fy = -cam->y-cam->zoom*cam->ratio-1;
                int ty = -cam->y+cam->zoom*cam->ratio+1;
                for (i = fx; i < tx; i++) {
                        for (j = fy; j < ty; j++) {
                                glBegin(GL_QUADS);
                                glColor3fv(colors[world->getBlock(i, j)]);
                                glVertex2f(i,j+1);  // lower left
                                glVertex2f(i,j); // lower right
                                glVertex2f(i+1,j);// upper right
                                glVertex2f(i+1,j+1); // upper left
                                glEnd();
                        }

		}
		i = world->snake->x;
		j = world->snake->y;
                if (i != -1) {
                        glBegin(GL_QUADS);
                        glColor3fv(colors[PORTALORANGE + world->portalColor]);
                        glVertex2f(i+0.3, j+0.7);  // lower left
                        glVertex2f(i+0.3,j+0.3); // lower right
                        glVertex2f(i+0.7,j+0.3);// upper right
                        glVertex2f(i+0.7,j+0.7); // upper left
                        glEnd();
		}


		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-1, 1, -1, 1, 0.01, 1000);
                glTranslatef(0,0,-1);
                double hpPercent = std::max((((double)world->snake->hp)/world->snake->maxhp)*0.96, 0.);
		double xpPercent = (((double)world->snake->xp)/world->snake->getXpToLevel());
		glBegin(GL_QUADS);
		glColor3f(hpPercent/2+0.5, 0, 0);
		glVertex2f(hpPercent, 0.99);  // lower left
		glVertex2f(hpPercent, 0.92); // lower right
		glVertex2f(-hpPercent, 0.92);// upper right
		glVertex2f(-hpPercent, 0.99); // upper left
		glColor3f(xpPercent/2+0.5, xpPercent/2+0.5, 0);
		glVertex2f(-0.96, 0.92);  // lower left
		glVertex2f(-0.96, 0.85); // lower right
		glVertex2f(-0.96 + xpPercent*1.92, 0.85);// upper right
		glVertex2f(-0.96 + xpPercent*1.92, 0.92); // upper left
		glEnd();
		glBegin(GL_QUADS);
		glColor3fv(colors[PORTALORANGE + world->portalColor]);
		glVertex2f(1, -1);  // lower left
		glVertex2f(0.95, -1); // lower right
		glVertex2f(0.95,-0.95);// upper right
		glVertex2f(1, -0.95); // upper left
		glEnd();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glEnable(GL_BLEND);

                glBlendFunc(GL_ONE, GL_ZERO);
                glDisable(GL_TEXTURE_2D);
                glDisable(GL_BLEND);
		glFlush();
		glFinish();
                swapBuffers();
        }
        double getXPix(int x) {
                return -1+(((double)x)/width())*2-cam->x;
        }
        double getYPix(int y) {
                return 1-(((double)y)/height())*2-cam->y;
        }

};
#endif
