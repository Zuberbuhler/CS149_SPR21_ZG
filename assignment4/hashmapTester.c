//// Created on 3/17/21.
// Sample solution//#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

struct nlist
{
    /* table entry: */
    struct nlist *next; /* next entry in chain */
    // char *name;  /* defined name, can remove */
    // char *defn; /* replacement text, can remove */

    /* starttime and finishtime */
    struct timespec starttime, finishtime;

    /* index // this is the line index in the input text file */
    int index;
    /*
        pid  // the process id. you can use the pid result of
                wait to lookup in the hashtable
    */
    int pid;
    /*
    char *command; // command. This is good to store for when you
                               decide to restart a command
    */
    char* command;
};

#define HASHSIZE 101

static struct nlist *hashtab[HASHSIZE]; /* pointer table */

/* This is the hash function: form hash value for string s */
/* TODO change to: unsigned hash(int pid) */
/* TODO modify to hash by pid . */

/* You can use a simple hash function: pid % HASHSIZE */
unsigned hash(int pid)
{
    printf("Hash: return\n");
    return pid % HASHSIZE;
}

/* lookup: look for s in hashtab */
/* TODO change to lookup by pid: struct nlist *lookup(int pid) */
/* TODO modify to search by pid, you won't need strcmp anymore */
/*
This is traversing the linked list under a slot of the hash
table. The array position to look in is returned by the hash function
*/

struct nlist *lookup(int pid)
{
    printf("Lookup: Start\n");
    struct nlist *np;
    for (np = hashtab[hash(pid)]; np != NULL; np = np->next)
    {
        if (np->pid == pid)
        {
            printf("Lookup: found pid in slot already!\n");
            return np; /* found */
        }

    }
    printf("Lookup: End\n");
    return NULL; /* not found */
}

/* insert: put (name, defn) in hashtab */

/* TODO: change this to insert in hash table the info for a
new pid and its command */

/* TODO: change signature to:
	struct nlist *insert(char *command,int pid, int index). */

/* This insert returns a nlist node.
Thus when you call insert inyour main function  */

/* you will save the returned nlist node in a variable (mynode). */

/* Then you can set the starttime and finishtime from your main function: */

/* mynode->starttime = starttime; mynode->finishtime = finishtime; */

//char *name, char *defn
struct nlist *insert(char *command, int pid, int index)
{
    printf("Inserting Start:\n");
    struct nlist *np;
    unsigned hashval;
    //TODO change to lookup by pid. There are 2 cases:
    if ((np = lookup(pid)) == NULL)
    {
        printf("Inserting: case1 creating new node in slot np\n");
        /* case 1: the pid is not
        found, so you have to create it with malloc.
        Then you want to set the pid, command and index */
        np = (struct nlist *) malloc(sizeof(*np));
        if (np == NULL || ((np->command = strdup(command)) == NULL))
        {
            printf("Inserting: Failed to malloc np\n");
            return NULL;
        }
        np->pid = pid;
        np->index = index;

        hashval = hash(pid);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else
    {
        printf("Inserting: Case2 np found in slot already\n");
        /* case 2: the pid is already there in the hashslot,
        i.e. lookup found the pid. In this case you can either
        do nothing, or you may want to set again the command
        and index (depends on your implementation). */
        free((void *) np->command); /*free previous command */
        if ((np->command = strdup(command)) == NULL)
        {
            printf("Inserting: updating np->command failed\n");
            return NULL;
        }
        np->index = index;
    }


    printf("Inserting: End\n");
    return np;
}

/** You might need to duplicate the command string to ensure
you don't overwrite the previous command each time a new line
is read from the input file. Or you might not need to duplicate it.
It depends on your implementation. **/

char* strduplicate(char *s) /* make a duplicate of s */
{
    printf("strduplicate: Start\n");
    char *p;
    p = (char *) malloc(strlen(s)+1); /* +1 for \0 */

    if (p != NULL)
    {
        strcpy(p, s);
    }

    printf("strduplicate: End\n");
    return p;
}

int main() {
    char * command = "ping -c 3 google.com";
    int pid1 = 101;
    int index1 = 1, index2;

    struct nlist* myNode1 = insert(command, pid1, index1);

    struct nlist* myNode2 = insert(command, pid1, index2);
    if(myNode2 != NULL) {
        printf("myNode2->pid = %i\n"
               "myNode2->index = %i\n"
               "myNode2->command = %s\n\n",
               myNode2->pid, myNode2->index, myNode2->command);
    }

    printf("myNode1->pid = %i\n"
           "myNode1->index = %i\n"
           "myNode1->command = %s\n",
           myNode1->pid, myNode1->index, myNode1->command);




    return 0;
}

