/*
* Code by Matthew Zuberbuhler & Sergio Gutierrez
*/
#include <stdio.h>

int main(void) {
   int baseInt;
   int headInt;
   
   /* Type your code here. */
   scanf( "%i %i", &baseInt, &headInt);
   printf("    %d\n", headInt);
   printf("    %d%d\n", headInt, headInt);
   for(int i = 0; i < 4; i++) 
   {
      printf("%d", baseInt);
    }
   for(int i = 0; i < 3; i++) 
   {
      printf("%d", headInt);
   }
   printf("\n");
   
   for(int i = 0; i < 4; i++) 
   {
      printf("%d", baseInt);
    }
   for(int i = 0; i < 4; i++) 
   {
      printf("%d", headInt);
   }
   printf("\n");
   for(int i = 0; i < 4; i++) 
   {
      printf("%d", baseInt);
    }
   for(int i = 0; i < 3; i++) 
   {
      printf("%d", headInt);
   }
   printf("\n");
   printf("    %d%d\n", headInt, headInt);
   printf("    %d\n", headInt);
   return 0;
}
