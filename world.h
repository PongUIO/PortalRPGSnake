#ifndef WORLD_H
#define WORLD_H
#include <Qt>
#include <QObject>
#include <cstdlib>

#include <tiles.h>

class World : public QObject {
       Q_OBJECT
public slots:
        void step() {
                if (skipNext) {
                        skipNext = false;
                        return;
                }
                if (snakeXPos == -1) {
                        return;
                }
                int oldXPos = snakeXPos;
                int oldYPos = snakeYPos;
                snakeXPos += getRelXDir(direction);
                snakeYPos += getRelYDir(direction);
                if (!food) {
                        removeEnd(oldXPos, oldYPos);
                } else {
                        food--;
                }
                while (getBlock(snakeXPos, snakeYPos) == APPLE) {
                        world[snakeXPos][snakeYPos] = GRASS;
                        food += 2;
                        placeApple();
                }
                if ( getBlock(snakeXPos, snakeYPos) == GRASS) {
                        world[snakeXPos][snakeYPos] = direction;
                } else {
                        snakeXPos = -1;
                        snakeYPos = -1;
                        endGame();
                }
        }
public:
        int **world;
        int snakeXPos, snakeYPos;
        int direction;
        int xsize, ysize;
        int food;
        bool skipNext, portalColor;
        World(int x, int y, QObject *parent = 0) : QObject (parent) {
                xsize = x; ysize = y;
                world = new int*[x];
                for (int i = 0; i < x; i++) {
                        world[i] = new int[y];
                }
                init();
        }

        void init() {
                skipNext = portalColor = false;
                snakeXPos = 3;
                snakeYPos = 3;
                direction = SNAKEUP;
                food = 2;
                for (int i = 0; i < xsize; i++) {
                        for (int j = 0; j < ysize; j++) {
                                world[i][j] = GRASS + (j == 0 || j == ysize-1 || i == 0 || i == xsize-1) *(WALL-GRASS);
                        }
                }
                world[snakeXPos][snakeYPos] = direction;
                placeApple();
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

                /*
                int revDir = direction - 2;
                if (revDir < 0) { revDir += 4; }
                */

                if (key == Qt::Key_W) {
                        direction = SNAKEUP;
                } else if (key == Qt::Key_A) {
                        direction = SNAKELEFT;
                } else if (key == Qt::Key_S) {
                        direction = SNAKEDOWN;
                } else if (key == Qt::Key_D) {
                        direction = SNAKERIGHT;
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
                    }
                }

                if (stepAfter) {
                        skipNext = false;
                        step();
                        skipNext = true;
                }


        }

        void shootPortal(int dir) {
            // snakeXPos, snakeYPos
            int portalX = 0, portalY = 0;

            if (getFirstWallForDirection(dir, &portalX, &portalY)) {
                closeOldPortal();
                world[portalX][portalY] = portalColor ? PORTALBLUE : PORTALORANGE;
                portalColor = !portalColor;
            }
        }

        void closeOldPortal() {
            int color = portalColor ? PORTALBLUE : PORTALORANGE;
            for (int x = 0; x < xsize; x++) {
                for (int y = 0; x < ysize; y++) {

                    if (world[x][y] == color) {
                        world[x][y] = WALL;
                        return;
                    }
                }
            }
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

            int testX = snakeXPos, testY = snakeYPos;
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

        bool removeEnd(int x, int y) {
                if (SNAKERIGHT < world[x][y]) {
                        return false;
                }
                int dir = world[x][y];
                int nx = x - getRelXDir(dir);
                int ny = y - getRelYDir(dir);
                if (ny == y && nx == x) {
                        return true;
                } else {
                        if (!removeEnd(nx, ny)) {
                                world[x][y] = GRASS;
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

        void endGame() {
                for (int i = 0; i < xsize; i++) {
                        for (int j = 0; j < ysize; j++) {
                                if (world[i][j] < 4) {
                                        world[i][j] = DEADSNAKE;
                                }
                        }
                }
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
