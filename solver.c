

#include <vss.h>
#include <fvec.h>
#include "solver.h"

#define TRIES_FOR_GENERATOR 1000


sudokoBoard *generator(sudokoBoard *board, int numOfLegalValues, int numOfFixedCells);

int calcNumOfPotentialValues(sudokoBoard *board);

int isEmptySmallerThenLegalVal(sudokoBoard *board, int numOfLegalValues);

void solutionValueToValue(sudokoBoard *board, int numOfFixedCells);

int fillRandomCells(sudokoBoard *board, int numOfLegalValues);

int fillCellRandValue(sudokoBoard *board, int row, int column);

void chooseRandFixedCells(sudokoBoard *board, int numOfFixedCells);

void allValuesToSolutionValues(sudokoBoard *board);

void firstDynamicCellRowAndColumn(sudokoBoard *board, int *pFirstDynamicCellColumn, int *pFirstDynamicCellRow);

void lastDynamicRowAndColumn(sudokoBoard *board, int *pLastDynamicCellColumn, int *pLastDynamicCellRow);

void nextCellRowAndColumn(sudokoBoard *board, int *pNextColumn, int *pNextRow, int currentRow,
                          int currentColumn);

void prevCellRowAndColumn(sudokoBoard *board, int *pPrevColumn, int *pPrevRow, int currentRow,
                          int currentColumn);

int findNextValue(sudokoBoard *board, int row, int column);

sudokoBoard *generate(sudokoBoard *board, int numOfLegalValues, int numOfEmptyCells) {
    int try;
    sudokoBoard *workingBoard;
    for (try = 0; try < TRIES_FOR_GENERATOR; ++try) {
        workingBoard = copyBoard(board);
        if (generator(workingBoard, numOfLegalValues, numOfEmptyCells)) {
            destroyBoard(board);
            return workingBoard;
        }
        destroyBoard(workingBoard);
    }
    return NULL;
}

int validate(sudokoBoard *board) {          /*check if board is solvable - if solvable - update solution value*/
    int isSolvable;
    isSolvable = gurobi(board, 0, VALIDATE, 0, 0);
    if (isSolvable == SOLVABLE)
        return SOLVABLE;
    else
        return FALSE;

}

sudokoBoard *guess(sudokoBoard *board, float threshold) {
    sudokoBoard *workingBoard = copyBoard(board);
    if (gurobi(workingBoard, threshold, GUESS, 0, 0) != SOLVABLE) {
        error_message(unsolvable_board, CmdArray[GUESS]);
        destroyBoard(workingBoard);
        return NULL;
    }
    destroyBoard(board);
    return workingBoard;
}

int guessHint(sudokoBoard *board, int row, int column) {
    if (gurobi(board, 0, GUESS_H, row, column) == UNSOLVABLE) {
        return error_message(unsolvable_board, CmdArray[GUESS_H]);
    }
}

int numOfSolutions(sudokoBoard *board) {
    Iteration *currentIteration;
    int numOfSol;
    int currentRow, currentColumn,
            firstDynamicCellRow, firstDynamicCellColumn,
            lastDynamicRow, lastDynamicColumn,
            nextRow, nextColumn, prevRow, prevColumn;
    stack *iterationStack;
    sudokoBoard *workingBoard;
    workingBoard = copyBoard(board);
    iterationStack = createStack();
    numOfSol = 0;
    firstDynamicCellRowAndColumn(workingBoard, &firstDynamicCellColumn, &firstDynamicCellRow);
    lastDynamicRowAndColumn(workingBoard, &lastDynamicColumn, &lastDynamicRow);
    Push(iterationStack, 0, 0);
    while (1) {
        currentIteration = top(iterationStack);
        currentRow = currentIteration->row;
        currentColumn = currentIteration->column;
        nextCellRowAndColumn(workingBoard, &nextColumn, &nextRow, 0, 0);
        prevCellRowAndColumn(workingBoard, &prevColumn, &prevRow, 0, 0);
        if (findNextValue(workingBoard, currentRow, currentColumn)) {
            if (currentRow == lastDynamicRow && currentColumn == lastDynamicColumn) {
                numOfSol++;
                Pop(iterationStack);
            } else {
                Push(iterationStack, nextRow, nextColumn);
            }
        } else {
            if (currentRow == firstDynamicCellRow && currentColumn == lastDynamicColumn) {
                break;
            } else {
                Pop(iterationStack);
            }
        }
    }
    destroyStack(iterationStack);
    destroyBoard(workingBoard);
    return numOfSol;
}

int findNextValue(sudokoBoard *board, int row, int column) {
    cell *currentCell = &(board->board[row][column]);
    while (1) {
        if (currentCell->value >= board->boardSize) {
            currentCell->value = 0;
            return 0;
        }
        currentCell->value++;
        if (checkIfValid(board, currentCell->value, row, column)) {
            return 1;
        }
    }
}

void prevCellRowAndColumn(sudokoBoard *board, int *pPrevColumn, int *pPrevRow, const int currentRow,
                          const int currentColumn) {
    int tRow, tColumn;
    if (currentColumn == 0) {
        tRow = currentRow - 1;
        tColumn = board->boardSize - 1;
    } else {
        tRow = currentRow;
        tColumn = currentColumn - 1;
    }
    while (1) {
        if (board->board[tRow][tColumn].is_fixed == 0) {
            *pPrevRow = tRow;
            *pPrevColumn = tColumn;
            return;
        }
        if (currentColumn == 0) {
            tRow--;
            tColumn = board->boardSize - 1;
        } else {
            tColumn--;
        }
        if (tRow == 0 && tColumn == 0) {
            return;
        }
    }
}

void nextCellRowAndColumn(sudokoBoard *board, int *pNextColumn, int *pNextRow, const int currentRow,
                          const int currentColumn) {
    int tRow, tColumn;
    if (currentColumn == board->boardSize - 1) {
        tRow = currentRow + 1;
        tColumn = 0;
    } else {
        tRow = currentRow;
        tColumn = currentColumn + 1;
    }
    while (1) {
        if (board->board[tRow][tColumn].is_fixed == 0) {
            *pNextRow = tRow;
            *pNextColumn = tColumn;
            return;
        }
        if (currentColumn == board->boardSize - 1) {
            tRow++;
            tColumn = 0;
        } else {
            tColumn++;
        }
        if (tRow == board->boardSize - 1 && tColumn == board->boardSize - 1) {
            return;
        }
    }
}

void lastDynamicRowAndColumn(sudokoBoard *board, int *pLastDynamicCellColumn, int *pLastDynamicCellRow) {
    int tRow, tColumn;
    tRow = board->boardSize - 1;
    tColumn = board->boardSize;
    prevCellRowAndColumn(board, pLastDynamicCellRow, pLastDynamicCellColumn, tRow, tColumn);
}

void firstDynamicCellRowAndColumn(sudokoBoard *board, int *pFirstDynamicCellColumn, int *pFirstDynamicCellRow) {
    int tRow, tColumn;
    tRow = 0;
    tColumn = -1;
    nextCellRowAndColumn(board, pFirstDynamicCellRow, pFirstDynamicCellColumn, tRow, tColumn);
}


sudokoBoard *generator(sudokoBoard *board, int numOfLegalValues, int numOfFixedCells) {
    int status;
    status = fillRandomCells(board, numOfLegalValues);
    if (status == 0) {
        return NULL;
    }
    status = gurobi(board, 0, GENERATE, 0, 0);
    if (status != SOLVABLE) {
        return NULL;
    }
    solutionValueToValue(board, numOfFixedCells);
    return TRUE;
}

int isEmptySmallerThenLegalVal(sudokoBoard *board, int numOfLegalValues) {
    int i, j, potentialCells;
    potentialCells = 0;
    for (i = 0; i < board->boardSize; ++i) {
        for (j = 0; j < board->boardSize; ++j) {
            if (board->board[i][j].value == 0) {
                potentialCells++;
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




