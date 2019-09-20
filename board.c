

#include "board.h"
#include <stdlib.h>
#include <stdio.h>

#define BLANK_SPACE "  "
#define CELL_SPACE 4

/*Declaration of static functions*/
void initBoard(cell ***pBoard, int boardSize);

void copyCell(cell *copyTo, cell *copyFrom);

void print_dashes(sudokoBoard *board);

void printCell(int valueToPrint, cell cellToPrint, int mark_errors,int mark_fixed);

int checkIfValid_ROW(sudokoBoard *board, int value, int row, int column, StatusType to_mark_error);

int checkIfValid_COLUMN(sudokoBoard *board, int value, int row, int column, StatusType to_mark_error);

int checkIfValid_BLOCK(sudokoBoard *board, int value, int row, int column, StatusType to_mark_error);




/*Implementation of public functions*/

sudokoBoard *createBoard(int heightOfBlock, int widthOfBlock) {
    sudokoBoard *board ;
    int i , boardSize;
    cell **tempBoard;
    boardSize = heightOfBlock * widthOfBlock;
    board = (sudokoBoard *) malloc(sizeof(sudokoBoard));
    if(board == NULL)
        return NULL;
    board->boardSize = boardSize;
    tempBoard = (cell **) malloc(boardSize * sizeof(cell *));
    if(tempBoard == NULL)
        return NULL;
    for (i = 0; i < boardSize; ++i){
        tempBoard[i] = (cell *) malloc(boardSize * sizeof(cell));
        if(tempBoard[i] == NULL)
            return NULL;
    }
    board->board = tempBoard;
    board->heightOfBlock = heightOfBlock;
    board->widthOfBlock = widthOfBlock;
    initBoard(&tempBoard, boardSize);
    return board;
}


sudokoBoard *copyBoard(sudokoBoard *board) {
    int boardSize = board->widthOfBlock * board->widthOfBlock, i, j;
    sudokoBoard *newBoard = createBoard(board->heightOfBlock, board->widthOfBlock);
    if(newBoard == NULL)
        return NULL;
    for (i = 0; i < boardSize; ++i) {
        for (j = 0; j < boardSize; ++j) {
            copyCell(&newBoard->board[i][j], &board->board[i][j]);
        }
    }
    return newBoard;
}


void destroyBoard(sudokoBoard *board) {
    int boardSize, i;
    if(board != NULL){
        boardSize = board->boardSize;

        for (i = 0; i < boardSize; ++i) {
            free(board->board[i]);
        }
        free(board->board);
        free(board);
    }
}


void printBoard(int whatToPrint, sudokoBoard *sudokoBoard, int mark_errors, int mark_fixed) {
    int i, j, valueToPrint;
    print_dashes(sudokoBoard);
    for (i = 0; i < sudokoBoard->boardSize; ++i) {
        printf("|");
        for (j = 0; j < sudokoBoard->boardSize; ++j) {
            switch (whatToPrint) {
                case 0 :
                    valueToPrint = sudokoBoard->board[i][j].value;
                    break;
                case 1:
                    valueToPrint = sudokoBoard->board[i][j].solution_value;
                    break;
                default:
                    valueToPrint = sudokoBoard->board[i][j].value;
            }
            printCell(valueToPrint, sudokoBoard->board[i][j], mark_errors,
                      mark_fixed);             /*print 4 digits - space , number(2 digits) , dot asterisk or space according to cell data*/
            if (j % sudokoBoard->widthOfBlock == sudokoBoard->widthOfBlock - 1)
                printf("|");
        }
        printf("\n");
        if (i % sudokoBoard->heightOfBlock == sudokoBoard->heightOfBlock - 1)
            print_dashes(sudokoBoard);
    }

}


StatusType checkIfValid(sudokoBoard *board, int value, int row, int column, int to_mark_error) {
    int valid = 1;  /*if one of them return FALSE valid is FALSE */
    valid = valid && checkIfValid_BLOCK(board, value, row, column, to_mark_error);
    valid = valid &&  checkIfValid_COLUMN(board, value, row, column, to_mark_error);
    valid = valid && checkIfValid_ROW(board, value, row, column, to_mark_error);
    if(valid)
        return TRUE;
    if (to_mark_error)
        board->board[row][column].is_erroneus = 1;
    return FALSE;
}


StatusType is_erroneous(sudokoBoard *board) {
    int i, j;

    for (i = 0; i < board->boardSize; ++i) {
        for (j = 0; j < board->boardSize; ++j) {
            if (board->board[i][j].is_erroneus == 1) {
                return TRUE;
            }
        }
    }
    return FALSE;
}




/*Implementation of static functions*/

void initBoard(cell ***pBoard, int boardSize) {
    int i, j;
    for (i = 0; i < boardSize; ++i) {
        for (j = 0; j < boardSize; ++j) {
            (*pBoard)[i][j].is_fixed = 0;
            (*pBoard)[i][j].value = 0;
            (*pBoard)[i][j].solution_value = 0;
            (*pBoard)[i][j].is_erroneus = 0;

        }
    }
}



void copyCell(cell *copyTo, cell *copyFrom) {

    copyTo->is_fixed = copyFrom->is_fixed;
    copyTo->solution_value = copyFrom->solution_value;
    copyTo->value = copyFrom->value;
    copyTo->is_erroneus = copyFrom->is_erroneus;
}





void print_dashes(sudokoBoard *board) {
    int dashes = CELL_SPACE * board->boardSize + board->heightOfBlock + 1;
    int i;

    for (i = 0; i < dashes; ++i) {
        printf("-");
    }
    printf("\n");
}
/*print 4 digits - space , number(2 digits) , dot asterisk or space according to cell data*/
void printCell(int valueToPrint, cell cellToPrint, int mark_errors,
               int mark_fixed) {    /*print 4 digits - space , number(2 digits) , dot asterisk or space according to cell data*/

    char sayfa = ' ';
    printf(" ");
    (valueToPrint == 0) ? printf(BLANK_SPACE) : printf("%2d", valueToPrint);
    if(mark_fixed == 1){
        sayfa = cellToPrint.is_fixed ? '.' : ' ';
    }
    if(mark_errors == 1){
        sayfa = cellToPrint.is_erroneus ? '*' : ' ';
    }
    printf("%c" , sayfa);
}



int checkIfValid_ROW(sudokoBoard *board, int value, int row, int column, StatusType to_mark_error) {

    int i;
    StatusType status = TRUE;

    for (i = 0; i < board->boardSize; ++i) {
        if (value == board->board[row][i].value && i != column) {
            if (to_mark_error) {
                board->board[row][i].is_erroneus = TRUE;
            }
            status = FALSE;
        }
    }
    return status;
}


int checkIfValid_COLUMN(sudokoBoard *board, int value, int row, int column, StatusType to_mark_error) {
    int i;
    StatusType status = TRUE;

    for (i = 0; i < board->boardSize; ++i) {
        if (value == board->board[i][column].value && i != row) {
            if (to_mark_error) {
                board->board[i][column].is_erroneus = TRUE;
            }

            status = FALSE;
        }
    }
    return status;
}


int checkIfValid_BLOCK(sudokoBoard *board, int value, int row, int column, StatusType to_mark_error) {
    int upperRow, leftColumn, i, j;
    StatusType status = TRUE;

    upperRow = row / board->heightOfBlock;
    upperRow *= board->heightOfBlock;
    leftColumn = column / board->widthOfBlock;
    leftColumn *= board->widthOfBlock;
    for (i = upperRow; i < upperRow + board->heightOfBlock; ++i) {
        for (j = leftColumn; j < leftColumn + board->widthOfBlock; ++j) {
            if (value == board->board[i][j].value && (i != row || j != column)) {
                if (to_mark_error) {
                    board->board[i][j].is_erroneus = TRUE;
                }
                status = FALSE;
            }
        }
    }
    return status;
}








