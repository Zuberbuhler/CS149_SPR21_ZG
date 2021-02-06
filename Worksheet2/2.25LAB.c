/*
 * Code by Matthew Zuberbuhler & Sergio Gutierrez 
 */
#include <stdio.h>

int main(void) {

   int userNum; // dividend
   int x; // divisor
   scanf("%d %d", &userNum, &x);

   for (int i = 0; i < 2; i++) // divide the dividend three times
   {
      userNum = userNum / x;
      printf("%d ", userNum);
   }
    printf("%d", userNum / x);
   printf("\n");
   
   return 0;
}



