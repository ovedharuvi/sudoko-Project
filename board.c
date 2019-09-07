

#include "board.h"
#include <stdlib.h>
#include <stdio.h>
#define BLANK_SPACE "  "
#define CELL_SPACE 4
#define FIXED 0
typedef enum{
    EXIT = -1, FALSE = 0, TRUE = 1
}StatusType;

int checkIfValid_ROW(sudokoBoard *board , int value , int row, int column);
int checkIfValid_COLUMN(sudokoBoard *board , int value ,int row, int column);
int checkIfValid_BLOCK(sudokoBoard *board , int value , int row , int column);


sudokoBoard *createBoard(int heightOfBlock , int widthOfBlock , int markErrors){
    sudokoBoard *board = (sudokoBoard*)malloc(sizeof(sudokoBoard)) ;
    int i;
    cell** tempBoard;
    int boardSize = heightOfBlock*widthOfBlock;
    board->boardSize = boardSize;
     tempBoard = (cell**)malloc(boardSize* sizeof(cell*));
    for(i=0 ; i<boardSize ; ++i)
       tempBoard[i] = (cell*)malloc(boardSize*sizeof(cell));
    board->board = tempBoard;
    board->heightOfBlock=heightOfBlock;
    board->widthOfBlock = widthOfBlock;
    board->markErrors = markErrors;
    return board;
}

void copyCell(cell *copyTo , cell *copyFrom){
    int *potentialValues = (int*)calloc(copyFrom->numOfPotentialValues , sizeof(int)) , i;
    copyTo->is_fixed = copyFrom->is_fixed;
    copyTo->numOfPotentialValues = copyFrom->numOfPotentialValues;
    copyTo->solution_value = copyFrom->solution_value;
    copyTo->value = copyFrom->value;
    for(i=0 ; i< copyFrom->numOfPotentialValues ; ++i){
        potentialValues[i] = copyFrom->potentialValues[i];
    }
    copyTo->potentialValues = potentialValues;
}


sudokoBoard *copyBoard(sudokoBoard *board) {
    int boardSize = board->widthOfBlock * board->widthOfBlock, i, j;
    sudokoBoard *newBoard = createBoard(board->heightOfBlock, board->widthOfBlock, board->markErrors);
    for (i = 0; i < boardSize; ++i) {
        for (j = 0; j < boardSize; ++j) {
            copyCell(&newBoard->board[i][j] , &board->board[i][j]);
        }
    }
    return newBoard;
}

void destroyBoard(sudokoBoard *board){
    int boardSize = board->boardSize , i;
    for(i=0 ; i<boardSize ; ++i){
        free(board->board[i]);
    }
    free(board->board);
    free(board);
}

void print_dashes(sudokoBoard *board) {
    int dashes = CELL_SPACE*board->boardSize + board->heightOfBlock + 1;
    int i;
    for (i = 0; i < dashes; ++i) {
        printf("-");
    }
    printf("\n");
}
void printCell(int valueToPrint , cell cellToPrint){    /*print 4 digits - space , number(2 digits) , dot asterisk or space according to cell data*/
    printf(" ");
    (valueToPrint == 0 )? printf(BLANK_SPACE) : printf("%2d" , valueToPrint);
    if(cellToPrint.is_fixed){
        printf(".");                /* 10.*/  /*fixed cell*/
    }
    else if (cellToPrint.is_erroneus){
        printf("*");                /* 10**/  /*erroneus cell*/
    }
    else{
        printf(" ");                /* 10 */  /*regular cell*/
    }
}

void printBoard(int whatToPrint , sudokoBoard* sudokoBoard){
    int i, j, valueToPrint;
    print_dashes(sudokoBoard);
    for(i=0 ; i< sudokoBoard->boardSize; ++i){
        printf("|");
        for(j=0 ; j<sudokoBoard->boardSize; ++j) {
            switch(whatToPrint){
                case 0 :
                    valueToPrint = sudokoBoard->board[i][j].value;
                    break;
                case 1:
                    valueToPrint = sudokoBoard->board[i][j].solution_value;
                    break;
                default:
                    valueToPrint = sudokoBoard->board[i][j].value;
            }
            printCell(valueToPrint , sudokoBoard->board[i][j]);             /*print 4 digits - space , number(2 digits) , dot asterisk or space according to cell data*/
            if (j % sudokoBoard->widthOfBlock == sudokoBoard->widthOfBlock - 1)
                printf("|");
        }
        printf("\n");
        if(i % sudokoBoard->heightOfBlock == sudokoBoard->heightOfBlock-1)
            print_dashes(sudokoBoard);
        }

}


int checkIfValid_ROW(sudokoBoard *board , int value , int row, int column){
    int i;
    for(i=0 ; i<board->boardSize ; ++i){
        if(value == board->board[row][i].value &&  i != column){
            return FALSE;
        }
    }
    return TRUE;
}


int checkIfValid_COLUMN(sudokoBoard *board , int value ,int row, int column){
    int i;
    for(i=0 ; i<board->boardSize ; ++i){
        if(value == board->board[i][column].value && i!= row){
            return FALSE;
        }
    }
    return TRUE;
}


int checkIfValid_BLOCK(sudokoBoard *board , int value , int row , int column){
    int upperRow , leftColumn , i , j;
    upperRow = row/board->heightOfBlock;
    upperRow *= board->heightOfBlock;
    leftColumn = column/board->widthOfBlock;
    leftColumn*= board->widthOfBlock;
    for(i=upperRow ; i < upperRow+board->heightOfBlock ; ++i){
        for(j=leftColumn ; j < leftColumn + board->widthOfBlock ; ++j){
            if(value == board->board[i][j].value && (i != row || j != column)){
                return FALSE;
            }
        }
    }
    return TRUE;
}

int checkIfValid(sudokoBoard *board , int value ,int  row ,int column){
    if(checkIfValid_BLOCK(board , value , row , column) && checkIfValid_COLUMN(board , value , row , column)  && checkIfValid_ROW(board , value , row , column))
        return TRUE;
    return FALSE;
}







