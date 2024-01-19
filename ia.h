#ifndef H_IA
#define H_IA

#include <assert.h>
#include "sokoban.h"


struct state
{
    cell** lvl;
    struct state* suc [4];
    int playerX, playerY;
    int flag;
};
typedef struct state state;


struct liste_movement
{
    enum Dir dir;
    struct liste_movement* suc;
    struct liste_movement* prev;
};
typedef struct liste_movement liste_movement;


state* createState(cell** lvl, int playerX, int playerY);
state* movePlayer_ia(enum Dir dirrection, state* parrentState, state* root);
int isStateLock(state* state);
int isMoveChange(enum Dir dirrection, state* state);
int isDejaVu(cell** evalLvl, state* root);
int forceBrute(state* currentState, state* root, char opti);
void drawSolve(state* state, int count, float time);
int countSolve(state* state);
int isStateLockUpgrade(state* state);
int findDeadEnd(state* state, int i, int j, enum Dir dirrection, enum Dir adjWall);

#endif