

#include "doublyLinkedList.h"

struct Node {                                  /*action represented by node */
    ACTION* action ;
    struct Node* next;
    struct Node* prev;
};

struct Node* head;                              /*maintaining 3 pointers - last action , first action and current action*/
struct Node* tail;
struct Node* currentAction;

void printAction(ACTION *action){
    printf("Old Value: %d \nNew Value: %d \nCell: [%d][%d] \nCmdType: %d \nInserted By Computer: %d",
            action->oldValue , action->newValue , action->row , action->column , action->command, action->insertedByComputer);
}

ACTION* createAction(int oldValue , int newValue, int row, int column, int insertedByComputer , CmdType command){
    ACTION* action = (ACTION*)malloc(sizeof(ACTION));
    action->oldValue = oldValue;
    action->newValue = newValue;
    action->row = row;
    action->column= column;
    action->insertedByComputer = insertedByComputer;
    action->command = command;
    return action;
}


struct Node* createNode(int oldValue , int newValue , int row , int column ,int insertedByComputer, CmdType command ){
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->action = createAction(oldValue , newValue , row , column , insertedByComputer, command);
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}


void InsertAction (int oldValue , int newValue, int row , int column , int insertedByComputer , CmdType command) {
    struct Node* newNode = createNode(oldValue , newValue , row, column, insertedByComputer, command);
    if(head == NULL){                               /*check if this is the first action (new list)*/
        head = newNode;
        tail = newNode;
        currentAction = newNode;
    }
    while( currentAction != tail){                  /*if there are actions we need to delete from memory */
        tail = tail->prev;
        free(tail->next);
        tail->next = NULL;
    }
    newNode->prev = currentAction;
    currentAction->next = newNode;
    tail = newNode;
    currentAction = newNode;
}

ACTION* listUndo(){
    if (currentAction == NULL){
        return NULL;
    }
    if(currentAction == head){
        currentAction = NULL;
        return head->action;
    }
    currentAction = currentAction->prev;
    return currentAction->next->action;
}

ACTION* listRedo(){
    if (currentAction == NULL){
        currentAction = head;
        return head->action;
    }

    if(currentAction == tail){
        return NULL;
    }
    currentAction = currentAction->next;
    return  currentAction->action;
}
/*void printLastToFirst(){
    struct Node* temp = tail;
    if(head == NULL){
        return;
    }
    while(temp != head){
        printAction(temp->action);
        temp = temp->prev;
    }
}
*/
/*
void printFirstToCurrentAction(){
    struct Node* temp = head;
    if(head == NULL){
        return;
    }
    while(temp != currentAction){
        printAction(temp->action);
        temp = temp->next;
    }
}
*/
void destroyList(){
    struct Node* temp = tail;
    if(temp == NULL){
        return;
    }
    while(temp != head){
        temp = temp->prev;
        free(temp->next->action);
        free(temp->next);
    }
    free(temp->action);
    free(temp);
    /*
    free(head);
    free(tail);
    free(currentAction);
     */
}
