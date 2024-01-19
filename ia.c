#include "ia.h"

state* createState(cell** lvl, int playerX, int playerY)
{
    state* newState;
    newState = malloc(sizeof(state));
    assert(newState);

    newState->playerX = playerX;
    newState->playerY = playerY;
    newState->lvl = lvl;
    newState->flag = 0;

    for (int i = 0; i < 4; i++)
        newState->suc[i] = NULL;

    return newState;    
}

int isMoveChange(enum Dir dirrection, state* state)
{
    switch (dirrection)
    {
    case UP:
        if(state->lvl[state->playerY - 1][state->playerX].objStatic == WALL)
            return 0;
        if(state->lvl[state->playerY - 1][state->playerX].objDynamic == BOX)
            if(state->lvl[state->playerY - 2][state->playerX].objDynamic == BOX ||
               state->lvl[state->playerY - 2][state->playerX].objStatic == WALL)
                return 0;
        break;

    case LEFT:
        if(state->lvl[state->playerY][state->playerX - 1].objStatic == WALL)
            return 0;
        if(state->lvl[state->playerY][state->playerX - 1].objDynamic == BOX)
            if(state->lvl[state->playerY][state->playerX - 2].objDynamic == BOX ||
               state->lvl[state->playerY][state->playerX - 2].objStatic == WALL)
                return 0;
        break;

    case DOWN:
        if(state->lvl[state->playerY + 1][state->playerX].objStatic == WALL)
            return 0;
        if(state->lvl[state->playerY + 1][state->playerX].objDynamic == BOX)
            if(state->lvl[state->playerY + 2][state->playerX].objDynamic == BOX ||
               state->lvl[state->playerY + 2][state->playerX].objStatic == WALL)
                return 0;
        break;

    case RIGHT:
        if(state->lvl[state->playerY][state->playerX + 1].objStatic == WALL)
            return 0;
        if(state->lvl[state->playerY][state->playerX + 1].objDynamic == BOX)
            if(state->lvl[state->playerY][state->playerX + 2].objDynamic == BOX ||
               state->lvl[state->playerY][state->playerX + 2].objStatic == WALL)
                return 0;
        break;
    }

    return 1;
}

int isDejaVu(cell** evalLvl, state* root)
{
    int i,j;
    int isIdentic = 1;
    int res = 0;
    for (i = 0; i < BOARD_MAX_SIZE_Y; i++)
        for (j = 0; j < BOARD_MAX_SIZE_X; j++)
            if(evalLvl[i][j].objDynamic != root->lvl[i][j].objDynamic)
                isIdentic = 0;

    if(isIdentic)
    {
        return 1;
    }
    else
    {
        for (i = 0; i < 4; i++)
            if(root->suc[i] != NULL)
                res += isDejaVu(evalLvl, root->suc[i]);
        return res;
    }

}

/*
Cette fonction crée un nouvelle etat à 
chaque movement si l'etat ne se trouve
pas deja dans le graph global

si elle trouve un etat qui existe deja elle renvoie NULL
*/
state* movePlayer_ia(enum Dir dirrection, state* parrentState, state* root)
{
    int playerCanMove = 1;
    int boxCanPush = 0;
    int deltaX = 0;
    int deltaY = 0;
    int _playerX = parrentState->playerX;
    int _playerY = parrentState->playerY;

    cell** newlvl = copyLevel(parrentState->lvl);

    switch (dirrection)
    {
    case UP:
        if(newlvl[_playerY - 1][_playerX].objDynamic == BOX)
            boxCanPush = 1;

        if(newlvl[_playerY - 1][_playerX].objStatic == WALL)
            playerCanMove = 0;

        if(boxCanPush)
            if(newlvl[_playerY - 2][_playerX].objDynamic == BOX ||
               newlvl[_playerY - 2][_playerX].objStatic == WALL )
               playerCanMove = 0;

        if(playerCanMove)
            deltaY = -1;
        break;

    case LEFT:
        if(newlvl[_playerY][_playerX-1].objDynamic == BOX)
            boxCanPush = 1;

        if(newlvl[_playerY][_playerX-1].objStatic == WALL)
            playerCanMove = 0;

        if(boxCanPush)
            if(newlvl[_playerY][_playerX - 2].objDynamic == BOX ||
               newlvl[_playerY][_playerX - 2].objStatic == WALL )
               playerCanMove = 0;

        if(playerCanMove)
            deltaX = -1;
        break;

    case DOWN:
        if(newlvl[_playerY + 1][_playerX].objDynamic == BOX)
            boxCanPush = 1;

        if(newlvl[_playerY + 1][_playerX].objStatic == WALL)
            playerCanMove = 0;

        if(boxCanPush)
            if(newlvl[_playerY + 2][_playerX].objDynamic == BOX ||
               newlvl[_playerY + 2][_playerX].objStatic == WALL )
               playerCanMove = 0;

        if(playerCanMove)
            deltaY = 1;
        break;

    case RIGHT:
        if(newlvl[_playerY][_playerX + 1].objDynamic == BOX)
            boxCanPush = 1;

        if(newlvl[_playerY][_playerX + 1].objStatic == WALL)
            playerCanMove = 0;

        if(boxCanPush)
            if(newlvl[_playerY][_playerX + 2].objDynamic == BOX ||
               newlvl[_playerY][_playerX + 2].objStatic == WALL )
               playerCanMove = 0;

        if(playerCanMove)
            deltaX = 1;
        break;
    }

    if(playerCanMove)
    {
        newlvl[_playerY][_playerX].objDynamic = NONE;    
        _playerX += deltaX;
        _playerY += deltaY;
        newlvl[_playerY][_playerX].objDynamic = PLAYER;
        
        if(boxCanPush)
            newlvl[_playerY + deltaY][_playerX + deltaX].objDynamic = BOX;
    }

    if(isDejaVu(newlvl, root))
        return NULL;
    else    
        return createState(newlvl, _playerX,_playerY);
}

/*  Je considère un etat lock si il y a une case 
    qui est dans un coin mais pas sur un goal

    Ou si la caisse est sur une ligne avec des murs 
    et qu'il ny a pas de goal sur la ligne

    ########        ########
    #-----$#        #--$---#
    #-@--.-#        #-@--.-#
    #------#        #------#
    ########        ########
*/
int isStateLock(state* state) // 2 tab
{
    int i,j;
    for (i = 0; i < BOARD_MAX_SIZE_Y; i++)
    {
        for (j = 0; j < BOARD_MAX_SIZE_X; j++)
        {
            if(state->lvl[i][j].objDynamic == BOX && 
               state->lvl[i][j].objStatic  == FLOOR)
            {
                if(state->lvl[i-1][j].objStatic == WALL &&
                   state->lvl[i][j+1].objStatic == WALL)
                    return 1;

                if(state->lvl[i][j+1].objStatic == WALL &&
                   state->lvl[i+1][j].objStatic == WALL)
                    return 1;

                if(state->lvl[i+1][j].objStatic == WALL &&
                   state->lvl[i][j-1].objStatic == WALL)
                    return 1;

                if(state->lvl[i][j-1].objStatic == WALL &&
                   state->lvl[i-1][j].objStatic == WALL)
                    return 1;
            }
        }
    }
    return 0;
}

int isStateLockUpgrade(state* state)// 3 tab
{
    int i,j;
    int deadEndFinded = 0;

    for (i = 0; i < BOARD_MAX_SIZE_Y; i++)
    {
        for (j = 0; j < BOARD_MAX_SIZE_X; j++)
        {
            if(state->lvl[i][j].objDynamic == BOX && 
               state->lvl[i][j].objStatic  == FLOOR)
            {
                //OLD
                if(state->lvl[i-1][j].objStatic == WALL &&
                   state->lvl[i][j+1].objStatic == WALL)
                    return 1;

                if(state->lvl[i][j+1].objStatic == WALL &&
                   state->lvl[i+1][j].objStatic == WALL)
                    return 1;

                if(state->lvl[i+1][j].objStatic == WALL &&
                   state->lvl[i][j-1].objStatic == WALL)
                    return 1;

                if(state->lvl[i][j-1].objStatic == WALL &&
                   state->lvl[i-1][j].objStatic == WALL)
                    return 1;

                if(state->lvl[i][j-1].objStatic == WALL)
                {
                    deadEndFinded += findDeadEnd(state,i,j,UP,LEFT);
                    deadEndFinded += findDeadEnd(state,i,j,DOWN,LEFT);
                }

                if(state->lvl[i][j+1].objStatic == WALL)
                {
                    deadEndFinded += findDeadEnd(state,i,j,UP,RIGHT);
                    deadEndFinded += findDeadEnd(state,i,j,DOWN,RIGHT);
                }

                if(state->lvl[i-1][j].objStatic == WALL)
                {
                    deadEndFinded += findDeadEnd(state,i,j,RIGHT,UP);
                    deadEndFinded += findDeadEnd(state,i,j,LEFT,UP);
                }

                if(state->lvl[i+1][j].objStatic == WALL)
                {
                    deadEndFinded += findDeadEnd(state,i,j,RIGHT,DOWN);
                    deadEndFinded += findDeadEnd(state,i,j,LEFT,DOWN);
                }

                if(deadEndFinded > 0)
                    return 1;
            }
        }
    }
    return 0;
}



int findDeadEnd(state* state, int i, int j, enum Dir dirrection, enum Dir adjWall)//4
{
    switch (adjWall)
    {
    case UP:
        if(state->lvl[i-1][j].objStatic != WALL)
            return -1;

    case DOWN:
        if(state->lvl[i+1][j].objStatic != WALL)
            return -1;

    case LEFT:
        if(state->lvl[i][j-1].objStatic != WALL)
            return -1;

    case RIGHT:
        if(state->lvl[i][j+1].objStatic != WALL)
            return -1;
    }

    switch (state->lvl[i][j].objStatic)
    {
    case WALL:
        return 1;
    case GOAL:
        return -1;
    case FLOOR:
        switch (dirrection)
        {
        case UP:
            return findDeadEnd(state,i-1,j,UP,adjWall);
        case DOWN:
            return findDeadEnd(state,i+1,j,DOWN,adjWall);
        case LEFT:
            return findDeadEnd(state,i,j+1,LEFT,adjWall);
        case RIGHT:
            return findDeadEnd(state,i,j-1,RIGHT,adjWall);
        }
        break;
    }

    printf("ERROR in findDeadEnd()");
    return 0;
}


int forceBrute(state* currentState, state* root, char opti) //1
{
    int i;
    enum Dir dir[4] = {UP, DOWN, LEFT, RIGHT};
    switch (opti)
    {
    case 'a':
        if(isStateLock(currentState))
            return 0;
        break;
    case 'b':
        if(isStateLockUpgrade(currentState))
            return 0;
        break;
    }
    if(isLvlFinish(currentState->lvl))
        return 1;
    for (i = 0; i < 4; i++)
    {
        if(isMoveChange(dir[i], currentState))
        {
            currentState->suc[i] = movePlayer_ia(dir[i], currentState, root);
            
            if(currentState->suc[i] != NULL)
            {
                if(forceBrute(currentState->suc[i], root, opti))
                {
                    currentState->suc[i]->flag = 1;
                    return 1;
                }
            }
        }
    }
    return 0;
}

int countSolve(state* state)
{
    int i;
    for (i = 0; i < 4; i++)
        if(state->suc[i] != NULL && state->suc[i]->flag == 1)
            return countSolve(state->suc[i]) + 1;

    return 0;
}

void drawSolve(state* state, int count, float time)
{
    int i;
    usleep(50000);
    draw(state->lvl);
    printf("COUPS: %d\tTEMPS: %.2fsec", count,time/1000000);
    fflush(stdout);
    for (i = 0; i < 4; i++)
        if(state->suc[i] != NULL && state->suc[i]->flag == 1)
            drawSolve(state->suc[i], count,time);
}


// LISTE
    // liste_movement*  new_movement(enum Dir dir)
    // {
    //     liste_movement* new_move = malloc(sizeof(liste_movement));
    //     new_move->dir = dir;
    //     new_move->prev = NULL;
    //     new_move->suc = NULL;
    //     return new_move;
    // }


    // liste_movement*  add_movement(liste_movement* tail ,enum Dir dir)
    // {
    //     liste_movement* new_move = new_movement(dir);

    //     tail->suc = new_move;
    //     new_move->prev = tail;

    //     return new_move;

    // }

