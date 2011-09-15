#ifndef WORLD_H
#define WORLD_H
#include <Qt>
#include <QObject>
#include <cstdlib>
#include <iostream>
#include <QDebug>

#include <tiles.h>

#include "snake.h"

class World : public QObject {
        Q_OBJECT
public slots:
        void step() {
		if (loading) {
			return;
		}
		snake->update();
                if (skipNext) {
                        skipNext = false;
                        return;
                }
		if (snake->x == -1) {
                        return;
                }

                if (snake->getPowerup(POWER_SLOWSPEED) && snake->shouldSkipMove()) {
                        return;
                }

		int oldX = snake->x;
                int oldY = snake->y;
                snake->x += getRelXDir(snake->direction);
                snake->y += getRelYDir(snake->direction);
		if (!snake->food) {
                        removeEnd(oldX, oldY, false, false);
		} else {
			snake->food--;
		}
		if (getBlock(snake->x, snake->y) == PORTALORANGE ) {
			gotoPortal(&snake->x, &snake->y, PORTALBLUE, true);
		} else if ( getBlock(snake->x, snake->y) == PORTALBLUE ) {
			gotoPortal(&snake->x, &snake->y, PORTALORANGE, true);
		} else if (getBlock(snake->x, snake->y) == DEADSNAKE) {
			snake->food++;
			world[snake->x][snake->y] = GRASS;
		}
		while (getBlock(snake->x, snake->y) == APPLE) {
			world[snake->x][snake->y] = GRASS;
			snake->xp += 3;
			placeApple();
		}
		if ( getBlock(snake->x, snake->y) == GRASS) {
                        world[snake->x][snake->y] = snake->direction;
                } else {
                        if (getBlock(snake->x, snake->y) == GLASS) {
                                snake->damage(5);
                                world[snake->x][snake->y] = snake->direction;
                        } else if (getBlock(snake->x, snake->y) == WALL) {
                                snake->damage(15);
                                world[snake->x][snake->y] = snake->direction;
                        } else if (getBlock(snake->x, snake->y) == WALLOUTSIDE) {
                                snake->hp = -1;
                        }
                        snake->update();
                }

        }
public:
	Snake *snake;
	char **world;
        int xsize, ysize, bluePortalDir, orangePortalDir, bluePortalX, bluePortalY, orangePortalX, orangePortalY;
	bool skipNext, portalColor;
	int blueDir, orangeDir;
        bool loading;
	World(int x, int y, Snake *snake, QObject *parent = 0) : QObject (parent) {
		loading = true;
		this->snake = snake;
                xsize = x; ysize = y;
		world = new char*[x];
		for (int i = 0; i < x; i++)
		{
			world[i] = new char[y];
		}
                init();
		loading = false;
        }

        void init() {
		bluePortalDir = orangePortalDir = bluePortalX = bluePortalY = orangePortalX = orangePortalY = -1;
		snake->init();
		skipNext = portalColor = false;
                for (int i = 0; i < xsize; i++) {
                        for (int j = 0; j < ysize; j++) {
                                world[i][j] = (j != 5)*(GRASS + (j == 0 || j == ysize-1 || i == 0 || i == xsize-1) *(WALL-GRASS)) + (j == 5)*GLASS;
                        }
                }
                world[snake->x][snake->y] = snake->direction;
		for (int i = 0; i < 64; i++) {
			placeApple();
		}
        }

        int gotoPortal(int *x, int *y, int color, bool changeDir) {
		if (color == PORTALBLUE) {
			*x = bluePortalX;
			*y = bluePortalY;
		} else {
			*x = orangePortalX;
			*y = orangePortalY;
		}
                int dir = color == PORTALORANGE ? orangePortalDir : bluePortalDir;
                *y += getRelYDir(dir);
                *x += getRelXDir(dir);
                if (changeDir) {
                        snake->direction = dir;
                }
        }

        int getBlock(int x, int y) {
                if ( inBounds(x, y) ) {
                        return world[x][y];
                } else {
                        return WALLOUTSIDE;
                }
        }

        void handleInput(int key) {
                bool stepAfter = true;

                if (key == Qt::Key_W) {
                        if (snake->direction == SNAKEDOWN) {
                                return;
                        }
                        snake->direction = SNAKEUP;
                } else if (key == Qt::Key_A) {
                        if (snake->direction == SNAKERIGHT) {
                                return;
                        }
                        snake->direction = SNAKELEFT;
                } else if (key == Qt::Key_S) {
                        if (snake->direction == SNAKEUP) {
                                return;
                        }
                        snake->direction = SNAKEDOWN;
                } else if (key == Qt::Key_D) {
                        if (snake->direction == SNAKELEFT) {
                                return;
                        }
                        snake->direction = SNAKERIGHT;
                } else {
                        stepAfter = false;

                        if (key == Qt::Key_Control) {
                                portalColor = !portalColor;

                        } else if (key == Qt::Key_Up) {
                                //if (revDir == SNAKEUP) { return; }
                                shootPortal(SNAKEUP);

                        } else if (key == Qt::Key_Left) {
                                //if (revDir == SNAKELEFT) { return; }
                                shootPortal(SNAKELEFT);

                        } else if (key == Qt::Key_Down) {
                                //if (revDir == SNAKEDOWN) { return; }
                                shootPortal(SNAKEDOWN);

                        } else if (key == Qt::Key_Right) {
                                //if (revDir == SNAKERIGHT) { return; }
                                shootPortal(SNAKERIGHT);
                        } else if (key == Qt::Key_R) {
                                init();
                        } else if (key == Qt::Key_1) {
                                snake->togglePowerup(POWER_SLOWSPEED);
                        }
                }

                if (stepAfter) {
                        snake->forceStep();
                        skipNext = false;
                        step();
                        skipNext = true;
                }
        }

        void shootPortal(int dir) {
		// snake->x, snake->y
                int portalX = 0, portalY = 0;
                if (getFirstWallForDirection(dir, &portalX, &portalY)) {
                        removeEnd(snake->x-getRelXDir(snake->direction), snake->y-getRelYDir(snake->direction), true, false);
                        closeOldPortal();
                        world[portalX][portalY] = portalColor ? PORTALBLUE : PORTALORANGE;
                        if (portalColor) {
				bluePortalX = portalX;
				bluePortalY = portalY;
                                bluePortalDir = flipDir(dir);
                        } else {
				orangePortalX = portalX;
				orangePortalY = portalY;
				orangePortalDir = flipDir(dir);
                        }
                        portalColor = !portalColor;
                }
        }

        void closeOldPortal() {
                int x = 0; int y = 0;
 //               findFirstBlock(&x, &y, color);
		if (portalColor) {
			if (bluePortalX != -1) {
				world[bluePortalX][bluePortalY] = WALL;
			}
		} else {
			if (orangePortalX != -1) {
				world[orangePortalX][orangePortalY] = WALL;
			}
		}
                if (x != -1) {
                        world[x][y] = WALL;
                }

        }

        void findFirstBlock(int *inx, int *iny, int type) {
                for (int x = 0; x < xsize; x++) {
                        for (int y = 0; y < ysize; y++) {
                                if (getBlock(x, y) == type) {
                                        *inx = x;
                                        *iny = y;
                                        return;
                                }
                        }
                }
                *inx = -1;
                *iny = -1;

        }

        bool getFirstWallForDirection(int dir, int *x, int *y) {
                int max = 0;

                if ( dir == SNAKEUP || dir == SNAKEDOWN ) {
                        max = ysize;
                } else if (dir == SNAKELEFT || dir == SNAKERIGHT) {
                        max = xsize;
                } else {
                        return false;
                }

		int testX = snake->x, testY = snake->y;
                for (int i = 0; i < max; i++) {
                        testX += getRelXDir(dir);
                        testY += getRelYDir(dir);

                        if (!inBounds(testX, testY)) {
                                return false;
                        }

                        if (getBlock(testX, testY) == WALL) {
                                *x = testX;
                                *y = testY;

                                return true;
                        }
                }

                return false;
        }

        bool removeEnd(int x, int y, bool killAfterPortal, bool afterPortal) {
		if (x == snake->x && y == snake->y) {
                        return false;
		}
		int block = world[x][y];
                if (block == PORTALORANGE) {
			gotoPortal(&x, &y, PORTALBLUE, false);
                        block = world[x][y];
                        afterPortal = true;
                }
                if (block == PORTALBLUE) {
                        gotoPortal(&x, &y, PORTALORANGE, false);
                        block = world[x][y];
                        afterPortal = true;
                }
		if (SNAKERIGHT < block) {
			return false;
		}
                if (killAfterPortal && afterPortal) {
                        world[x][y] = DEADSNAKE;
                }
                int nx = x - getRelXDir(block);
                int ny = y - getRelYDir(block);
                if (ny == y && nx == x) {
                        return true;
                } else {
                        if (!removeEnd(nx, ny, killAfterPortal, afterPortal)) {
                                if ( !killAfterPortal) {
                                       world[x][y] = GRASS;
                                }
                                return true;
                        } else {
                                return true;
                        }
                }
        }

        bool inBounds(int x, int y) {
                return 0 <= x && x < xsize && 0 <= y && y < ysize;
        }

        int getRelXDir(int dir) {
                if (dir == SNAKELEFT) {
                        return -1;
                } else if (dir == SNAKERIGHT) {
                        return 1;
                }
                return 0;

        }
        int getRelYDir(int dir) {
                if (dir == SNAKEUP) {
                        return 1;
                } else if (dir== SNAKEDOWN) {
                        return -1;
                }
                return 0;
        }
        int flipDir(int dir) {
                return (dir + 2)%4;
        }

        bool isSpaceLeft() {
                for (int i = 0; i < xsize; i++) {
                        for (int j = 0; j < ysize; j++) {
                                if (world[i][j] == GRASS) {
                                        return true;
                                }
                        }
                }
                return false;
        }

        void placeApple() {
                if (!isSpaceLeft()) {
                        return;
                }
                int apX, apY;
                while (true) {
                        apX = rand()%xsize;
                        apY = rand()%ysize;
                        if (world[apX][apY] == GRASS) {
                                world[apX][apY] = APPLE;
                                break;
                        }
                }
        }
        bool isColliding(int x, int y) {
                int v = getBlock(x, y);
                return v == WALL || v == WALLOUTSIDE;
        }
};

#endif // WORLD_H
