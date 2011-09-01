#ifndef WORLD_H
#define WORLD_H
#include <Qt>

class World {
        public:
                int **world;
                int snakeXPos, snakeYPos;
                int direction;
                int xsize, ysize;

                World(int x, int y) {
                        xsize = x; ysize = y;
                        snakeXPos = 3;
                        snakeYPos = 3;
                        direction = 0;
                        world = new int*[x];
                        for (int i = 0; i < x; i++) {
                                world[i] = new int[y];
                                for (int j = 0; j < y; j++) {
                                        world[i][j] = 4;
                                }
                        }
                        world[snakeXPos][snakeYPos] = direction;
                        step(); step();
                }

                int getBrick(int x, int y) {
                        if ( 0 <= x && x < xsize && 0 <= y && y < ysize) {
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
                }

                void step() {
                        int oldXPos = snakeXPos;
                        int oldYPos = snakeYPos;
                        if (direction == 0) {
                                snakeYPos++;
                        } else if (direction == 1) {
                                snakeXPos--;
                        } else if (direction== 2) {
                                snakeYPos--;
                        } else if (direction == 3) {
                                snakeXPos++;
                        }
                        world[oldXPos][oldYPos] = 4;
                        world[snakeXPos][snakeYPos] = direction;
                }

};

#endif // WORLD_H
