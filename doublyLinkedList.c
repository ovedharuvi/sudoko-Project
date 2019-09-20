

#include "doublyLinkedList.h"

struct Node {                                  /*action represented by node */
    ACTION* action ;
    struct Node* next;
    struct Node* prev;
};

struct Node* head;                              /*maintaining 3 pointers - last action , first action and current action*/
struct Node* tail;
struct Node* currentAction;

/*Static functions declaration*/

ACTION* createAction(int oldValue , int newValue, int row, int column, int insertedByComputer , CmdType command);

struct Node* createNode(int oldValue , int newValue , int row , int column ,int insertedByComputer, CmdType command );



/*Public functions implementation*/
void InsertAction (int oldValue , int newValue, int row , int column , int insertedByComputer , CmdType command) {
    struct Node* newNode = createNode(oldValue , newValue , row, column, insertedByComputer, command);
    if(newNode == NULL)
        printf("Error")
    if(head == NULL){                               /*check if this is the first action (new list)*/
        head = newNode;
        tail = newNode;
        currentAction = newNode;
        return;
    }
    if(currentAction == NULL){                      /*if first action undo and then action made*/
        destroyList();
        head = newNode;
        tail = newNode;
        currentAction = newNode;
        return;
    }
    while( currentAction != tail){                  /*if there are actions we need to delete from memory */
        tail = tail->prev;
        free(tail->next->action);
        free(tail->next);
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

void destroyList(){

    struct Node* temp;
    if (head == NULL){
        return;
    }
    temp = tail;
    while(temp != head){
        temp = temp->prev;
        free(temp->next->action);
        free(temp->next);
    }
    head=NULL;
    tail=NULL;
    currentAction=NULL;
    free(temp->action);
    free(temp);

}




/*Static functions implementation*/

ACTION* createAction(int oldValue , int newValue, int row, int column, int insertedByComputer , CmdType command){
    ACTION* action = (ACTION*)malloc(sizeof(ACTION));
    if(action == NULL)
        return NULL;
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
    if(newNode == NULL)
        return NULL;
    newNode->action = createAction(oldValue , newValue , row , column , insertedByComputer, command);
    if(newNode->action == NULL)
        return NULL;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}
