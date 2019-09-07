

#include <vss.h>
#include <fvec.h>
#include "solver.h"
#include "LP.h"
#include "stack.h"

#define TRIES_FOR_GENERATOR 1000
#define FORWARD 1
#define BACKWARD 0

typedef enum {
    EXIT = -1, FALSE = 0, TRUE = 1
} StatusType;


int generator(sudokoBoard *board, int numOfLegalValues, int numOfFixedCells);

int calcNumOfPotentialValues(sudokoBoard *board);

int isEmptySmallerThenLegalVal(sudokoBoard *board, int numOfLegalValues);

void solutionValueToValue(sudokoBoard *board, int numOfFixedCells);

int fillRandomCells(sudokoBoard *board, int numOfLegalValues);

int fillCellRandValue(sudokoBoard *board, int row, int column);

void chooseRandFixedCells(sudokoBoard *board, int numOfFixedCells);

void allValuesToSolutionValues(sudokoBoard *board);

int generate(sudokoBoard *board, int numOfLegalValues, int numOfEmptyCells) {
    int try;
    if (isEmptySmallerThenLegalVal(board, numOfLegalValues))
        return ERROR_EMPTY_SMALLER_THEN_LEGAL_VAL;
    for (try = 0; try < TRIES_FOR_GENERATOR; ++try) {
        if (generator(board, numOfLegalValues, numOfEmptyCells))
            return TRUE;
    }
    return FALSE;
}

int validate(sudokoBoard *board) {
    int isSolvable;
    isSolvable = gurobi(board, 0, VALIDATE, 0, 0);
    if (isSolvable == SOLVABLE)
        return SOLVABLE;
    else {
        return FALSE;
    }
}

int guess(sudokoBoard *board, int threshold) {
    if (isErroneus(board)) {
        return BOARD_IS_ERRONEUS_ERROR;
    } else {
        gurobi(board, threshold, GUESS, 0, 0);
    }
}

int guessHint(sudokoBoard *board, int row, int column) {
    if (isErroneus(board)) {
        return BOARD_IS_ERRONEUS_ERROR;
    }
    if (board->board[row][column].is_fixed == 1) {
        return CELL_IS_FIXED_ERROR;
    }
    if (board->board[row][column].value != 0) {
        return CELL_IS_FILLED_ERROR;
    }
    if (gurobi(board, 0, GUESS_H, row, column) == UNSOLVABLE) {
        return BOARD_IS_UNSOLVABLE_ERROR;
    }
}

int numOfSolutions(sudokoBoard *board) {
    Iteration *currentIteration;
    int currentRow, currentColumn, currentDirection ,
        firstDynamicCellRow , firstDynamicCellColumn,
        lastDynamicRow, lastDynamicColumn,
        boardSize , heightOfBlock , widthOfBlock,
        nextRow, nextColumn , prevRow , prevColumn;
    stack *itarationStack;
    sudokoBoard *workingBoard , *currentBoard;
    cell currentCell;
    boardSize = board->boardSize;
    heightOfBlock = board->heightOfBlock;
    widthOfBlock = board->widthOfBlock;
    workingBoard = copyBoard(board);
    itarationStack = createStack();
    firstDynamicCellRowAndColumn(&firstDynamicCellRow , &firstDynamicCellColumn);
    lastDynamicRowAndColumn(&lastDynamicRow , &lastDynamicColumn);
    if (isErroneus(workingBoard)) {
        return BOARD_IS_ERRONEUS_ERROR;
    }
    Push(itarationStack , 0 , 0 , FORWARD);
    while (1) {
        currentIteration = top(itarationStack);
        currentRow = currentIteration->row;
        currentColumn = currentIteration->column;
        currentDirection = currentIteration->direction;
        currentCell = workingBoard->board[currentRow][currentColumn];
        nextCellRowAndColumn(&nextRow , &nextColumn);
        prevCellRowAndColumn(&prevRow , &prevColumn);
        if(currentDirection == FORWARD){
            if(!findValue(board , currentRow , currentColumn)){
                Push(itarationStack, prevRow, prevColumn, BACKWARD);
            }
            else{

            }
        }






            break;
    }

}


int generator(sudokoBoard *board, int numOfLegalValues, int numOfFixedCells) {
    int status = 1;
    status = fillRandomCells(board, numOfLegalValues);
    if (status == 0) {
        return FALSE;
    }
    status = gurobi(board, 0, GENERATE, 0, 0);
    if (status != SOLVABLE) {
        return FALSE;
    }
    solutionValueToValue(board, numOfFixedCells);
    return TRUE;
}

int isEmptySmallerThenLegalVal(sudokoBoard *board, int numOfLegalValues) {
    int i, j, potentialCells, v;
    potentialCells++;
    for (i = 0; i < board->boardSize; ++i) {
        for (j = 0; j < board->boardSize; ++j) {
            for (v = 1; v <= board->boardSize; ++v) {
                if (checkIfValid(board, v, i, j)) {
                    potentialCells++;
                    break;
                }
            }
        }
    }

    if (potentialCells < numOfLegalValues) {
        return TRUE;
    }
    return FALSE;
}


int fillRandomCells(sudokoBoard *board, int numOfLegalValues) {
    int filled, randRow, randColumn;
    filled = 0;
    while (filled < numOfLegalValues) {
        randRow = rand() % board->boardSize;
        randColumn = rand() % board->boardSize;
        if (board->board[randRow][randColumn].value != 0)
            continue;
        if (fillCellRandValue(board, randRow, randColumn)) {
            filled++;
            continue;
        }
        return 0;
    }
}

int fillCellRandValue(sudokoBoard *board, int row, int column) {
    int *potentialValues, numOfPotentialValues, v, randIndex;
    numOfPotentialValues = 0;
    potentialValues = (int *) malloc(sizeof(int) * board->boardSize);
    for (v = 1; v <= board->boardSize; v++) {
        if (checkIfValid(board, v, row, column)) {
            potentialValues[numOfPotentialValues] = v;
        }
    }
    if (potentialValues == 0) {
        free(potentialValues);
        return 0;
    }
    randIndex = rand() % numOfPotentialValues;
    board->board[row][column].value = potentialValues[randIndex];
    free(potentialValues);
    return 1;
}

void solutionValueToValue(sudokoBoard *board, int numOfFixedCells) {
    allValuesToSolutionValues(board);
    chooseRandFixedCells(board, numOfFixedCells);
}

void allValuesToSolutionValues(sudokoBoard *board) {
    int i, j;
    for (i = 0; i < board->boardSize; ++i) {
        for (j = 0; j < board->boardSize; ++j) {
            if (board->board[i][j].value != 0)
                board->board[i][j].solution_value = board->board[i][j].value;
            board->board[i][j].value = 0;
        }
    }

}

void chooseRandFixedCells(sudokoBoard *board, int numOfFixedCells) {
    int randRow, randColumn, filled;
    filled = 0;
    while (filled < numOfFixedCells) {
        randRow = rand() % board->boardSize;
        randColumn = rand() % board->boardSize;
        if (board->board[randRow][randColumn].value == 0) {
            filled++;
            board->board[randRow][randColumn].value = board->board[randRow][randColumn].solution_value;
        }
    }
}




