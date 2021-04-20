#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

typedef struct TRACE_NODE_STRUCT {
    char* functionid;                         // ptr to function identifier
    struct TRACE_NODE_STRUCT* next;           // ptr to next frama
} TRACE_NODE;

typedef struct command_struct {
    char* command;
    int index;
    struct command_struct* nextCommandPtr;
} CommandNode;

void CreateCommandNode(CommandNode* thisNode, char* cmd, int ind, CommandNode* nextCmd);
void InsertCommandAfter(CommandNode* thisNode, CommandNode* newNode);
void PrintNodes(CommandNode* node);
CommandNode* GetNextCommand(CommandNode* thisNode);
void FreeNodes(CommandNode* node);
// Remove the top of the stack
void POP_TRACE();

// Push p on the stack
void PUSH_TRACE(char* p);

/* function PRINT_TRACE prints out the sequence of function calls that are on the stack at this instance */
/* For example, it returns a string that looks like: funcA:funcB:funcC.  */
char* PRINT_TRACE();

void FreeTraceTop();


static TRACE_NODE* TRACE_TOP = NULL;

/* --------------------------------*/
// Push p on the stack
void PUSH_TRACE(char* p)
{
    TRACE_NODE* tnode;
    static char glob[] = "global";

    if (TRACE_TOP == NULL) {

        // Initialize the stack with "global" identifier
        TRACE_TOP = (TRACE_NODE*) malloc(sizeof(TRACE_NODE));

        // Print msg if malloc fails
        if (TRACE_TOP == NULL) {
            printf("PUSH_TRACE: memory allocation error\n");
            exit(1);
        }

        TRACE_TOP->functionid = glob;
        TRACE_TOP->next = NULL;
    }

    // Create the node for p
    tnode = (TRACE_NODE*) malloc(sizeof(TRACE_NODE));

    // Print msg if malloc fails
    if (tnode == NULL) {
        printf("PUSH_TRACE: memory allocation error\n");
        exit(1);
    }

    tnode->functionid = p;
    tnode->next = TRACE_TOP;    // insert tnode as the first in the list
    TRACE_TOP = tnode;          // point TRACE_TOP to the first node
}

/* --------------------------------*/
// Remove the top of the stack
void POP_TRACE() {
    TRACE_NODE* tnode = TRACE_TOP;
    TRACE_TOP = tnode->next;
    free(tnode);
}

/* ---------------------------------------------- */
/* function PRINT_TRACE prints out the sequence of function calls that are on the stack at this instance */
/* For example, it returns a string that looks like: funcA:funcB:funcC.  */
char* PRINT_TRACE() {
    int depth = 50; //A max of 50 levels in the stack will be combined in a string for printing out.
    int i, length, j;
    TRACE_NODE* tnode;
    static char buf[100];

    // Stack not initialized yet, so we are still in the 'global' area
    if (TRACE_TOP == NULL) {
        strcpy(buf,"global");
        return buf;
    }

    /* peek at the depth top entries on the stack, but do not
       go over 100 chars and do not go over the bottom of the
       stack */
    sprintf(buf,"%s", TRACE_TOP->functionid);
    length = strlen(buf);   // length of the string so far

    for(i=1, tnode = TRACE_TOP->next; tnode != NULL && i < depth; i++, tnode = tnode->next) {
        // Length of what we want to add
        j = strlen(tnode->functionid);

        // Total length is ok
        if (length + j + 1 < 100) {
            sprintf(buf + length, ":%s", tnode->functionid);
            length += j + 1;
        }
        else
            break;
    }
    return buf;
}

// Frees TRACE_TOP
void FreeTraceTop() {
    free(TRACE_TOP);
}

void CreateCommandNode(CommandNode* thisNode, char* cmd, int ind, CommandNode* nextCmd) {
    // Copy cmd into thisNodes's command
    thisNode->command = cmd;
    thisNode->index = ind;
    thisNode->nextCommandPtr = nextCmd;
    return;
}

// Insert node newNode after thisNode
void InsertCommandAfter(CommandNode* thisNode, CommandNode* newNode) {
    CommandNode* tmpNext = NULL;

    tmpNext = thisNode->nextCommandPtr;
    thisNode->nextCommandPtr = newNode;
    newNode->nextCommandPtr = tmpNext;
    return;
}

// Recursively print contents of linkedlist
void PrintNodes(CommandNode* node) {
    if (node == NULL) {
        return;
    }

    fprintf(stdout,"Index: %d\tCommand: %s\n", node->index, node->command);
    PrintNodes(GetNextCommand(node));
}

// Free the linked list
void FreeNodes(CommandNode* node) {
    CommandNode* nextNode = GetNextCommand(node);
    CommandNode* currNode = node;

    // Traverse through the linkedlist
    while (currNode != NULL) {
        free(currNode);

        // Set nextNode and currNode
        if (nextNode != NULL) {
            CommandNode* tempNode = nextNode;
            nextNode = GetNextCommand(nextNode);
            currNode = tempNode;
        }

        // Set currNode to NULL if nextNode is NULL
        // nextNode being NULL indicates that currNode is the last node
        else {
            currNode = NULL;
        }
    }
}

// Get next command node in linked list
CommandNode* GetNextCommand(CommandNode* thisNode) {
    return thisNode->nextCommandPtr;
}

void* REALLOC(void* p, int newSize, char* file, int line) {
    p = realloc(p, newSize);
    fprintf(stdout,"File %s, line %d, function %s reallocated the memory segment at address %p to a new size %d\n", file, line, PRINT_TRACE(), p, newSize);
    return p;
}

// -------------------------------------------
// function MALLOC calls malloc
// For instance, example of print out:
// "File tracemem.c, line X, function F allocated new memory segment at address A to size S"
// Information about the function F should be printed by printing the stack (use PRINT_TRACE)
void* MALLOC(int size, char* file, int line) {
    void* p = malloc(size);
    fprintf(stdout,"File %s, line %d, function %s allocated new memory segment at address %p to size %d\n", file, line, PRINT_TRACE(), p, size);
    return p;
}

// ----------------------------------------------
// function FREE calls free
// For instance, example of print out:
// "File tracemem.c, line X, function F deallocated the memory segment at address A"
// Information about the function F should be printed by printing the stack (use PRINT_TRACE)
void FREE(void* p,char* file,int line) {
    free(p);
    fprintf(stdout,"File %s, line %d, function %s deallocated the memory segment at address %p\n", file, line, PRINT_TRACE(), p);
}

#define realloc(a,b) REALLOC(a,b,__FILE__,__LINE__)
#define malloc(a) MALLOC(a,__FILE__,__LINE__)
#define free(a) FREE(a,__FILE__,__LINE__)

// -----------------------------------------
// Add an extra column to char** fileCommands.
char** add_columns(char** array, int rows, int columns) {
    PUSH_TRACE("add_columns");

    array = (char**) realloc(array, sizeof(char*) * rows);

    for(int i = 0; i < rows; i++) {
        array[i] = (char*) realloc(array[i],sizeof(char) * (columns + 1));
    }

    POP_TRACE();
    return array;
}

// -----------------------------------------
// Add an extra row to char** fileCommands
char** add_row(char** array, int rows, int columns) {
    PUSH_TRACE("add_row");

    array = (char**) realloc(array, sizeof(char*) * rows);

    // Reallocate array memory
    for(int i = 0; i < rows; i++) {
        array[i] = (char*) realloc(array[i], sizeof(char) * columns);
    }

    POP_TRACE();
    return array;
}

// ------------------------------------------
// Allocates memory for the char** array
char** allocate_array_mem(char** array, int rows, int columns) {
    PUSH_TRACE("allocate_array_mem");

    // Make char** array
    array = (char**) malloc(sizeof(char*) * rows);

    // Allocate array memory
    for(int i = 0; i < rows; i++) {
        array[i] = (char*) malloc(sizeof(char) * columns);
    }

    POP_TRACE();
    return array;
}

int main(int argc, char *argv[]) {
    if (argc > 2 || argc <= 1) {
        printf("Number of files inputted must be 1\n");
        exit(EXIT_FAILURE);
    }

    // Initialize variables for reading file
    FILE * fp;
    char * line = NULL;
    size_t length = 0;
    ssize_t read;

    // Open memtrace.out file and redirect stdout to memtrace.out
    int outFile = open("memtrace.out", O_RDWR | O_CREAT | O_APPEND);
    chmod("memtrace.out", S_IRWXU);
    dup2(outFile, STDOUT_FILENO);

    // Initialize a char** array of 10 rows and 20 columns
    // William stated to assume string commands are of length 20 max
    char** fileCommands = NULL;
    int rows = 10, columns = 20;
    fileCommands = allocate_array_mem(fileCommands, rows, columns);
    int index = 0;

    // Initialize the linkedlist and necessary variables to help create the list
    CommandNode *head = NULL;
    CommandNode *currNode = NULL;
    CommandNode *prevNode = (CommandNode*) malloc(sizeof(CommandNode));
    free(prevNode);

    // Open the file specified by the terminal
    fp = fopen(argv[1], "r");

    // Exit program if the file couldn't be opened
    if (fp == NULL) {
        printf("File can't be opened\n");
        exit(EXIT_FAILURE);
    }

    // Read each line of the file
    while ((read = getline(&line, &length, fp)) != -1) {

        // Check if command string is bigger than the size of the row
        if (strlen(line) > columns) {
            columns = strlen(line);
            fileCommands = add_columns(fileCommands, rows, columns);
        }

        // Check if there is more rows needed
        if (index == rows) {
            rows += 1;
            fileCommands = add_row(fileCommands, rows, columns);
        }

        // Insert command read from file into char** array
        strcpy(fileCommands[index], line);

        // Create linkedlist
        currNode = (CommandNode*) malloc(sizeof(CommandNode));
        CreateCommandNode(currNode, fileCommands[index], index, NULL);
        InsertCommandAfter(prevNode, currNode);
        prevNode = currNode;
        if (index == 0) {
            head = currNode;
        }

        // Print linkedlist to determine if linkedlist contains correct elements
        PrintNodes(head);

        index += 1;
    }

    // Deallocate the array
    for(int i = 0; i < rows; ++i)
        free(fileCommands[i]);
    free(fileCommands);

    // Deallocate the linkedlist
    FreeNodes(head);

    // Deallocate line
    free(line);

    // Deallocate TRACE_TOP from Trace_Node.c
    FreeTraceTop();

    // Close file handler
    fclose(fp);

    return 0;
}