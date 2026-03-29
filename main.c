#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#ifdef _WIN32
#include <ncurses/ncurses.h>
#else
#include <ncurses.h>
#endif
int rows = 0;
int cols = 0;
int rotState = 0;
int blockNum = 0;

void resetGameState(int **GameState){
    int i , j;
for (i = 0; i < cols; i++) {
    GameState[i] = calloc(rows,sizeof(int));
}
return;
}

void freeGameState(int **GameState){
    int i , j;
for (i = 0; i < cols; i++) {
    free(GameState[i]);
}
return;
}
long uniTime(){
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC,&t);
    long millisec = t.tv_sec*1000;
    long milli = t.tv_nsec/1e+6;
    return millisec + milli;
}

void printGameState(WINDOW *pTetrisWin,int **GameState){
    werase(pTetrisWin);
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
        // *2 for proper square
        wmove(pTetrisWin,i+1,j*2+1);
        if(GameState[i][j]==0){
            wprintw(pTetrisWin,"[]");
        }
        }
    }
    box(pTetrisWin,0,0);
    wrefresh(pTetrisWin);
}

int main(){
    char *UTF;
    setlocale(LC_CTYPE,"");
    char *textMode = setlocale(LC_CTYPE,NULL);
    if(strstr(textMode,".UTF-8")||strstr(textMode,".utf8")){
        UTF = textMode;
    }else{
    UTF = setlocale(LC_CTYPE,".UTF-8");
    }

    initscr();
    start_color();
    printw("how mant rows\n");
    scanw(" %d",&rows);
    printw("how mant columns\n");
    scanw(" %d",&cols);
    printw("rows %d, cols %d",rows,cols);
    if(rows < 4||cols<4){
    rows=10;
    cols=16;
    printw("to few rows/columns\nsetting default\n");
    }
    int **GameState=malloc(cols * sizeof(int*));
    resetGameState(GameState);
    erase();
    refresh();
    nodelay(stdscr,true);
    noecho();
    keypad(stdscr,1);
    // *2 so blocks look square
    WINDOW *pTetrisWin = newwin(cols+2,rows*2+2,0,0);
    printGameState(pTetrisWin,GameState);
    move(cols+2,0);
    long oldTime = uniTime();
    long currentTime = uniTime();
    int num=1;
    int tickRate=500;
    while(true){
        currentTime = uniTime();

        if(currentTime - oldTime >= tickRate){
            num++;
            oldTime=currentTime;
            printGameState(pTetrisWin,GameState);
            move(cols+2,0);
            printw("%d\n",num);
            refresh();
        }

        int input = getch();
        move(cols+2,0);
        if(input==ERR){
            continue;
        }
        switch(input){
            case 'a':
            break;
            case KEY_LEFT:
            break;
            case 'd':
            break;
            case KEY_RIGHT:
            break;
            case 's':
            break;
            case KEY_DOWN:
            break;
            case 'w':
            break;
            case KEY_UP:
            break;
            case 27:
            goto GameOver;
            break;
        }
    }
    GameOver:
    nodelay(stdscr,false);
    //erase();
    mvprintw(cols+3,0,"\nPress any key to exit \n");
    refresh();
    freeGameState(GameState);
    free(GameState);
    getch();
    endwin();
}