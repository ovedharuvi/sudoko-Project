

#include <vss.h>
#include "LP.h"

#define MAP_ROW(x) 3*(x)
#define MAP_COLUMN(x) 3*(x)+1
#define MAP_VALUE(x) 3*(x)+2


/*return the upper left index of the block , blocks are numbered Left-To_Right , Up-To_Down
 * example:
 *           0 1 2
 *           3 4 5
 *           6 7 8*/
void getUpperRowLeftColumn(int *pUpperRow, int *pLeftColumn, int numOfBLock, int heightOfBlock, int widthOfBlock);

/*return 0 if LP command , return 1 if ILP command*/
int LP_OR_ILP(CmdType command);

/*counts the potential values that are valid for the current state of the board */
void getNumOfCoefficients(sudokoBoard *board, int *pNumOfCoefficients);

/*check that memory allocated successfully*/
int checkMem(double *objArray, char *vtype, int *mapArray, int *indArray, double *valArray);

/*fill map array - each potential value for each cell gets 3 cells in map array   (row, column, value)*/
void fillMap(sudokoBoard *board, int *mapArray);

/*preform actions according to command ---
 * _____________GUESS_______________
 *      guess a solution according to the LP solution and fills the cell values if valid.
 * ___________GUESS_HINT____________
 * print to the user the scores of each value for the specific cell
 * _______VALIDATE OR GENERATE______
 * fills the solution value for each cell
 * */
int actByCommand(sudokoBoard *board, double *solArray, int *mapArray, CmdType command, double threshhold,
                 int numOfCoefficients, int guessHintRow, int guessHintColumn,);

/*fills the solution value for each cell*/
void fillSolValuesInBoard(sudokoBoard *sudokoBoard, const double *solArray, const int *mapArray, int numOfCoefficients);

/*Random choose index from potentialValuesArray according to their scores----
 * i.e if the value 2 has a score of 0.3 and value 6 has a score of 0.6---
 *        6 has twice the chance to be picked*/
void randomChooseByWeight(int numOfValues, float *potentialValuesArray, int *pChosenIndex);

void freeAllocatedMem(double *objArray, char *vtype, int *mapArray, int *indArray, double *valArray, GRBmodel **pBmodel,
                      GRBenv **pBenv, double *solArray);

int gurobi(sudokoBoard *sudokoBoard, float threshhold, CmdType command, int guessHintRow, int guessHintColumn) {
    int numOfCoefficients, i, j, v, k, currentRow, currentColumn, binary, returnValue;
    int heightOfBlock, widthOfBlock, solStatus, error, boardSize;
    GRBenv *env;
    GRBmodel *model;
    int *ind, *map = NULL;
    double *val;
    double *obj, *sol = NULL;
    char *vtype;
    double objVal;
    error = 0;
    boardSize = sudokoBoard->boardSize;
    heightOfBlock = sudokoBoard->heightOfBlock;
    widthOfBlock = sudokoBoard->widthOfBlock;
    binary = LP_OR_ILP(command);

    /*check how many variables we need*/
    getNumOfCoefficients(sudokoBoard, &numOfCoefficients);

    /*Allocating mamory*/
    obj = (double *) malloc(sizeof(double) * numOfCoefficients);
    vtype = (char *) malloc(sizeof(char) * numOfCoefficients);
    map = (int *) malloc(3 * sizeof(int) * numOfCoefficients);
    ind = (int *) malloc(sizeof(int) * boardSize);
    val = (double *) malloc(sizeof(double) * boardSize);
    if (checkMem(obj, vtype, map, ind, val)) {
        return MEMORY_ERROR;
    }

    /*Mapping the varibles --- each variable mapped to 3 cells (row,column,value)  */
    fillMap(sudokoBoard, map);

    /*Create environment*/

    error = GRBloadenv(&env, "sudoko.log");
    if (error)
        goto QUIT;
    error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
    if (error)
        goto QUIT;
    error = GRBnewmodel(env, &model, "sudoku", numOfCoefficients, obj, NULL, NULL, vtype, NULL);
    if (error)
        goto QUIT;

    /*Init objective function*/
    for (i = 0; i < numOfCoefficients; ++i) {
        if (binary) {
            vtype[i] = GRB_BINARY;
            obj[i] = 1;
        } else {
            vtype[i] = GRB_CONTINUOUS;
            obj[i] = 1;
        }

    }

    /*Add constraints*/

    /*Each cell gets one value exactly*/


    /*
    for(i=0 ; i < boardSize ; ++i){
        currentColumn = i;
        for(j=0 ; j < boardSize ; j++){
            currentRow = j;
            k=0;
            for(v = 0 ; v < numOfCoefficients ; ++v){
                if(map[MAP_COLUMN(v)] == currentColumn && map[MAP_ROW(v) == currentRow){
                    ind[k] = v;
                    val[k] = 1.0;
                    k++;
                }
            }
        }

        error = GRBaddconstr(model , k , ind , val , GRB_EQUAL,
                             1.0 , NULL);
        if(error)
            goto QUIT;

    }

*/                  /*this is less efficient way but more simple*/

    for (i = 0; i < numOfCoefficients; ++i) {
        currentRow = map[MAP_ROW(i)];
        currentColumn = map[MAP_COLUMN(i)];
        k = 0;
        if (i != numOfCoefficients - 1) {
            while (map[MAP_COLUMN(i + 1)] == currentColumn && map[MAP_ROW(i + 1)] == currentRow) {
                ind[k] = i;
                val[k] = 1.0;
                k++;
                i++;
            }
        }
        ind[k] = i;
        val[k] = 1.0;
        k++;

        error = GRBaddconstr(model, k, ind, val, GRB_EQUAL,
                             1.0, NULL);
        if (error)
            goto QUIT;
    }

    /*each value appears one time in a row*/

    for (v = 0; v < boardSize; v++) {
        for (i = 0; i < boardSize; ++i) {
            currentRow = i;
            k = 0;
            for (j = 0; j < numOfCoefficients; ++j) {
                if (map[MAP_VALUE(j)] == (v + 1) && map[MAP_ROW(j)] == currentRow) {
                    ind[k] = j;
                    val[k] = 1.0;
                    k++;
                }
            }
            error = GRBaddconstr(model, k, ind, val,
                                 GRB_EQUAL, 1.0, NULL);
            if (error)
                goto QUIT;
        }
    }

    /*Each value appears one time in a column*/

    for (v = 1; v <= boardSize; ++v) {
        for (i = 0; i < boardSize; ++i) {
            currentColumn = i;
            k = 0;
            for (j = 0; j < numOfCoefficients; ++j) {
                if (map[MAP_VALUE(j)] == v && map[MAP_COLUMN(j)] == currentColumn) {
                    ind[k] = j;
                    val[k] = j;
                    k++;
                }
            }
            error = GRBaddconstr(model, k, ind, val,
                                 GRB_EQUAL, 1.0, NULL);
            if (error)
                goto QUIT;
        }
    }

    /*Each value appears one time in a block*/
    for (v = 1; v <= boardSize; ++v) {
        int currentValue = v;
        for (i = 0; i < boardSize; ++i) {
            int upperRow, leftColumn;
            k = 0;
            getUpperRowLeftColumn(&upperRow, &leftColumn, i, heightOfBlock, widthOfBlock);
            for (j = 0; j < numOfCoefficients; ++j) {
                int currentUpperRow, currentLeftColumn;
                currentUpperRow = (map[MAP_ROW(j)] / heightOfBlock) * heightOfBlock;
                currentLeftColumn = (map[MAP_COLUMN(j)] / widthOfBlock) * widthOfBlock;
                if (currentLeftColumn == leftColumn && currentUpperRow == upperRow &&
                    map[MAP_VALUE(j)] == currentValue) {
                    ind[k] = j;
                    val[k] = 1.0;
                    k++;
                }
            }
            error = GRBaddconstr(model, k, ind, val,
                                 GRB_EQUAL, 1.0, NULL);
            if (error)
                goto QUIT;
        }
    }
    /*Optimizing model*/
    error = GRBoptimize(model);
    if (error)
        goto QUIT;
    error = GRBwrite(model, "sudoku.lp");
    if (error)
        goto QUIT;
    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &solStatus);
    if (error)
        goto QUIT;
    error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objVal);
    if (error)
        goto QUIT;
    error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, numOfCoefficients, sol);
    if (error)
        goto QUIT;

    /*Change the board depends on command*/
    if (solStatus == GRB_OPTIMAL)
        actByCommand(sudokoBoard, sol, map, command, threshhold, numOfCoefficients, guessHintRow, guessHintColumn, 0);

    QUIT:

    freeAllocatedMem(obj, vtype, map, ind, val, &model, &env, sol);

    if (error)
        return GUROBI_ERROR;
    if (solStatus == GRB_OPTIMAL)
        return SOLVABLE;
    if (solStatus == GRB_INF_OR_UNBD)
        return UNSOLVABLE;
    else {
        return GUROBI_ERROR;
    }


}


int actByCommand(sudokoBoard *board, double *solArray, int *mapArray, CmdType command, double threshhold,
                 int numOfCoefficients, int guessHintRow, int guessHintColumn) {
    int i, j, k, currentRow, currentColumn, boardSize;
    boardSize = board->boardSize;
    if (command == VALIDATE || command == GENERATE) {
        fillSolValuesInBoard(board, solArray, mapArray, numOfCoefficients);
    }

    if (command == GUESS) {
        float *potentialValues;
        i = 0;
        potentialValues = malloc(sizeof(float) * 2 * boardSize);
        if (potentialValues == NULL) {
            return MEMORY_ERROR;     /* need to exit!!*/
        }
        while (1) {
            int chosenIndex;
            k = 0;
            currentColumn = mapArray[MAP_COLUMN(i)];
            currentRow = mapArray[MAP_ROW(i)];

            while (mapArray[MAP_ROW(i)] == currentRow && mapArray[MAP_COLUMN(i) == currentColumn]) {
                if (checkIfValid(board, mapArray[MAP_VALUE(i)], currentRow, currentColumn) &&
                    solArray[i] > threshhold) {
                    potentialValues[k] = mapArray[MAP_VALUE(i)];
                    k++;
                    potentialValues[k] = solArray[i];
                    k++;
                }

                i++;
                if (i == numOfCoefficients)
                    break;

            }
            randomChooseByWeight(k, potentialValues, &chosenIndex);
            board->board[currentRow][currentColumn].value = (int) potentialValues[chosenIndex];
            if (i == numOfCoefficients)
                break;
        }
        return SOLVABLE;
    }


    if (command == GUESS_H) {
        i = 0;
        while (mapArray[MAP_ROW(i)] != guessHintRow || mapArray[MAP_COLUMN(i)] != guessHintColumn) {
            i++;
        }
        while (mapArray[MAP_ROW(i)] == guessHintRow && mapArray[MAP_COLUMN(i)] == guessHintColumn) {
            if (solArray[i] > 0) {
                printf("Score of %d is %f\n", mapArray[MAP_VALUE(i)], solArray[i]);
            }
            i++;
            if (i > numOfCoefficients)
                break;
        }
        return SOLVABLE;
    }
}

void randomChooseByWeight(int numOfValues, float *potentialValuesArray, int *pChosenIndex) {
    int sumOfWeights, randomNumber;
    int i;

    for (i = 1, sumOfWeights = 0; i < numOfValues; i += 2) {
        sumOfWeights += potentialValuesArray[i]*1000;
    }
    randomNumber = rand() % sumOfWeights;
    for (i = 1; i < numOfValues; i += 2) {
        if (randomNumber < potentialValuesArray[i]*1000) {
            *pChosenIndex = i - 1;
            return;
        }
        randomNumber -= potentialValuesArray[i]*1000;
    }
#ifdef DEBUG
    printf("Error: random choose by weights doesn't work");
#endif
}

void fillMap(sudokoBoard *board, int *mapArray) {
    int i, j, v, k;
    for (i = 0; i < board->boardSize; i++) {
        for (j = 0; j < board->boardSize; j++) {
            for (v = 1; v <= board->boardSize; v++) {
                if (checkIfValid(board, v, i, j)) {
                    mapArray[MAP_ROW(k)] = i;
                    mapArray[MAP_COLUMN(k)] = j;
                    mapArray[MAP_VALUE(k)] = v;
                    k++;
                }
            }
        }
    }
}

/*return 0 if memory didn't allocated successfully , 1 otherwise*/
int checkMem(double *objArray, char *vtype, int *mapArray, int *indArray, double *valArray) {
    if (objArray == NULL || vtype == NULL || mapArray == NULL || indArray == NULL
        || valArray == NULL) {
        return 0;
    }
    return 1;
}

void freeAllocatedMem(double *objArray, char *vtype, int *mapArray, int *indArray, double *valArray, GRBmodel **pBmodel,
                      GRBenv **pBenv, double *solArray) {
    free(objArray);
    free(vtype);
    free(mapArray);
    free(indArray);
    free(valArray);
    GRBfreeenv(*pBenv);
    GRBfreemodel(*pBmodel);
    free(solArray);
}

void getNumOfCoefficients(sudokoBoard *board, int *pNumOfCoefficients) {
    int i, j, v;
    for (i = 0; i < board->boardSize; i++) {
        for (j = 0; j < board->boardSize; j++) {
            for (v = 1; v <= board->boardSize; v++) {
                if (checkIfValid(board, v, i, j)) {
                    (*pNumOfCoefficients)++;
                }
            }
        }
    }
}

/*return 0 if command needs LP solution, return 1 if command needs ILP solution*/
int LP_OR_ILP(CmdType command) {
    if (command == GUESS_H || command == GUESS) {
        return 0;
    } else {
        return 1;
    }
}


void getUpperRowLeftColumn(int *pUpperRow, int *pLeftColumn, int numOfBLock, int heightOfBlock, int widthOfBlock) {
    *pLeftColumn = (numOfBLock % heightOfBlock) * widthOfBlock;
    *pUpperRow = (numOfBLock / heightOfBlock) * heightOfBlock;
}

void
fillSolValuesInBoard(sudokoBoard *sudokoBoard, const double *solArray, const int *mapArray, int numOfCoefficients) {
    int i, currentRow, currentColumn, currentValue;
    for (i = 0; i < numOfCoefficients; ++i) {
        if (solArray[i] > 0) {
            currentRow = mapArray[MAP_ROW(i)];
            currentColumn = mapArray[MAP_COLUMN(i)];
            currentValue = mapArray[MAP_VALUE(i)];
            sudokoBoard->board[currentRow][currentColumn].solution_value = currentValue;
        }
    }
}



