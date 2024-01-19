#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include "sokoban.h"
#include "ia.h"

int main(int argc, char* argv[])
{
    if(argv[1] == NULL || argv[2] == NULL)
    {
        printf("ERROR : commande invalide\n./sokoban <chemin_du_lvl> <optimisation>\n");
        return 0;
    }

    setUpShell();
    int x,y,count;
    clock_t td, ta;
    state* s;
    cell** lvl;

    lvl = loadLevel(argv[1]);
    findPlayer(&x,&y,lvl);
    s = createState(lvl, x, y);
    
    draw(s->lvl);
    td = clock();
    forceBrute(s,s,argv[2][0]);
    ta = clock();
    count = countSolve(s);
    drawSolve(s,count, (int)ta-td);
    
    // update(lvl);
    return 0;
}
