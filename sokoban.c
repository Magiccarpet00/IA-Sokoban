#include <string.h>
#include "sokoban.h"
#include "ia.h"


static int BOARD_OFFSET = 2; 
// static cell tab[BOARD_MAX_SIZE_Y][BOARD_MAX_SIZE_X];
// static int playerX,playerY;

int setUpShell()
{
  struct termios term, term_orig;

  if(tcgetattr(0, &term_orig)) {
    printf("tcgetattr failed\n");
    return -1;
  }

  term = term_orig;

  term.c_lflag &= ~ICANON;
  //term.c_lflag &= ~ECHO;
  //term.c_cc[VMIN] = 0;
  //term.c_cc[VTIME] = 0;

  if (tcsetattr(0, TCSANOW, &term)) {
    printf("tcsetattr failed\n");
    return -1;
  }

  return 1;
}

void draw(cell** lvl)
{
    char charToDraw[12];
    printf("\033[2J");
    for (int i = 0; i < BOARD_MAX_SIZE_Y; i++)
    {
        for (int j = 0; j < BOARD_MAX_SIZE_X; j++)
        {
            printf("\033[%d;%dH",i+BOARD_OFFSET,j+BOARD_OFFSET);
            switch (lvl[i][j].objDynamic){
            case NONE: switch (lvl[i][j].objStatic){
                       case FLOOR: strcpy(charToDraw, "\033[39;49m "); break;
                       case WALL:  strcpy(charToDraw, "\033[32;42m "); break;
                       case GOAL:  strcpy(charToDraw, "\033[91;49m."); break;
                       }break;
    
            case BOX: switch (lvl[i][j].objStatic){
                      case FLOOR: strcpy(charToDraw, "\033[37;100m#"); break;
                      case GOAL:  strcpy(charToDraw, "\033[91;100m#"); break;
                      case WALL:  strcpy(charToDraw, "?"); break; //Cas impossible
                      }break;

            case PLAYER: strcpy(charToDraw, "\033[35;49m@"); break;
            }
            printf("%s",charToDraw);
        }
    }
    fflush(stdout);
}

// cell** movePlayer(enum Dir dirrection, cell** lvl)
// {
    // int playerCanMove = 1;
    // int boxCanPush = 0;
    // int deltaX = 0;
    // int deltaY = 0;

    // cell** newlvl = copyLevel(lvl);

    // switch (dirrection)
    // {
    // case UP:    
    //     if(newlvl[*playerY - 1][*playerX].objDynamic == BOX)
    //         boxCanPush = 1;

    //     if(newlvl[playerY - 1][playerX].objStatic == WALL)
    //         playerCanMove = 0;

    //     if(boxCanPush)
    //         if(newlvl[playerY - 2][playerX].objDynamic == BOX ||
    //            newlvl[playerY - 2][playerX].objStatic == WALL )
    //            playerCanMove = 0;

    //     if(playerCanMove)
    //         deltaY = -1;
    //     break;

    // case LEFT:
    //     if(newlvl[playerY][playerX-1].objDynamic == BOX)
    //         boxCanPush = 1;

    //     if(newlvl[playerY][playerX-1].objStatic == WALL)
    //         playerCanMove = 0;

    //     if(boxCanPush)
    //         if(newlvl[playerY][playerX - 2].objDynamic == BOX ||
    //            newlvl[playerY][playerX - 2].objStatic == WALL )
    //            playerCanMove = 0;

    //     if(playerCanMove)
    //         deltaX = -1;
    //     break;

    // case DOWN:
    //     if(newlvl[playerY + 1][playerX].objDynamic == BOX)
    //         boxCanPush = 1;

    //     if(newlvl[playerY + 1][playerX].objStatic == WALL)
    //         playerCanMove = 0;

    //     if(boxCanPush)
    //         if(newlvl[playerY + 2][playerX].objDynamic == BOX ||
    //            newlvl[playerY + 2][playerX].objStatic == WALL )
    //            playerCanMove = 0;

    //     if(playerCanMove)
    //         deltaY = 1;
    //     break;

    // case RIGHT: 
    //     if(newlvl[playerY][playerX + 1].objDynamic == BOX)
    //         boxCanPush = 1;

    //     if(newlvl[playerY][playerX + 1].objStatic == WALL)
    //         playerCanMove = 0;

    //     if(boxCanPush)
    //         if(newlvl[playerY][playerX + 2].objDynamic == BOX ||
    //            newlvl[playerY][playerX + 2].objStatic == WALL )
    //            playerCanMove = 0;

    //     if(playerCanMove)
    //         deltaX = 1;
    //     break;
    // }

    // if(playerCanMove)
    // {
    //     newlvl[playerY][playerX].objDynamic = NONE;    
    //     playerX += deltaX;
    //     playerY += deltaY;
    //     newlvl[playerY][playerX].objDynamic = PLAYER;
        
    //     if(boxCanPush)
    //         newlvl[playerY + deltaY][playerX + deltaX].objDynamic = BOX;
    // }

    // return newlvl;
// }

// void input(cell** lvl)
// {
//     char c;
//     read(0,&c,1);

//     switch (c)
//     {
//     case 'z': movePlayer(UP, lvl);    break;
//     case 'q': movePlayer(LEFT, lvl);  break;
//     case 's': movePlayer(DOWN, lvl);  break;
//     case 'd': movePlayer(RIGHT, lvl); break;
//     }
// }

int isLvlFinish(cell** lvl)
{
    int i,j;
    int res = 1;
    for (i = 0; i < BOARD_MAX_SIZE_Y; i++)
        for (j = 0; j < BOARD_MAX_SIZE_X; j++)
            if(lvl[i][j].objStatic == GOAL)
                if(lvl[i][j].objDynamic != BOX)
                    return 0;
    return res;
}

// void update(cell** lvl)
// {
//     while(1)
//     {
//         draw(lvl);
//         if(isLvlFinish(lvl))
//             printf("FINISH\n");
//         input(lvl);
//     }
// }

void clearLevel(cell** lvl)
{
    int i,j;
    for (i = 0; i < BOARD_MAX_SIZE_Y; i++)
    {
        for (j = 0; j < BOARD_MAX_SIZE_X; j++)
        {
            lvl[i][j].objDynamic = NONE;
            lvl[i][j].objStatic = FLOOR;
        }
    }
}

cell** createBlankLevel()
{
    cell** newlvl = (cell**)malloc(sizeof(cell*) * BOARD_MAX_SIZE_Y );
    assert(newlvl);
    for (int i = 0; i < BOARD_MAX_SIZE_Y ; i++)
    {
        newlvl[i] = (cell*)malloc(sizeof(cell)* BOARD_MAX_SIZE_X);
        assert(newlvl[i]);
    }

    clearLevel(newlvl);

    return newlvl;
}

cell** copyLevel(cell ** lvl)
{
    cell** newlvl;
    int x,y;    
    newlvl = createBlankLevel();

    for (y = 0; y < BOARD_MAX_SIZE_Y; y++)
    {
        for (x = 0; x < BOARD_MAX_SIZE_X; x++)
        {
            newlvl[y][x].objDynamic = lvl[y][x].objDynamic;
            newlvl[y][x].objStatic = lvl[y][x].objStatic;
        }
    }
    
    return newlvl;
}

cell** loadLevel(char* lvlName)
{
    FILE* ptr;
    cell** newlvl;
    char ch;
    int x = 0;
    int y = 0;

    newlvl = createBlankLevel();

    ptr = fopen(lvlName, "r");
    while (!feof(ptr)) {
        ch = fgetc(ptr);

        switch (ch)
        {
        case '\n':
            x=0;
            y++;
            break;
        case '-':
            newlvl[y][x].objStatic  = FLOOR;
            newlvl[y][x].objDynamic = NONE;
            x++;
            break;
        case '#':
            newlvl[y][x].objStatic  = WALL;
            newlvl[y][x].objDynamic = NONE;
            x++;
            break;
        case '.':
            newlvl[y][x].objStatic  = GOAL;
            newlvl[y][x].objDynamic = NONE;
            x++;
            break;
        case '@':
            newlvl[y][x].objStatic  = FLOOR;
            newlvl[y][x].objDynamic = PLAYER;
            x++;
            break;
        case '$':
            newlvl[y][x].objStatic  = FLOOR;
            newlvl[y][x].objDynamic = BOX;
            x++;
            break;
        case '%':
            newlvl[y][x].objStatic  = GOAL;
            newlvl[y][x].objDynamic = BOX;
            x++;
            break;
        }
    }
    
    fclose(ptr);

    return newlvl;
}

void findPlayer(int* _x, int* _y, cell ** lvl)
{
    int y,x;

    for (y = 0; y < BOARD_MAX_SIZE_Y; y++)
    {
        for (x = 0; x < BOARD_MAX_SIZE_X; x++)
        {
            if(lvl[y][x].objDynamic == PLAYER)
            {
                *_x = x;
                *_y = y;
            }
        }
    }
}
