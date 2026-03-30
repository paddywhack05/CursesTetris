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
int color=2;
int nextColor=2;
int score=0;
int totalLinesCleared=0;
char *UTF;
typedef struct Cord
{
int y;
int x;
}Cord;
enum Colors{
    Default=1,
    Red,
    Green,
    Blue,
    Yellow,
    Magenta,
};
enum Blocks{
    Block = 1,
    Line,
    Z,
    S,
    L,
    J,
    T,
};
enum Direction{
    Right,
    Left,
};
int spawnBlock(int **GameState,Cord *CordArray,int num);
int appendCordArray(int **GameState,struct Cord *CordArray,struct Cord *newCordArray);
void incrementRot(){
    (rotState < 3) ? (rotState++) : (rotState=0);
}
void decrementRot(){
    (rotState==0) ? (rotState=3) : (rotState--);
}
void givePoints(int linesCleared){
    totalLinesCleared += linesCleared;
    switch (linesCleared)
    {
    case 1:
        score += 100;
        break;
    case 2:
        score += 300;
        break;
    case 3:
        score += 500;
        break;
    case 4:
        score += 1000;
        break;
    }
}
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
void printCords(Cord *CordArray,int pad){
    move(cols+pad,0);
    for(int i=0;i<4;i++){
        printw("x=%d,y=%d, ",CordArray[i].x,CordArray[i].y);
    }
}
void printFixed(int arr[4][4])
{
    move(cols+5,0);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            printw("%d ", arr[i][j]);
        printf("\n");
    }
    refresh();
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
            wattron(pTetrisWin,COLOR_PAIR(color));
            wprintw(pTetrisWin,"%s",blockChar);
            wattron(pTetrisWin,COLOR_PAIR(1));
        }
        if(GameState[i][j]>=2){
            wattron(pTetrisWin,COLOR_PAIR(GameState[i][j]));
            wprintw(pTetrisWin,"%s",blockChar);
            wattron(pTetrisWin,COLOR_PAIR(1));
        }
        }
    }
    box(pTetrisWin,0,0);
    wrefresh(pTetrisWin);
    //nextBlock window
    int realRows=rows;
    int realCols=cols;
    WINDOW *pNextBlockWin=newwin(5,5*2,2,rows*2+3);
    int **box = malloc(sizeof(int*)*5);
    Cord boxCords[4];
        for(int i = 0;i<4;i++){
        boxCords[i].y=0;
        boxCords[i].x=0;
    }
    cols=4;
    rows=4;
    resetGameState(box);
    Cord oldBoxCords[4];
    memcpy(oldBoxCords,boxCords,sizeof(Cord)*4);
    if(nextBlock==Line){
        for(int i = 0;i<4;i++){
        boxCords[i].y=0;
        boxCords[i].x=i;
        }
        appendCordArray(box,oldBoxCords,boxCords);
    }
    else{
        rows=3;
        spawnBlock(box,boxCords,nextBlock);
    }
    for(int i = 0;i<4;i++){
        for(int j=0;j<4;j++){
            wmove(pNextBlockWin,i+1,j*2+1);
        if(box[i][j]==0){
            wprintw(pNextBlockWin,"  ");
        }
        if(box[i][j]==1){
            wattron(pNextBlockWin,COLOR_PAIR(nextColor));
            wprintw(pNextBlockWin,"%s",blockChar);
            wattron(pNextBlockWin,COLOR_PAIR(1));
        }
        }
    }
    box(pNextBlockWin,0,0);
    wrefresh(pNextBlockWin);
    rows = realRows;
    cols=realCols;

    WINDOW *pScoreWin=newwin(2,5*2,0,rows*2+3);
    wprintw(pScoreWin,"Score:");
    mvwprintw(pScoreWin,1,0,"%d",score);
    wrefresh(pScoreWin);
}
int appendCordArray(int **GameState,struct Cord *CordArray,struct Cord *newCordArray){
        for(int i=0; i<4;i++){
        if(newCordArray[i].y>=cols||newCordArray[i].x>=rows){
            //printf("too far up/right");
            return -1;
        }
        if(newCordArray[i].x<0||newCordArray[i].y<0){
            //printf("too far left");
            return -1;
        }
        if(GameState[newCordArray[i].y][newCordArray[i].x]>=2){
            //printf("colision");
            return -1;
        }
    }
        for(int i=0; i<4;i++){
            if(GameState[CordArray[i].y][CordArray[i].x]==1){
            GameState[CordArray[i].y][CordArray[i].x]=0;
            }
        }
        for(int i=0; i<4;i++){
            GameState[newCordArray[i].y][newCordArray[i].x]=1;
        }
}

void groundGravity(int **GameState,int *listToClear,int index){

    for (int i = index-1;i>=0;i--){
        for(int j = listToClear[i];j>1;j--){
            for(int a = 0;a<rows;a++){
                if(GameState[j-1][a]>=2){
                    int oldColour=GameState[j-1][a];
                     GameState[j-1][a]=0;
                     GameState[j][a]=oldColour;
                }
            }
        }
    }

}

void clearLines(int **GameState,int *listToClear,int index){
    int i,j;
    for(i=0;i<index;i++){
  for(j=0; j<rows;j++){
        GameState[listToClear[i]][j]=0;
    }
    }
    groundGravity(GameState, listToClear, index);
    return;
}

void checkLines(int **GameState){
    int linesToClear[4];
    int anotherIndex=0;
    int i , j;
for (i = cols-1; i > 0; i--) {
        int num = rows;
  for (j = 0; j < rows; j++) {
        if(!(GameState[i][j]>=2)){
            break;
        }else{
            num--;
        }
        }
         if(num==0){
            linesToClear[anotherIndex]=i;
            anotherIndex++;
        }
        if(num == 0){
        }
    }
    
    if(anotherIndex > 0){
        clearLines(GameState,linesToClear,anotherIndex);
        givePoints(anotherIndex);
    }
    return;
}

int setGround(int **GameState,Cord *CordArray){
    for(int i=0; i<4;i++){
        GameState[CordArray[i].y][CordArray[i].x]=color;
    }
    checkLines(GameState);
   int block = spawnBlock(GameState,CordArray,0);
    return block;
}
int applyPadding(Cord *newCordArray,Cord *CordArray,int **GameState){
        printCords(newCordArray,3);
            int padding=0;
            int paddingY=0;
            if(currentBlock!=Line){
                padding=0;
                paddingY =0;
                padding = (rotState==0) ? 0 : padding;
                padding = (rotState==1) ? -1 : padding;
                paddingY = (rotState ==2) ? -1 : paddingY;
            }else{
                padding = 0;
                paddingY =0;
            }
            mvprintw(cols+2,0,"Apply the padding");
            refresh();
        for(int i=0; i<4;i++){
        if(newCordArray[i].y+paddingY>=cols||newCordArray[i].x+padding>=rows){
            mvprintw(cols+2,0,"too far up/right");
            return -1;
        }
        if(newCordArray[i].x+padding<0||newCordArray[i].y+paddingY<0){
            mvprintw(cols+2,0,"too far Left");
            //printf("too far left");
            return -1;
        }
        if(GameState[newCordArray[i].y+paddingY][newCordArray[i].x+padding]>=2){
            mvprintw(cols+2,0,"Collision");
            return -1;
        }
    }
    for(int i=0; i<4;i++){
        newCordArray[i].x = newCordArray[i].x+padding;
        newCordArray[i].y = newCordArray[i].y+paddingY;
    }
    //mvprintw(cols+2,0,"Success \n%d",newCordArray[0].y);
    printCords(newCordArray,4);
    refresh();
    return 0;
}
void rotateLine(int **GameState,Cord *CordArray,int direction){
        int box[4][4]={0};
        int paddingY;
        int paddingX;

        paddingX = (rotState==0) ? CordArray[0].x : paddingX;
        paddingY = (rotState==0) ? CordArray[0].y-1 : paddingY;

        paddingX = (rotState==1) ? CordArray[0].x-2 : paddingX;
        paddingY = (rotState==1) ? CordArray[0].y : paddingY;

        paddingX = (rotState==2) ? CordArray[0].x : paddingX;
        paddingY = (rotState==2) ? CordArray[0].y-2 : paddingY;

        paddingX = (rotState==3) ? CordArray[0].x-1 : paddingX;
        paddingY = (rotState==3) ? CordArray[0].y : paddingY;

        for(int i=0;i<4;i++){
           int Y = CordArray[i].y-paddingY;
           int X = CordArray[i].x-paddingX;
           box[Y][X]=1;
        }
        //Transpose
        int box2[4][4]={0};
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                box2[i][j] = box[j][i];
                //box[i][j] = 0;
            }
        }
        //Reverse
        if(direction == Right){
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                box[i][3-j] = box2[i][j];
            }
        }
        }else if(direction == Left){
            for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                box[3-i][j] = box2[i][j];
            }
        }
        }
            int num=0;
            Cord CordArray_rotated[4];
            printFixed(box);
        for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
        if(box[i][j]==1){
            CordArray_rotated[num].y=i+paddingY;
            CordArray_rotated[num].x=j+paddingX;
            num++;
            }
            }
        }
        int code = applyPadding(CordArray_rotated,CordArray,GameState);
        if(code!=ERR){
            appendCordArray(GameState,CordArray,CordArray_rotated);
            memcpy(CordArray,CordArray_rotated,sizeof(Cord)*4);
            if(direction == 0){
                incrementRot();
            }else{
                decrementRot();
            }
        }
}
void rotate(int **GameState,Cord *CordArray,int padX,int padY,int direction){
            int matrix[3][3]={0};

        //Loop through x cords set lowest to var
        int Xnum = CordArray[0].x;
        for(int i=1;i<4;i++){
            if(CordArray[i].x<Xnum){
            Xnum = CordArray[i].x;
            }else{
                continue;
            }
        }
        int Ynum = CordArray[0].y;
        for(int i=1;i<4;i++){
            if(CordArray[i].y<Ynum){
            Ynum = CordArray[i].y;
            }else{
                continue;
            }
        }
        for(int i=0;i<4;i++){
           int Y = CordArray[i].y-Ynum+padY;
           int X = CordArray[i].x-Xnum+padX;
           matrix[Y][X]=1;
        }
        //Transpose
        int matrix2[3][3]={0};

        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                matrix2[i][j] = matrix[j][i];
            }
        }
        //Reverse

            for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
               // matrix[i][j] = matrix2[2-i][2-j];
               matrix[i][j] = matrix2[i][j];
            }
        }
        if(direction==Left){
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                matrix[i][j] = matrix2[2-i][j];
            }
        }
        }else if(direction==Right){
            for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                matrix[i][j] = matrix2[i][2-j];
            }
        }
        }
            int index=0;
            Cord CordArray_rotated[4];
        for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
        if(matrix[i][j]==1){
            CordArray_rotated[index].y=i+Ynum;
            CordArray_rotated[index].x=j+Xnum;
            index++;
            }
            }
        
        }
        int code = applyPadding(CordArray_rotated,CordArray,GameState);
        if(code!=ERR){
            appendCordArray(GameState,CordArray,CordArray_rotated);
            memcpy(CordArray,CordArray_rotated,sizeof(Cord)*4);
            if(direction == 0){
                incrementRot();
            }else{
                decrementRot();
            }
        }
}

void rotateRight(int **GameState,Cord *CordArray){
        int padY=0;
        int padX=0;
        padX = (rotState == 1) ? 1 : padX;
        padY = (rotState == 2) ? 1 : padY;
    switch (currentBlock)
    {
       case Block://block
        return;
        break;
        case Line://line
           rotateLine(GameState,CordArray,Right);
        break;
        default:
            rotate(GameState,CordArray,padX,padY,Right);
        break;
    }
}
void rotateLeft(int **GameState,Cord *CordArray){
        int padY=0;
        int padX=0;
        padX = (rotState == 1) ? 1 : padX;
        padY = (rotState == 2) ? 1 : padY;
    switch (currentBlock)
    {
       case Block://block
        return;
        break;
        case Line://line
           rotateLine(GameState,CordArray,Left);
        break;
        default:
            rotate(GameState,CordArray,padX,padY,Left);
        break;
    }
}

void moveRight(int **GameState,Cord *CordArray){
    for(int i=3; i>=0;i--){
    if(CordArray[i].x+1 > rows-1){
        return;
    }
    if(GameState[CordArray[i].y][CordArray[i].x+1] >= 2){
        return;
    }
    }
        Cord oldCordArray[4];
        memcpy(oldCordArray,CordArray,sizeof(Cord)*4);
           for(int i=3; i>=0;i--){
            CordArray[i].x=CordArray[i].x+1;
        }
        appendCordArray(GameState,oldCordArray,CordArray);
                return;
}

void moveLeft(int **GameState,Cord *CordArray){
    for(int i=3; i>=0;i--){
    if(CordArray[i].x-1 < 0){
        return;
    }
    if(GameState[CordArray[i].y][CordArray[i].x-1] >= 2){
        return;
    }
    }
        Cord oldCordArray[4];
        memcpy(oldCordArray,CordArray,sizeof(Cord)*4);
           for(int i=3; i>=0;i--){
            CordArray[i].x=CordArray[i].x-1;
        }
        appendCordArray(GameState,oldCordArray,CordArray);
                return;
}

int advanceState(int **GameState,Cord *CordArray){
    for(int i=3; i>=0;i--){
    if(CordArray[i].y+1 > cols-1){
        int block = setGround(GameState,CordArray);
        return block;
    }
    if(GameState[CordArray[i].y+1][CordArray[i].x] >= 2){
        int block = setGround(GameState,CordArray);
        return block;
    }
    }
        Cord oldCordArray[4];
        memcpy(oldCordArray,CordArray,sizeof(Cord)*4);
           for(int i=3; i>=0;i--){
            CordArray[i].y=CordArray[i].y+1;
        }
        appendCordArray(GameState,oldCordArray,CordArray);
                return 0;
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
    init_pair(Default, COLOR_WHITE, COLOR_BLACK);
    init_pair(Red, COLOR_RED, COLOR_BLACK);
    init_pair(Green, COLOR_GREEN, COLOR_BLACK);
    init_pair(Blue, COLOR_BLUE, COLOR_BLACK);
    init_pair(Yellow, COLOR_YELLOW, COLOR_BLACK);
    init_pair(Magenta, COLOR_MAGENTA, COLOR_BLACK);
    //init_pair(3, COLOR_BLACK, COLOR_GREEN);
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
            int code = advanceState(GameState,CordArray);
            if(code ==ERR){
                goto GameOver;
            }
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

            case KEY_LEFT:
            moveLeft(GameState,CordArray);
            break;
            case 'd':

            case KEY_RIGHT:
            moveRight(GameState,CordArray);
            break;
            case 's':

            case KEY_DOWN:
            int code = advanceState(GameState,CordArray);
            if(code ==ERR){
                goto GameOver;
            }
            break;
            case 'w':

            case KEY_UP:

            case 'e':
            rotateRight(GameState,CordArray);
            break;
            case 'q':
            rotateLeft(GameState,CordArray);
            break;
            case 27:
            goto GameOver;
            break;
        }
        printGameState(pTetrisWin,GameState);
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

int spawnBlock(int **GameState,Cord *CordArray,int block){
    int checkFlag=0;
    int rNum;
    int flag=0;
    if(block==0){
        rotState = 0;
        if(nextBlock==0){
        rNum = (rand() % (7 - 1 + 1)) + 1;
        nextBlock = (rand() % (7 - 1 + 1)) + 1;
        color=(rand() % (5-1+1))+2;
        nextColor=(rand() % (5-1+1))+2;
        flag=1;
        }
        else{
            rNum = nextBlock;
            nextBlock = (rand() % (7 - 1 + 1)) + 1;
            color=nextColor;
            nextColor=(rand() % (5-1+1))+2;
        }
        if(CordArray[0].y <= 1){
           checkFlag = 1;
        }else{
           checkFlag = 0;
        }
        currentBlock = rNum;
    }else{
        rNum = block;
    }

    //rNum=Line;//!REMEMBER ME
    //color=(rand() % (5-1+1))+2;
    Cord oldCordArray[4];
    memcpy(oldCordArray,CordArray,sizeof(Cord)*4);
    switch (rNum)
    {
    case Block://block
    CordArray[0].y = 0;
    CordArray[0].x = rows/2;

    CordArray[1].y = 0;
    CordArray[1].x = rows/2+1;

    CordArray[2].y = 1;
    CordArray[2].x = rows/2;

    CordArray[3].y = 1;
    CordArray[3].x = rows/2+1;
    break;
    case Line://line
    CordArray[0].y = 0;
    CordArray[0].x = rows/2-1;

    CordArray[1].y = 0;
    CordArray[1].x = rows/2;

    CordArray[2].y = 0;
    CordArray[2].x = rows/2+1;

    CordArray[3].y = 0;
    CordArray[3].x = rows/2+2;
    break;
    case Z://z
    CordArray[0].y = 0;
    CordArray[0].x = rows/2-1;

    CordArray[1].y = 0;
    CordArray[1].x = rows/2;

    CordArray[2].y = 1;
    CordArray[2].x = rows/2;

    CordArray[3].y = 1;
    CordArray[3].x = rows/2+1;
    break;
    case S://s
    CordArray[0].y = 0;
    CordArray[0].x = rows/2;

    CordArray[1].y = 0;
    CordArray[1].x = rows/2+1;

    CordArray[2].y = 1;
    CordArray[2].x = rows/2;

    CordArray[3].y = 1;
    CordArray[3].x = rows/2-1;
    break;
    case L://L
    CordArray[0].y = 0;
    CordArray[0].x = rows/2-1;

    CordArray[1].y = 1;
    CordArray[1].x = rows/2-1;

    CordArray[2].y = 1;
    CordArray[2].x = rows/2;

    CordArray[3].y = 1;
    CordArray[3].x = rows/2+1;
    break;
    case J://j
    CordArray[0].y = 0;
    CordArray[0].x = rows/2+1;

    CordArray[1].y = 1;
    CordArray[1].x = rows/2+1;

    CordArray[2].y = 1;
    CordArray[2].x = rows/2;

    CordArray[3].y = 1;
    CordArray[3].x = rows/2-1;
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
    break;
    default:
    printf("rNum is broken %d",rNum);
        break;
    }
    int code = appendCordArray(GameState,oldCordArray,CordArray);
    if(code == ERR){
        return ERR;
    }
    return rNum;
}