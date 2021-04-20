#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>


// A linked list node
struct Node
{
    int index;
    struct Node *next; /* next entry in chain */
    char* command;
};

static struct Node* TRACE_TOP = NULL;

char* PRINT_TRACE()
{
    int depth = 50; //A max of 50 levels in the stack will be combined in a string for printing out.
    int i, length, j;
    struct Node* tnode;
    static char buf[100];

    if (TRACE_TOP==NULL) {     // stack not initialized yet, so we are
        strcpy(buf,"global");   // still in the `global' area
        return buf;
    }

    /* peek at the depth(50) top entries on the stack, but do not
       go over 100 chars and do not go over the bottom of the
       stack */

    sprintf(buf,"%s",TRACE_TOP->command);
    length = strlen(buf);                  // length of the string so far
    for(i=1, tnode=TRACE_TOP->next;
        tnode!=NULL && i < depth;
        i++,tnode=tnode->next) {
        j = strlen(tnode->command);             // length of what we want to add
        if (length+j+1 < 100) {              // total length is ok
            sprintf(buf+length,":%s",tnode->command);
            length += j+1;
        }else                                // it would be too long
            break;
    }
    return buf;
} /*end PRINT_TRACE*/

void* REALLOC(void* p,int t,char* file,int line)
{
    p = realloc(p,t);
    fprintf(stdout,"File %s, line %d, function %s reallocated the memory segment at address %p to a new size %d\n", file, line, PRINT_TRACE(), p, t);
    return p;
}

// -------------------------------------------
// function MALLOC calls malloc
// TODO MALLOC should also print info about memory usage.
// For instance, example of print out:
// "File tracemem.c, line X, function F allocated new memory segment at address A to size S"
// Information about the function F should be printed by printing the stack (use PRINT_TRACE)
void* MALLOC(int t,char* file,int line)
{
    void* p;
    p = malloc(t);
    fprintf(stdout,"File %s, line %d, function %s allocated new memory segment at address %p to size %d\n", file, line, PRINT_TRACE(), p, t);
    return p;
}

// ----------------------------------------------
// function FREE calls free
// TODO FREE should also print info about memory usage.
// For instance, example of print out:
// "File tracemem.c, line X, function F deallocated the memory segment at address A"
// Information about the function F should be printed by printing the stack (use PRINT_TRACE)
void FREE(void* p,char* file,int line)
{
    free(p);
    fprintf(stdout,"File %s, line %d, function %s deallocated the memory segment at address %p\n", file, line, PRINT_TRACE(), p);
}

#define realloc(a,b) REALLOC(a,b,__FILE__,__LINE__)
#define malloc(a) MALLOC(a,__FILE__,__LINE__)
#define free(a) FREE(a,__FILE__,__LINE__)

char* strduplicate(char *s) /* make a duplicate of s */
{
    printf("mallocing memory for string _%s_\n", s);
    char *p = (char *) malloc(sizeof(char) * (int)(strlen(s)+1)); /* +1 for \0 */
    if (p != NULL)
    {
        strcpy(p, s);
    }

    return p;
}

void append(struct Node **head, char *command, int index)
{
    struct Node *np;
    if(*head == NULL) {
        np = (struct Node *) malloc(sizeof(struct Node));
        if (np == NULL || ((np->command = strduplicate(command)) == NULL))
        {
            exit(1);
        }
        np->index = index;
        np->next = NULL;
        *head = np;
        return;
    }
    else {
        struct Node *tmp;
        tmp = *head;
        np = (struct Node *) malloc(sizeof(struct Node));
        if (np == NULL || ((np->command = strduplicate(command)) == NULL)) {
            exit(1);
        }
        np->index = index;
        np->next = NULL;
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        tmp->next = np;
    }
    return;
}

void printLinkList(struct Node * head, int pos)
{
    if(head == NULL)
    {
        printf("tail of List (^^^)\n");
    }
    else
    {
        struct Node  *tmp = head;

        printf("Printing Node: pos %d\n"
               "index = %d\n"
               "command = _%s_\n", pos, tmp->index, tmp->command);
        tmp = tmp->next;
        pos++;
        printLinkList(tmp, pos);
    }
}

void printNode(struct Node *node)
{
    if(node != NULL)
    {
        printf("PrintingNode: index = %d\n"
               "command = _%s_\n", node->index, node->command);
    }
}

void PUSH_TRACE(char* p)          // push p on the stack
{
    struct Node * tnode;
    static char glob[]="global";

    if (TRACE_TOP==NULL)
    {

        // initialize the stack with "global" identifier
        printf("malloc TRACE_TOP: with %s sizeof: %d\n", glob, (int)sizeof(struct Node));
        TRACE_TOP=(struct Node *) malloc(sizeof(struct Node));

        // no recovery needed if allocation failed, this is only
        // used in debugging, not in production
        if (TRACE_TOP==NULL) {
            printf("PUSH_TRACE: memory allocation error\n");
            exit(1);
        }

        if((TRACE_TOP->command = strduplicate(glob)) == NULL)
        {
            printf("strDuplicate failed\n");
        }
        //printf("TRACE_TOP->command = _%s_\n", TRACE_TOP->command);
        TRACE_TOP->next=NULL;
    }

    // create the node for p
    printf("malloc another link for TRACE_TOP with string: %s: sizeof: %d\n", p, (int)sizeof(struct Node));
    tnode = (struct Node *) malloc(sizeof(struct Node));

    // no recovery needed if allocation failed, this is only
    // used in debugging, not in production
    if (tnode==NULL) {
        printf("PUSH_TRACE: memory allocation error\n");
        exit(1);
    }

    if((tnode->command = strduplicate(p)) == NULL)
    {
        printf("strDuplicate failed\n");
    }
    tnode->next = TRACE_TOP;  // insert fnode as the first in the list
    TRACE_TOP=tnode;          // point TRACE_TOP to the first node

}/*end PUSH_TRACE*/

void POP_TRACE()
{
    struct Node* tnode;
    tnode = TRACE_TOP;
    TRACE_TOP = tnode->next;

    printf("freeing tnode->command = _%s_ and tnode itself\n", tnode->command);
    free(tnode->command);
    free(tnode);
    tnode->command = NULL;
    tnode = NULL;
}

void freeLinkedList(struct Node ** head)
{
    //since we are freeing the whole node we do not need
    //to worry about modifying original head
    struct Node* tmp;
    while (*head != NULL)
    {
        printf("freeing linkedList head->command: _%s_ and current head\n", (*head)->command);
        tmp = (*head);
        (*head) = tmp->next;
        free(tmp->command);
        free(tmp);
        tmp->command = NULL;
        tmp = NULL;
    }

    if(*head == NULL)
    {
        printf("head == NULL\n");
    }
}

void cpynStr(char ** dest, char * src, int size)
{
    //size does not include '\0'

    for(int i = 0; i < size; i++)
    {
        //printf("src[%d] = '%c'\n", i, src[i]);
        (*dest)[i] = src[i];
    }
    (*dest)[size + 1] = '\0';
    printf("\n");
}

void addLines(char *** strArr, int rows)
{
    PUSH_TRACE("reallocating more memory");
    printf("reallocating more space: %d\n", (int)(sizeof(char *) * (rows + 10)));
    (*strArr) = (char **) realloc((*strArr), (int)(sizeof(char *)) * (int)(rows + 10));
    POP_TRACE();
    //return strArr;
}

void reallocBestFit(char *** strArr, int numOfCommands)
{
    PUSH_TRACE("reallocating for best fit");
    printf("reallocating for perfect space: %d\n", (int)(sizeof(char *) * (numOfCommands)));
    (*strArr) = (char **) realloc((*strArr), (int)(sizeof(char *)) * (numOfCommands));
    POP_TRACE();
    //return strArr;
}

void createArray(char *** array, FILE ** fp, int * numOfCommands)
{
    PUSH_TRACE("make_extend_array");
    //variables for reading file
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    //starting row allocation
    int rows = 10;

    //allocated 10 rows to begin with
    printf("malloc initial array in createArray: %d\n", (int) (sizeof(char *) * rows));
    (*array) = (char **)malloc(sizeof(char *) * rows);

    //index is number of line command is found
    int index = 0;
    int commandNum = 0; //count of the commands

    //read lines until end of file
    while ((read = getline(&line, &len, *fp)) != -1)
    {
        commandNum++;
        //if we run out of rows allocated we need to reallocate more
        if(commandNum > rows)
        {
            addLines(array, rows); //allocates 10 more spaces in memory
            //we increase size 10 at a time.
            rows += 10;
            //set all newly allocated rows that are not malloced yet to NULL
            for(int i = commandNum; i < rows; i++)
            {
                (*array)[i] = NULL;
            }
        }
        //if command is last it does not include a '\n' at the end of it
        if(line[strlen(line) - 1] == '\n')
        {
            //allocate memory for string
            printf("malloc array[%d]: %s: %d\n", index, line, (int)(sizeof(char) * (int) strlen(line)));
            (*array)[index] = (char *)malloc(sizeof(char) * (int) strlen(line));
            //copy value of string into the allocated memory
            cpynStr((*array) + index, line, (((int)strlen(line)) - 1));
        }
        else
        {
            //allocate memory for string
            printf("malloc last array[%d]: %s: %d\n", index, line, (int)(sizeof(char) * (int) strlen(line)));
            (*array)[index] = (char *)malloc(sizeof(char) * (int)((strlen(line)) + 1));
            //copy value of string into the allocated memory

            printf("HERE\n");
            cpynStr((*array) + index, line, (int)(strlen(line)));
            //strcpy((**array) + index, line);
        }
        index++;
    }

    //condense size of array to perfectly match # of commands
    reallocBestFit(array, commandNum);
    rows = commandNum; //adjust size of rows to match

    //put value in variable passed by reference for use in main
    *numOfCommands = commandNum;
    POP_TRACE();

    //return array;
}

int main(int argc, char *argv[]) {
    //check arguments provided in command line
    if (argc < 2)
    {
        printf("Too few arguments\n");
        exit(0);
    }

    //open file with commands
    FILE * fp;
    fp = fopen(argv[1], "r");
    //catch if file descriptor fails
    if (!fp)
    {
        fclose(fp);
        printf("range: cannot open file\n");
        return 1; // file failed to open.
    }

    //create memtrace.out file
    int f_write = open("memtrace.out", O_RDWR | O_CREAT | O_TRUNC, 0777);

    //redirect stdout to memtrace.out
    dup2(f_write, fileno(stdout));

    //store result from createArray function
    int numOfCommands;
    char** strs;
    createArray(&strs, &fp, &numOfCommands);

    //at this point all commands are in the array of strings (char ** strs)
    //so we can close command file
    fclose(fp);

    //prepare to construct linked list
    struct Node* head = NULL;

    //we need to pass the head by reference for the append to work
    for(int i = 0; i < numOfCommands; i++)
    {
        append(&head, strs[i], i + 1);
    }

    //print linked list using recursion
    printf("Head of list(vvv)\n");
    printLinkList(head, 1);

    //free allocated memory in linked list
    freeLinkedList(&head);

    //free array of strings
    for (int i = 0; i < numOfCommands; i++)
    {
        printf("freeing strs[%d]: = _%s_: size of: %d\n", i, strs[i], (int)sizeof(strs[i]));
        free(strs[i]);
        strs[i] = NULL;
    }
    printf("freeing array\n");
    free(strs);
    strs = NULL;

    //close memtrace.out
    close(f_write);

    freeLinkedList(&TRACE_TOP);
    return 0;
}
