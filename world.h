#ifndef WORLD_H
#define WORLD_H
#include <Qt>
#include <QObject>
#include <cstdlib>

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
                if (getBrick(snakeXPos, snakeYPos) == 9) {
                        world[snakeXPos][snakeYPos] = 4;
                        food += 2;
                        placeApple();
                }
                if ( getBrick(snakeXPos, snakeYPos) == 4) {
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
        bool skipNext;
        World(int x, int y, QObject *parent = 0) : QObject (parent) {
                skipNext = false;
                xsize = x; ysize = y;
                snakeXPos = 3;
                snakeYPos = 3;
                direction = 0;
                food = 2;
                world = new int*[x];
                for (int i = 0; i < x; i++) {
                        world[i] = new int[y];
                        for (int j = 0; j < y; j++) {
                                world[i][j] = 4;
                        }
                }
                world[snakeXPos][snakeYPos] = direction;
                placeApple();
        }

        int getBrick(int x, int y) {
                if ( inBounds(x, y) ) {
                        return world[x][y];
                } else {
                        return 5;
                }
        }

        void changeDir(int dir) {
                if (dir == Qt::Key_W) {
                        direction = 0;
                } else if (dir == Qt::Key_A) {
                        direction = 1;
                } else if (dir == Qt::Key_S) {
                        direction = 2;
                } else if (dir == Qt::Key_D) {
                        direction = 3;
                }
                skipNext = false;
                step();
                skipNext = true;
        }

        bool removeEnd(int x, int y) {
                if (3 < world[x][y]) {
                        return false;
                }
                int dir = world[x][y];
                int nx = x - getRelXDir(dir);
                int ny = y - getRelYDir(dir);
                if (ny == y && nx == x) {
                        return true;
                } else {
                        if (!removeEnd(nx, ny)) {
                                world[x][y] = 4;
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
                if (dir == 1) {
                        return -1;
                } else if (dir == 3) {
                        return 1;
                }
                return 0;

        }
        int getRelYDir(int dir) {
                if (dir == 0) {
                        return 1;
                } else if (dir== 2) {
                        return -1;
                }
                return 0;
        }

        void endGame() {
                for (int i = 0; i < xsize; i++) {
                        for (int j = 0; j < ysize; j++) {
                                if (world[i][j] < 4) {
                                        world[i][j] = 8;
                                }
                        }
                }
        }
        bool isSpaceLeft() {
                for (int i = 0; i < xsize; i++) {
                        for (int j = 0; j < ysize; j++) {
                                if (world[i][j] == 4) {
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
                        if (world[apX][apY] == 4) {
                                world[apX][apY] = 9;
                                break;
                        }
                }
        }
};

#endif // WORLD_H
