#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#ifdef _WIN32
#include <ncurses/ncurses.h>
#else
//#include <ncurses.h>
#include <ncursesw/ncurses.h>
#endif
int rows = 0;
int cols = 0;
int rotState = 0;
int currentBlock = 0;
int nextBlock = 0;
char *UTF;
typedef struct Cord
{
int y;
int x;
}Cord;

enum Blocks{
    Block = 1,
    Line,
    Z,
    S,
    L,
    J,
    T,
};

int spawnBlock(int **GameState,Cord *CordArray,int num);
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
    char *blockChar = "██";
    if(UTF==NULL){
    strcpy(blockChar,"[]");
    }
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
        // *2 for proper square
        wmove(pTetrisWin,i+1,j*2+1);
        if(GameState[i][j]==0){
            wprintw(pTetrisWin," .");
        }
        if(GameState[i][j]==1){
            wprintw(pTetrisWin,"%s",blockChar);
        }
        }
    }
    box(pTetrisWin,0,0);
    wrefresh(pTetrisWin);
}
void appendCordArray(int **GameState,struct Cord *CordArray,struct Cord *newCordArray){
        for(int i=0; i<4;i++){
        if(newCordArray[i].y>=cols||newCordArray[i].x>=rows){
            //printf("too far up/right");
            return;
        }
        if(newCordArray[i].x<0||newCordArray[i].y<0){
            //printf("too far left");
            return;
        }
        if(GameState[newCordArray[i].y][newCordArray[i].x]>=2){
            //printf("colision");
            return;
        }
    }
        for(int i=0; i<4;i++){
            GameState[CordArray[i].y][CordArray[i].x]=0;
        }
        for(int i=0; i<4;i++){
            GameState[newCordArray[i].y][newCordArray[i].x]=1;
        }
}

int main(){
     
    setlocale(LC_ALL,"");
    char *textMode = setlocale(LC_ALL,NULL);
    if(strstr(textMode,".UTF-8")||strstr(textMode,".utf8")){
        UTF = textMode;
    }else{
    UTF = setlocale(LC_ALL,".UTF-8");
    }
    srand(time(NULL));
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
    struct Cord CordArray[4];
    for(int i = 0;i<4;i++){
        CordArray[i].y=0;
        CordArray[i].x=0;
    }
    spawnBlock(GameState,CordArray,0);
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

int spawnBlock(int **GameState,struct Cord *CordArray,int block){
    int checkFlag=0;
    int rNum;
    int flag=0;
    if(block==0){
    rotState = 0;
     if(nextBlock==0){
     rNum = (rand() % (7 - 1 + 1)) + 1;
     nextBlock = (rand() % (7 - 1 + 1)) + 1;
     flag=1;
     }
     else {
        rNum = nextBlock;
        nextBlock = (rand() % (7 - 1 + 1)) + 1;
     }
        if(CordArray[0].y <= 1){
           checkFlag = 1;
        }else{
           checkFlag = 0;
        }
    
    }else{
        rNum = block;
    }

    //rNum=Block;//!REMEMBER ME

    switch (rNum)
    {
    case Block://block
    Cord oldCordArray[4];
    memcpy(oldCordArray,CordArray,sizeof(Cord)*4);
    CordArray[0].y = 0;
    CordArray[0].x = rows/2;

    CordArray[1].y = 0;
    CordArray[1].x = rows/2+1;

    CordArray[2].y = 1;
    CordArray[2].x = rows/2;

    CordArray[3].y = 1;
    CordArray[3].x = rows/2+1;
    appendCordArray(GameState,oldCordArray,CordArray);
    /*
    GameState[0][rows/2] = 1;
    GameState[0][rows/2+1]=1;
    GameState[1][rows/2] = 1;
    GameState[1][rows/2+1]=1;
    */
    break;
    case Line://line
    memcpy(oldCordArray,CordArray,sizeof(Cord)*4);
    CordArray[0].y = 0;
    CordArray[0].x = rows/2-1;

    CordArray[1].y = 0;
    CordArray[1].x = rows/2;

    CordArray[2].y = 0;
    CordArray[2].x = rows/2+1;

    CordArray[3].y = 0;
    CordArray[3].x = rows/2+2;
    appendCordArray(GameState,oldCordArray,CordArray);
/*
    GameState[0][rows/2-1] = 1;
    GameState[0][rows/2]=1;
    GameState[0][rows/2+1] = 1;
    GameState[0][rows/2+2]=1;
*/
    break;
    case Z://z
    memcpy(oldCordArray,CordArray,sizeof(Cord)*4);
    CordArray[0].y = 0;
    CordArray[0].x = rows/2-1;

    CordArray[1].y = 0;
    CordArray[1].x = rows/2;

    CordArray[2].y = 1;
    CordArray[2].x = rows/2;

    CordArray[3].y = 1;
    CordArray[3].x = rows/2+1;
    appendCordArray(GameState,oldCordArray,CordArray);
/*
    GameState[0][rows/2-1] = 1;
    GameState[0][rows/2]=1;
    GameState[1][rows/2] = 1;
    GameState[1][rows/2+1]=1;
*/
    break;
    case S://s
    memcpy(oldCordArray,CordArray,sizeof(Cord)*4);
    CordArray[0].y = 0;
    CordArray[0].x = rows/2;

    CordArray[1].y = 0;
    CordArray[1].x = rows/2+1;

    CordArray[2].y = 1;
    CordArray[2].x = rows/2;

    CordArray[3].y = 1;
    CordArray[3].x = rows/2-1;
    appendCordArray(GameState,oldCordArray,CordArray);
    /*
    GameState[0][rows/2] = 1;
    GameState[0][rows/2+1]=1;
    GameState[1][rows/2] = 1;
    GameState[1][rows/2-1]=1;
    */

    break;
    case L://L
    memcpy(oldCordArray,CordArray,sizeof(Cord)*4);
    CordArray[0].y = 0;
    CordArray[0].x = rows/2-1;

    CordArray[1].y = 1;
    CordArray[1].x = rows/2-1;

    CordArray[2].y = 1;
    CordArray[2].x = rows/2;

    CordArray[3].y = 1;
    CordArray[3].x = rows/2+1;
    appendCordArray(GameState,oldCordArray,CordArray);
    /*
    GameState[0][rows/2-1]=1;
    GameState[1][rows/2-1] = 1;
    GameState[1][rows/2] = 1;
    GameState[1][rows/2+1]=1;
    */

    break;
    case J://j
    memcpy(oldCordArray,CordArray,sizeof(Cord)*4);
    CordArray[0].y = 0;
    CordArray[0].x = rows/2+1;

    CordArray[1].y = 1;
    CordArray[1].x = rows/2+1;

    CordArray[2].y = 1;
    CordArray[2].x = rows/2;

    CordArray[3].y = 1;
    CordArray[3].x = rows/2-1;
    appendCordArray(GameState,oldCordArray,CordArray);
    /*
    GameState[0][rows/2+1]=1;
    GameState[1][rows/2+1] = 1;
    GameState[1][rows/2] = 1;
    GameState[1][rows/2-1]=1;
    */

    break;
    case T://T
    memcpy(oldCordArray,CordArray,sizeof(Cord)*4);
    CordArray[0].y = 0;
    CordArray[0].x = rows/2;

    CordArray[1].y = 1;
    CordArray[1].x = rows/2;

    CordArray[2].y = 1;
    CordArray[2].x = rows/2-1;

    CordArray[3].y = 1;
    CordArray[3].x = rows/2+1;
    appendCordArray(GameState,oldCordArray,CordArray);
    /*
    GameState[0][rows/2] = 1;
    GameState[1][rows/2]=1;
    GameState[1][rows/2-1] = 1;
    GameState[1][rows/2+1]=1;
    */

    break;
    default:
    printf("rNum is broken %d",rNum);
        break;
    }
    return rNum;
}