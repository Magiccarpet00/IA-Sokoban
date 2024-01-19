#ifndef H_SOKOBAN
#define H_SOKOBAN

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <assert.h>

// #define BOARD_MAX_SIZE_Y 8
// #define BOARD_MAX_SIZE_X 14

#define BOARD_MAX_SIZE_Y 11
#define BOARD_MAX_SIZE_X 20

enum ObjStatic{
    FLOOR,
    WALL,
    GOAL
};


enum ObjDynamic{
    PLAYER,
    BOX,
    NONE
};


enum Dir{
    UP,
    LEFT,
    DOWN,
    RIGHT
};


struct cell
{
    enum ObjDynamic objDynamic;
    enum ObjStatic objStatic;
};
typedef struct cell cell;


int setUpShell();
int isLvlFinish(cell** lvl);
void draw(cell** lvl);
void input(cell** lvl);
void clearLevel(cell** lvl);
void findPlayer(int* _x, int* _y, cell ** lvl);
cell** createBlankLevel();
cell** copyLevel(cell ** lvl);
cell** loadLevel(char* lvlName);

// void update(cell** lvl);
// cell** movePlayer(enum Dir dirrection, cell** lvl);
#endif
