/* Program to demonstrate time taken by function fun() */
#include <stdio.h>
#include <time.h>

// A function that terminates when enter key is pressed
void fun()
{
    printf("fun() starts \n");
    printf("Press enter to stop fun \n");
    while(1)
    {
        if (getchar())
            break;
    }
    printf("fun() ends \n");
}

// The main program calls fun() and measures time taken by fun()
int main()
{
    // Calculate the time taken by fun()
    struct timespec start, finish;
    double elapsed = 0;

    clock_t t;
    t = clock();
    clock_gettime(CLOCK_MONOTONIC, &start);
    printf("start %ld\n", start.tv_nsec);
    fun();

    clock_gettime(CLOCK_MONOTONIC, &finish);
    printf("finish %ld\n", finish.tv_nsec);

    elapsed = ((double)finish.tv_sec + 1.0e-9*finish.tv_nsec) -
                ((double)start.tv_sec + 1.0e-9*start.tv_nsec);
    if(elapsed <= 2)
    {
        printf("less than 2 seconds!!!\n");
    }
    printf("fun() took %.5f seconds to execute \n", elapsed);
    printf("fun() took about %.5f seconds\n",
           ((double)finish.tv_sec + 1.0e-9*finish.tv_nsec) -
           ((double)start.tv_sec + 1.0e-9*start.tv_nsec));

    return 0;
}
