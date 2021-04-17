#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// A linked list node
struct Node
{
    struct Node *next; /* next entry in chain */

    /* starttime and finishtime */
    struct timespec start;
    struct timespec finish;

    int index;
    int pid;
    char* command;
};

struct Node *lookupPID(struct Node *head, int pid)
{
    struct Node *np  = head;
    for (; np != NULL; np = np->next)
    {
        if (np->pid == pid)
        {
            return np; /* found */
        }
    }
    return NULL; /* not found */
}

char* strduplicate(char *s) /* make a duplicate of s */
{
    char *p = (char *) malloc(strlen(s)+1); /* +1 for \0 */
    if (p != NULL)
    {
        strcpy(p, s);
    }

    return p;
}

struct Node* insert(struct Node **head, char *command, int pid, int index)
{
    struct Node *np;
    if(*head == NULL) {
        np = (struct Node *) malloc(sizeof(struct Node));
        if (np == NULL || ((np->command = strduplicate(command)) == NULL))
        {
            return NULL;
        }
        np->pid = pid;
        np->index = index;
        np->next = NULL;
        *head = np;
        return np;
    }

    if ((np = lookupPID(*head, pid)) == NULL)
    {
        struct Node *tmp;
        tmp = *head;
        np = (struct Node *) malloc(sizeof(struct Node));
        if (np == NULL || ((np->command = strduplicate(command)) == NULL))
        {
            return NULL;
        }
        np->pid = pid;
        np->index = index;
        np->next = NULL;
        while(tmp->next != NULL) {

            tmp = tmp->next;
        }
        tmp->next = np;
        //hashval = hash(pid);
        //np->next = hashtab[hashval];
        //hashtab[hashval] = np;
    } else
    {
        //printf("Inserting: Case2 np found in slot already-> ");
        /* case 2: the pid is already there in the hashslot,
        i.e. lookup found the pid. In this case you can either
        do nothing, or you may want to set again the command
        and index (depends on your implementation). */
        free((void *) np->command); /*free previous command */
        if ((np->command = strdup(command)) == NULL)
        {
            //printf("Inserting: updating np->command failed and return null\n");
            return NULL;
        }
        np->index = index;
    }


    //printf("Inserting: End\n");
    return np;
}

void printLinkList(struct Node * head, int pos)
{
    if(head == NULL)
    {
        printf("Provided Null Node\n");
    }
    else
    {
        struct Node  *tmp = head;

        printf("Printing Node: pos %d\n"
               "index = %d\n"
               "pid = %d\n"
               "command = _%s_\n", pos, tmp->index, tmp->pid, tmp->command);
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
               "\t\t\t  pid = %d\n"
               "\t\t\t  command = _%s_\n", node->index, node->pid, node->command);
    }
}

void freeLinkedList(struct Node * head)
{
    //since we are freeing the whole node we do not need
    //to worry about modifying original head
    struct Node* tmp;

    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }
    if(head == NULL)
    {
        printf("head == NULL\n");
    }
}


int main(int argc, char *argv[]) {
    printf("1\n");
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int numOfCommands = 0;

    // this is us making a static array to hold as many as 50 words each up to 49 characters long
    // we can make this dynamically allocated array by parsing through file first and finding
    // the longest word and the max amount of words
    fp = fopen(argv[1], "r");
    if (!fp)
    {
        fclose(fp);
        printf("range: cannot open file\n");
        return 1; // file failed to open.
    }

    while ((read = getline(&line, &len, fp)) != -1)
    {
        numOfCommands++;
        printf("size = %d\n", (int)read);
    }

    printf("number of commands: %d\n", numOfCommands);

    fclose(fp);

    char *strs[numOfCommands];

    fp = fopen(argv[1], "r");
    if (!fp)
    {
        fclose(fp);
        printf("range: cannot open file\n");
        return 1; // file failed to open.
    }

    int commandNum = 0;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        commandNum++;
        printf("strlen(\"%s\") = %d\n", line, (int)read);
        if(commandNum < numOfCommands)
        {
            strs[commandNum - 1] = malloc((int)read - 1);
            strncpy(strs[commandNum - 1], line, (int)read - 1);
        }
        else
        {

            strs[commandNum - 1] = malloc((int)read + 1);
            strncpy(strs[commandNum - 1], line, (int)read);
        }
        //printf("strs[%d] = %s\n", commandNum,strs[commandNum]);
    }
    for(int i = 0; i < numOfCommands; i++)
    {
        printf("strs[%d] = _%s_\n", i, strs[i]);
    }


    fclose(fp);

    printf("2\n");

    struct Node* head = NULL;

    //we need to pass the head by reference for the insert to work
    printf("3\n");

    for(int i = 0; i < numOfCommands; i++)
    {
        insert(&head, strs[i], i + 100, i + 1);
    }

    printLinkList(head, 1);

    printf("Looking up Node\n");
    printNode(lookupPID(head, 103));
    //
    freeLinkedList(head);

    return 0;
}
