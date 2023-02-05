// Name: Khanh Gia Nguyen
// Student ID: 32498349
// Date of Submission: Tuesday, 26th of April 2022

/*
Assignmnent Description:
    - Investigate the Lee Algorithm for finding a path between two locations.
        - The algorithm guarantees the shortest path.
    - Task 1:
        - Accept a command line argument that takes in the text file of the maze.
    - Task 2:
        - If it finds a path, it should print a trace of the path as a sequence 
          of matrix coordinates separated by space characters.
        - If not, it should print "No path found.".
    - Not allowed:
        - Global or static arrays.
        - Starting from source, cells that are visited should be placed in a queue 
          (of which elements are dynamically allocated).
        - Solution path (if ever exists) should be placed in a dynamically allocated stack.
*/

// C LIBRARY:
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// QUEUE AND STACK DEFINITIONS:
struct queueNode{
    int data;
    struct queueNode *nextPtr;
};

typedef struct queueNode QueueNode;
typedef QueueNode *QueueNodePtr;

int isEmpty(QueueNodePtr headPtr){
    return headPtr == NULL;
}

int dequeue(QueueNodePtr *headPtr, QueueNodePtr *tailPtr)
{
    int value = (*headPtr)->data;
    QueueNodePtr tempPtr = *headPtr;
    *headPtr = (*headPtr)->nextPtr;

    if(*headPtr == NULL){
        *tailPtr = NULL;
    }

    free(tempPtr);
    return value;
}

void enqueue(QueueNodePtr *headPtr, QueueNodePtr *tailPtr, int value){
    QueueNodePtr newPtr = malloc(sizeof(QueueNode));

    if(newPtr != NULL){
        newPtr->data = value;
        newPtr->nextPtr = NULL;

        if(isEmpty(*headPtr)){
            *headPtr = newPtr;
        }
        else{
            (*tailPtr)->nextPtr = newPtr;
        }
        *tailPtr = newPtr;
    } 
    else{
        printf("%c not inserted. No memory available.\n", value);
    }
}

void printQueue(QueueNodePtr currentPtr){
    if (currentPtr == NULL) {
        puts("Queue is empty.\n");
    }
    else {
        puts("The queue is:");

        while (currentPtr != NULL) {
            printf("%d --> ", currentPtr->data);
            currentPtr = currentPtr->nextPtr;
        }       
        puts("NULL\n");
    }   
}

struct stackNode{
    int data;
    struct stackNode *nextPtr;
};

typedef struct stackNode StackNode; 
typedef StackNode *StackNodePtr;

void push(StackNodePtr *topPtr, int info){
    StackNodePtr newPtr = malloc(sizeof(StackNode));

    if(newPtr != NULL){
        newPtr->data = info;
        newPtr->nextPtr = *topPtr;
        *topPtr = newPtr;
    }
    else{
        printf("%d not inserted. No memory available.\n", info);
    }
}

int pop(StackNodePtr *topPtr){
    StackNodePtr tempPtr = *topPtr;
    int popValue = (*topPtr)->data;
    *topPtr = (*topPtr)->nextPtr;
    free(tempPtr);
    return popValue;
}

int isEmptyStack(StackNodePtr topPtr){
    return topPtr == NULL;
}

void printStack(StackNodePtr currentPtr){
    if(currentPtr == NULL){
        puts("The stack is empty.\n");
    }
    else{
        puts("The stack is:");
        while(currentPtr != NULL){
            printf("%d --> ", currentPtr->data);
            currentPtr = currentPtr->nextPtr;
        }
        puts("NULL\n");
    }
}

// MAIN FUNCTION CALL:
int main(int argc, char*argv[])
{
    if(argc != 2){
        puts("Usage: mycopy infile outfile");
    }

    FILE *fp;
    fp = fopen(argv[1], "r");

    // FINDING THE SIZE OF THE SQUARE MAZE:
    int lines = 0;
    int ch = 0;
    while(!feof(fp)){
        ch = fgetc(fp);
        if(ch == '\n'){
            lines++;
        }
    }
    fclose(fp);

    int SIZE = lines + 2;

    // VARIABLE INITIALISERS:
    int (*mazeMapper)[SIZE-1] = malloc(sizeof(int[SIZE-1][SIZE-1]));
    char line[SIZE];
    int count = 0;

    // FILE DECLARATIONS:
    FILE *filePtr;
    filePtr = fopen(argv[1], "r");
    
    // CREATING 2D ARRAY:
    if (filePtr == NULL){
        printf("This file is empty.");
        return -1;
    }    
    
    int i = 0;
    while(!feof(filePtr)){
        fgets(line, SIZE, filePtr);
        if (count%2 == 0){
            for(int j = 0; j < SIZE-1; j++){
                if(line[j] == '#')              // # IS -2
                mazeMapper[i][j] = -2;
                else if(line[j] == '.')         // . is -1
                mazeMapper[i][j] = -1;
                else if(line[j] == 'S')         // S is -3
                mazeMapper[i][j] = -3;
                else if(line[j] == 'T')         // T is -4
                mazeMapper[i][j] = -4;
            }
            i++;
        }
        count++;
    }

    // OPTIONAL SEGMENT FOR SEEING WHAT THE MAZE LOOKS LIKE (PRINTING THE MAZE):
    /*
    printf("\n");
    for(int k = 0; k < SIZE - 1 ; k++){
        for(int l = 0; l < SIZE - 1; l++){
            printf("  %3d ", mazeMapper[k][l]);
        }
        printf("\n");
    }
    */

    // IMPLEMENT LEE'S ALGORITHM:

    // FIND STARTING POINT:
    int start[2];
    bool startStat = false;
    for(int n = 0; n < SIZE-1; n++){
        for(int m = 0; m < SIZE-1; m++){
            if(mazeMapper[n][m] == -3){
                start[0]= n;
                start[1] = m;
                startStat = true;
                break;
            }
        }
        if(startStat == true){
            break;
        }
    }
    

    // DEFINE QUEUE:
    QueueNodePtr headPtr = NULL;
    QueueNodePtr tailPtr = NULL;

    // INITIALISE QUEUE WITH START NODE:
    enqueue(&headPtr, &tailPtr, start[0]);
    enqueue(&headPtr, &tailPtr, start[1]);

    
    // MAIN LEE ALGORITHM:

    // MOVEMENT ARRAYS:
    int rowMove[4] = {-1, 0, 0, 1}; 
    int colMove[4] = {0, -1, 1, 0}; 

    // VARIABLE DEFINITIONS:
    int currentDistance = 0; // Current distance counter.
    int row, col, tempRow, tempCol; // Define row and col to be looked at.
    int current = -3;
    bool status = false;
    mazeMapper[start[0]][start[1]] = 0;
    int end[2];
    bool ultStackStat = false;

    // LOOP TO INITIATE THE CHECKING OF PATH TO TARGET USING QUEUE:
    while(status != true){
        // Access current row and column being looked at from the queue.
        if(isEmpty(headPtr)){
            printf("No path found.");
            ultStackStat = true;
            break;
        }
        row = dequeue(&headPtr, &tailPtr);
        col = dequeue(&headPtr, &tailPtr);
        int curent = mazeMapper[row][col];

        // Mark all neighbours with current distance.
        for(int iMov = 0; iMov < 4; iMov++){
            // Update row and collumn for current neighbour check.

            // Need to check for absolute valid moves from the move list check; if the moves are valid then it can go through.
            tempRow = row + rowMove[iMov];
            tempCol = col + colMove[iMov];
            if (tempCol >= 0 && tempCol < SIZE - 1 && tempRow >= 0 && tempRow < SIZE - 1){
                // Marks the value at the move if not a wall and add spots of marking to queue.
                if(mazeMapper[tempRow][tempCol] != -2 && mazeMapper[tempRow][tempCol] == -1 || mazeMapper[tempRow][tempCol] == -4){
                    if (mazeMapper[tempRow][tempCol] == -4){
                        end[0] = tempRow;
                        end[1] = tempCol;
                        mazeMapper[tempRow][tempCol] = mazeMapper[row][col] + 1;
                        status = true;
                        break;
                    }
                    mazeMapper[tempRow][tempCol] = mazeMapper[row][col] + 1;
                    enqueue(&headPtr, &tailPtr, tempRow);
                    enqueue(&headPtr, &tailPtr, tempCol);
                }
            }
            // Optional command to check the values in queue.
            // printQueue(headPtr); 
        }
    }

    // The section that I have purposely comment out details my stack implementation to reverse from target to start 
    // and print from start to finish, but please note as specified in the forum by Ahmet that if you don't need a stack
    // then you don't have to use one. Hence, to make my code efficient I will not implement a stack.

    // VARIABLE AND STACK DEFINITIONS:
    // StackNodePtr stackPtr = NULL;

    // INITIALISE STACK WITH ENDPOINT:
    // push(&stackPtr, end[1]); // # Un-comment this if you want to run stack mode.
    // push(&stackPtr, end[0]); // # Un-comment this if you want to run stack mode.
    printf("%d,", end[0]+1); // # Comment this if you want to run stack mode.
    printf("%d ", end[1]+1); // # Comment this if you want to run stack mode.
    
    bool backStatus = false;
    int rowBack, colBack, tempRowBack, tempColBack;

    rowBack = end[0];
    colBack = end[1];
        

    // LOOP TO TRACE BACK THE STEPS TO REACH THE START USING STACKS:
    while(backStatus == false && ultStackStat == false){
        for(int iMov2 = 0; iMov2 < 4; iMov2++){
            tempRowBack = rowBack + rowMove[iMov2]; 
            tempColBack = colBack + colMove[iMov2];
            // Continue finding the shortest path through the maze.
            if(mazeMapper[tempRowBack][tempColBack] > 0 && mazeMapper[tempRowBack][tempColBack] < mazeMapper[rowBack][colBack]){
                rowBack = tempRowBack;
                colBack = tempColBack;
                // push(&stackPtr, tempColBack); // # Un-comment this if you want to run stack mode.
                // push(&stackPtr, tempRowBack); // # Un-comment this if you want to run stack mode.
                printf("%d,", tempRowBack+1); // # Comment this if you want to run stack mode.
                printf("%d ", tempColBack+1); // # Comment this if you want to run stack mode.
                break;
            }
            // Break the command when reaching the starting point.
            else if(mazeMapper[tempRowBack][tempColBack] == 0){
                // push(&stackPtr, tempColBack); // # Un-comment this if you want to run stack mode.
                // push(&stackPtr, tempRowBack); // # Un-comment this if you want to run stack mode.
                printf("%d,", tempRowBack+1); // # Comment this if you want to run stack mode.
                printf("%d ", tempColBack+1); // # Comment this if you want to run stack mode.
                backStatus = true;
                break;
            }
        }
        // Optional command to check values in stack:
        // printStack(stackPtr);
    }
    
    // # Un-comment this if you want to run stack mode (print either way is stated.)
    // PRINTING THE FINAL OUTPUT FROM T TO S:
    /*
    int myCount = 0;
    int *array = (int*) malloc(SIZE * SIZE * sizeof(int));
    while(!isEmptyStack(stackPtr) && ultStackStat == false){
        array[myCount] = pop(&stackPtr)+1;
        myCount++;
    }

    for(int g = myCount-1; g >= 1; g-=2){
        printf("%d,", array[g-1]);
        printf("%d ", array[g]);
    }
    printf("\n");
    */

    // PRINTING THE FINAL OUTPUT FROM S to T:
    /*
    while(!isEmptyStack(stackPtr) && ultStackStat == false){
        printf("%d,", pop(&stackPtr)+1);
        printf("%d ", pop(&stackPtr)+1);  
    }
    printf("\n");
    */
    
    free(mazeMapper);
    fclose(filePtr);
    return 0;
}
