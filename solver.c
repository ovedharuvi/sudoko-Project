

#include "solver.h"
typedef enum{
    EXIT = -1, FALSE = 0, TRUE = 1
}StatusType;
int generate(sudokoBoard *board , int numOfLegalValues , int numOfEmptyCells){

}

int validate(sudokoBoard *board){

}

int guess(sudokoBoard *board , int threshold){

}

int guessHint(sudokoBoard *board , int row , int column){

}

int numOfSolutions(sudokoBoard *board){

}

int checkIfValid(sudokoBoard *board , int value ,int  row ,int column){

}

int checkIfValid_ROW(sudokoBoard *board , int value , int row, int column){
int i;
for(i=0 ; i<board->boardSize ; ++i){
    if(value == board->board[row][i].value &&  i != column){
        return FALSE;
    }
}
}
int checkIfValid_COLUMN(sudokoBoard *board , int value , int column){

}
int checkIfValid_BLOCK(sudokoBoard *board , int value , int row , int column){

}
