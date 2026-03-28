#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <ncurses/ncurses.h>
#else
#include <ncurses.h>
#endif
#include <stdbool.h>
int rows = 0;
int cols = 0;
int rotState = 0;
int blockNum = 0;
int main(){
initscr();
start_color();
WINDOW *pTetrisWin;
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
scanw(" %c");
nodelay(stdscr,true);
endwin();
//endwin();
}