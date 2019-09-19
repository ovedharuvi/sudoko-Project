

#include "solver.h"

#define TRIES_FOR_GENERATOR 1000

/*Static functions declaration*/


/*
 * Return Value: sudokoBoard* -- if generator succeeded
 *               NULL         -- otherwise
 * 3 stages algorithm:
 *              1. Fill randomly 'numOfLegalValue' cells with random valid values.
 *              2. Solve the board using ILP.
 *              3. Choose randomly 'numOfFixedCells' to remain fixed and change all other cell's value to 0*/
sudokoBoard *generator(sudokoBoard *board, int numOfLegalValues, int numOfFixedCells);


/*init all values to 0 and randomly fill 'numOfFixedCells' with the solution value given from ILP and is.fixed = 1 */
void solutionValueToValue(sudokoBoard *board, int numOfFixedCells);


/*Choose 'numOfLegalValues' random cells and fill them with a random valid value
 *  Return Value - 1 */
StatusType fillRandomCells(sudokoBoard *board, int numOfLegalValues);


/*Chooses randomly a value to a cell from all valid values
 *          Return Value - TRUE if value is initialized.
 *                         FALSE if memory problem or there is no valid value for the given cell*/
int fillCellRandValue(sudokoBoard *board, int row, int column);


/*  Choose random 'numOfFixedCells' to init their value to sol_value and turn them into a fixed cell*/
void chooseRandFixedCells(sudokoBoard *board, int numOfFixedCells);


/*  Init all cells' value to 0 and solution value to old value */
void allValuesToSolutionValues(sudokoBoard *board);


/*Call by reference function
 *  search the first none-fixed cell in the board , initialize it's index to pFirstDynamicCellColumn, pFirstDynamicCellRow*/
void firstDynamicCellRowAndColumn(sudokoBoard *board, int *pFirstDynamicCellColumn, int *pFirstDynamicCellRow);


/*Call by reference function
 *  search the last none-fixed cell in the board , initialize it's index to pLastDynamicCellColumn, pLastDynamicCellRow*/
void lastDynamicRowAndColumn(sudokoBoard *board, int *pLastDynamicCellColumn, int *pLastDynamicCellRow);


/*  Call by reference function
 *  find the next not-fixed cell and initialize it's index to pNextRow, pNextColumn*/
void nextCellRowAndColumn(sudokoBoard *board, int *pNextColumn, int *pNextRow, int currentRow,
                          int currentColumn);


/*  Call by reference function.
 * Find the previous not-fixed cell and initialize it's index at pPrevRow , pPrevColumn.  */
void prevCellRowAndColumn(sudokoBoard *board, int *pPrevColumn, int *pPrevRow, int currentRow,
                          int currentColumn);


/*  Used in NumOfSol ----
 *  Return Value: TRUE if value found and initialized ,  FALSE if value did not found -- then 0 initialized as new value
 *  finding the next(down to up) valid value for cell board[row][column].
 *  i.e first tries 1 then 2 and so on.
 *  if the cell value was 3 the function will search the one after that.
 * */
StatusType findNextValue(sudokoBoard *board, int row, int column);


/*  Preparation for num of sol --
 *  every cell which contains a value - is fixed cell in the working board.*/
void fixAllCells(sudokoBoard *board);


/*Public functions implementation - Documentation in header file*/

sudokoBoard *generate(sudokoBoard *board, int numOfLegalValues, int numOfFixedCells) {
    int try;
    sudokoBoard *workingBoard;
    for (try = 0; try < TRIES_FOR_GENERATOR; ++try) {
        workingBoard = copyBoard(board);
        if (generator(workingBoard, numOfLegalValues, numOfFixedCells)) {
            return workingBoard;
        }
        destroyBoard(workingBoard);
    }
    return NULL;
}

StatusType validate(sudokoBoard *board) {
    return gurobi(board, 0, VALIDATE, 0, 0);
}

sudokoBoard *guess(sudokoBoard *board, float threshold) {
    sudokoBoard *workingBoard;
    workingBoard = copyBoard(board);
    if (gurobi(workingBoard, threshold, GUESS, 0, 0) != TRUE) {
        error_message(unsolvable_board, CmdArray[GUESS]);
        destroyBoard(workingBoard);
        return NULL;
    }
    return workingBoard;
}

int guessHint(sudokoBoard *board, int row, int column) {
    if (gurobi(board, 0, GUESS_H, row, column) == FALSE) {
        return error_message(unsolvable_board, CmdArray[GUESS_H]);
    }
    return TRUE;
}

int numOfSolutions(sudokoBoard *board) {
    Iteration *currentIteration;
    int numOfSol;
    int currentRow, currentColumn,
            firstDynamicCellRow, firstDynamicCellColumn,
            lastDynamicRow, lastDynamicColumn,
            nextRow, nextColumn;
    stack *iterationStack;
    sudokoBoard *workingBoard;
    workingBoard = copyBoard(board);
    fixAllCells(workingBoard);
    iterationStack = createStack();
    numOfSol = 0;
    nextRow = 0;
    nextColumn = 0;
    lastDynamicColumn = 0;
    lastDynamicRow = 0;
    firstDynamicCellColumn = 0;
    firstDynamicCellRow = 0;
    firstDynamicCellRowAndColumn(workingBoard, &firstDynamicCellColumn, &firstDynamicCellRow);
    lastDynamicRowAndColumn(workingBoard, &lastDynamicColumn, &lastDynamicRow);
    Push(iterationStack, firstDynamicCellRow, firstDynamicCellColumn);
    while (1) {
        currentIteration = top(iterationStack);
        currentRow = currentIteration->row;
        currentColumn = currentIteration->column;
        if (!(currentRow == lastDynamicRow && currentColumn == lastDynamicColumn))
            nextCellRowAndColumn(workingBoard, &nextColumn, &nextRow, currentRow, currentColumn);
        if (findNextValue(workingBoard, currentRow, currentColumn)) {
            if (currentRow == lastDynamicRow && currentColumn == lastDynamicColumn) {
                numOfSol++;
                workingBoard->board[currentRow][currentColumn].value = 0;
                Pop(iterationStack);
            } else {
                Push(iterationStack, nextRow, nextColumn);
            }
        } else {
            if (currentRow == firstDynamicCellRow && currentColumn == firstDynamicCellColumn) {
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


/*  Checks if the value 'numOfLegalValues' is NOT Valid
 *  return TRUE if number of empty cell is smaller than the number provided by the user (not valid)
 *  return FALSE otherwise*/
StatusType isEmptySmallerThanLegalVal(sudokoBoard *board, int numOfLegalValues) {
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



/*Static function implementation*/


void fixAllCells(sudokoBoard *board) {
    int i, j;
    for (i = 0; i < board->boardSize; ++i) {
        for (j = 0; j < board->boardSize; ++j) {
            if (board->board[i][j].value > 0) {
                board->board[i][j].is_fixed = TRUE;
            }
        }
    }
}


StatusType findNextValue(sudokoBoard *board, int row, int column) {
    cell *currentCell = &(board->board[row][column]);
    while (1) {
        if (currentCell->value >= board->boardSize) {
            currentCell->value = 0;
            return FALSE;
        }
        currentCell->value++;
        if (checkIfValid(board, currentCell->value, row, column, FALSE)) {
            return TRUE;
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
        if (tColumn == 0) {
            tRow--;
            tColumn = board->boardSize - 1;
        } else {
            tColumn--;
        }
        if (tRow == 0 && tColumn == 0) {
            if (board->board[tRow][tColumn].is_fixed == 0) {
                *pPrevRow = tRow;
                *pPrevColumn = tColumn;
                return;
            }
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
        if (tColumn == board->boardSize - 1) {
            tRow++;
            tColumn = 0;
        } else {
            tColumn++;
        }
        if (tRow == board->boardSize - 1 && tColumn == board->boardSize - 1) {
            if (board->board[tRow][tColumn].is_fixed == 0) {
                *pNextRow = tRow;
                *pNextColumn = tColumn;
                return;
            }
            return;
        }
    }
}



void lastDynamicRowAndColumn(sudokoBoard *board, int *pLastDynamicCellColumn, int *pLastDynamicCellRow) {
    int tRow, tColumn;
    tRow = board->boardSize - 1;
    tColumn = board->boardSize;
    prevCellRowAndColumn(board, pLastDynamicCellColumn, pLastDynamicCellRow, tRow, tColumn);
}



void firstDynamicCellRowAndColumn(sudokoBoard *board, int *pFirstDynamicCellColumn, int *pFirstDynamicCellRow) {
    int tRow, tColumn;
    tRow = 0;
    tColumn = -1;
    nextCellRowAndColumn(board, pFirstDynamicCellColumn, pFirstDynamicCellRow, tRow, tColumn);
}


sudokoBoard *generator(sudokoBoard *board, int numOfLegalValues, int numOfFixedCells) {
    StatusType status;
    status = fillRandomCells(board, numOfLegalValues);
    if (status == FALSE) {
        return NULL;
    }
    status = gurobi(board, 0, GENERATE, 0, 0);
    if (status == FALSE) {
        return NULL;
    }
    solutionValueToValue(board, numOfFixedCells);
    return board;
}


StatusType fillRandomCells(sudokoBoard *board, int numOfLegalValues) {
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
        return FALSE;
    }
    return TRUE;
}


int fillCellRandValue(sudokoBoard *board, int row, int column) {
    int *potentialValues, numOfPotentialValues, v, randIndex;
    numOfPotentialValues = 0;
    potentialValues = (int *) malloc(sizeof(int) * board->boardSize);
    if (potentialValues == NULL)
        return FALSE;
    for (v = 1; v <= board->boardSize; v++) {
        if (checkIfValid(board, v, row, column, FALSE)) {
            potentialValues[numOfPotentialValues] = v;
            numOfPotentialValues++;
        }
    }
    if (numOfPotentialValues == 0) {
        free(potentialValues);
        return FALSE;
    }
    randIndex = rand() % numOfPotentialValues;
    board->board[row][column].value = potentialValues[randIndex];
    free(potentialValues);
    return TRUE;
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
            board->board[randRow][randColumn].is_fixed = TRUE;
        }
    }
}




