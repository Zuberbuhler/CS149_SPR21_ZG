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

int containsLetter(char * strToDouble) {
    for(int i = 0; i < strlen(strToDouble); i++){
        if((strToDouble[i] < 48 //not numbers
        || strToDouble[i] > 57) //not numbers
        && strToDouble[i] != 46) //the period '.'
        {
            return 1; //it does contain a letter
        }
    }
    return 0; //if only contains numbers
}

int main(int argc, char **argv) {
    for(int i = 0; i < argc; i++)
    {
        printf("arg[%i] = %s\n", i, argv[i]);
    }
    char *tmpPtr;

    if(argc < 3)
    {
        return 1; //too little arguments
    }
    else
    {
        //we need to see if all of the files names open a file:
        FILE * fp;
        for(int i = 1; i < argc - 1; i++)
        {
            fp = fopen(argv[i], "r");
            if(!fp)
            {
                fclose(fp);
                printf("range: cannot open file\n");
                return 1; //file failed to open.

            }
            fclose(fp); //else keep checking until all filenames are tested
        }
        //we need to see if a number has been provided as the last arg:
        if (containsLetter(argv[argc - 1]) == 1)
        {
            printf("Power argument is not a number!\n");
            return 1;
        }
        double exponent = strtod(argv[argc-1], &tmpPtr);

        //at this point we can start forking process
        //we need to fork main process 1 time per filename arg
        //that will give us one process per filename
        int id1 = 0, id2 = 0, mpid = getpid();
        printf("main process pid: %i\n", getpid());
        if(argc == 3)
        {
            printf("There are only 3 arguments!\n");
            //do a normal summation with one process
            fp = fopen(argv[(int)(getpid() - mpid + 1)], "r");
            printf("opening %s\n", argv[(int)(getpid() - mpid + 1)]);
            double tmp = 0, result = 0;
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
            printf("Final Result: %0.2lf\n", result);
        }
        else
        {
            int fd[2];
            if(pipe(fd) == -1)
            {
                printf("An error occurred opening the pipe\n");
                return 1;
            }
            int numOfProc = 1;
            id1 = fork();

            numOfProc++;
            if(id1 == -1)
            {
                printf("fork failed!\n");
                return 1;
            }
            while (numOfProc < argc - 2 && id1 != 0)
            {
                id2 = fork();
                if (id2 == -1)
                {
                    printf("Fork failed");
                    return 1;
                }
                numOfProc++;
                if(id2 == 0){
                    break;
                }
            }
            fp = fopen(argv[(int)(getpid() - mpid + 1)], "r");
            printf("opening %s\n", argv[(int)(getpid() - mpid + 1)]);
            double tmp = 0, result = 0;
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

            if(getpid() == mpid)
            {
                double readTmp = 0;
                for (int i = 0; i < numOfProc - 1; i++)
                //numOfProc - 1 is number of child processes b/c we use main process to read 1 of the files
                {
                    close(fd[1]);
                    printf("Waiting for a child process to end. waitCode: %i\n", wait(NULL));
                    read(fd[0], &readTmp, sizeof(double));
                    //print the number read from pipe and the number of items read
                    printf("readTmp#%i: %0.2lf\n", i+ 1, readTmp);
                    close(fd[0]);
                    result += readTmp;
                }
                printf("Final Result = %0.2lf\n", result);
            }
            else
            {
                close(fd[0]); // we are not reading from child;
                write(fd[1], &result, sizeof(double));
                close(fd[1]);
            }


//        if(getpid() == mpid) {
//            while (wait(NULL) != -1 || errno != ECHILD) {
//                printf("Waiting for a child to finish!\n");
//            }
//        }
        }

        printf("hello from pid: %i, ppid: %i\n",
               getpid(), getppid());



        return 0;
    }
}
