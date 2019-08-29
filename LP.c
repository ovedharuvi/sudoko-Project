

#include "LP.h"
typedef enum {
    SOLVE = 0,EDIT = 1,MARK = 2, PRINT = 3, SET = 4, VALIDATE = 5, GUESS = 6, GENERATE = 7, UNDO = 8,
    REDO = 9, SAVE = 10, HINT = 11, GUESS_H = 12, NUM_S = 13, AUTOFILL = 14, RESET = 15, EXIT_GAME = 16, INVALID =17
}CmdType;

int gurobi(sudokoBoard* sudokoBoard, CmdType command){
    int numOfCoefficints , i , j , boardSize , heightOfBlock , widthOfBlock;

    numOfCoefficints = getNumOfCoefficients(sudokoBoard);

    return 0;
}

