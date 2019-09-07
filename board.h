

#ifndef SUDOKOPROJECT_BOARD_H
#define SUDOKOPROJECT_BOARD_H
typedef struct {
    int is_fixed; /*0 if fixed 1 if not fixed */
    int is_erroneus; /*0 if erroneus 1 if not*/
    int value; /*0 if empty*/
    int solution_value; /*0 if empty*/
    int *potentialValues;
    int numOfPotentialValues;

} cell;
typedef struct {
    cell **board;
    int markErrors;
    int heightOfBlock;
    int widthOfBlock;
    int boardSize;
} sudokoBoard;

sudokoBoard *createBoard(int heightOfBlock , int widthOfBlock , int markErrors);
sudokoBoard *copyBoard(sudokoBoard *board);
void destroyBoard(sudokoBoard *board);
void printBoard(int whatToPrint , sudokoBoard* sudokoBoard);            /*what to print = 0 --> print cell value
 *                                                                        what to print = 1 --> print cell solution value*/

int checkIfValid(sudokoBoard *board , int value ,int  row ,int column);
int isErroneus(sudokoBoard * board);
#endif //SUDOKOPROJECT_BOARD_H
