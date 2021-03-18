/*
 * Code by Matthew Zuberbuhler & Sergio Gutierrez 
 */
/* 
    run by type the following in terminal:
    1.      gcc summation_many.c -o main
    2.      ./main numbers1.txt numbers2.txt 1  
    //test with multiple files and no files and empty files
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>
//#include <errno.h>

// for testing the exponent provided by user
// returns 1 if exponent argument is not a number; has no letters/special symbols
int containsLetter(char * strToDouble) {
    int periodCount = 0;
    for(int i = 0; i < strlen(strToDouble); i++){
        if((strToDouble[i] < 48 // not numbers
        || strToDouble[i] > 57) // not numbers
        && strToDouble[i] != 46 // the period '.'
        && periodCount < 2)     // at most 1 '.'
        {
            return 1; //contains a letter
        }
        if(strToDouble[i] == 46)
        {
            periodCount++;
        }
    }
    return 0; //only contains numbers
}

// argc holds amount of arguments provided by user + 1
// argv is an array of strings that holds the arguments
int main(int argc, char **argv) {
    // prints all the arguments provided by user + 1
    /*
    for(int i = 0; i < argc; i++)
    {
        printf("arg[%i] = %s\n", i, argv[i]);
    }
    */

    if(argc < 3) // too little arguments
    {
        printf("Provide at least one filename at one exponent.\n");
        return 0;
    }
    else 
    {
        // Check if all filenames open a file successfully:
        FILE *fp;
        for (int i = 1; i < argc - 1; i++) {
            fp = fopen(argv[i], "r");
            if (!fp) {
                fclose(fp);
                printf("range: cannot open file\n");
                return 1; // file failed to open.

            }
            fclose(fp); // else keep checking until all filenames are tested
        }

        // Check if last argument is a number for exponent
        if (containsLetter(argv[argc - 1]) == 1) {
            printf("Power argument is not a number!\n");
            return 1;
        }
        // We have checked all cases for error, now time to perform summation_many

        int id1 = 0, fd[2], numOfChildren = 0;
        double exponent = strtod(argv[argc - 1], NULL);
        double tmp = 0, result = 0;

        // assign file descriptors with pipe
        if (pipe(fd) == -1) {
            printf("An error occurred opening the pipe\n");
            return 1;
        }

        // we are only making children of main process
        // NOT children of children, etc.
        //1 filename = argc = 3
        //to make 1 child:  argc - 2 = 1 thus while condition
        while (numOfChildren < argc - 2) // only main process performs loop
        {
            id1 = fork();
            if (id1 == -1) {
                printf("Fork failed");
                return 1;
            }

            numOfChildren++; //use this is map to filename arguments

                // all child process open their own file: mapped with (numOfChildren)
            if (id1 == 0)
            {
                // Each child process reads the numbers in their file
                // and creates partial summation
                // stored in result
                fp = fopen(argv[numOfChildren], "r");

                for(int i = 0; fscanf(fp, "%lf", &tmp) != EOF; i++)
                {
                    if(exponent == 1) {
                        result += tmp;
                    }
                    else
                    {
                        result += pow(tmp, exponent);
                    }
                }

                //printf("**result of pid[%i] and ppid[%i]: %0.2lf\n", getpid(), getppid(), result);
            }
            //printf("opening %s\n", argv[(int)(getpid() - mpid + 1)]);

            // main process reads from pipe after waiting for each child
            if(id1 != 0)
            {
                double readTmp = 0;
                close(fd[1]); //main process doesn't write

                // numOfProc - 1 is number of child processes
                // b/c we use main process to read 1 of the files
                //printf("result before partial sums: %0.2lf\n", result);
                // for (int i = 0; i < numOfChildren; i++)
                // {
                //     wait(NULL);

                //     read(fd[0], &readTmp, sizeof(double)); //read in from pipe

                //     // print the number read from pipe and the number of items read
                //     //printf("Result(%0.2lf) + readTmp(%0.2lf): ", result, readTmp);
                //     result += readTmp;
                //     //printf("%0.2lf\n", result);
                // }

                    while(wait(NULL) > 0);

                    for (int i = 0; i < numOfChildren; i++) 
                    {
                        read(fd[0], &readTmp, sizeof(double)); //read in from pipe
                        // print the number read from pipe and the number of items read
                        printf("Result(%0.2lf) + readTmp(%0.2lf): ", result, readTmp);
                        result += readTmp;
                        //printf("%0.2lf\n", result);
                    }

                close(fd[0]);

                printf("%0.2lf\n", result);
            }
            else // child processes write to pipe and return when finished
            {
                //printf("process %i, writing %0.2lf\n", getpid(), result);
                close(fd[0]); // we are not reading from child;

                while(write(fd[1], &result, sizeof(double)) < 0); // each child writes their partial sum to pipe
                
                close(fd[1]);
                return 0;
            }

            if (id1 == 0) // all children leave loop after created
            {
                break;
            }
        }
        return 0;
    }
}