/*
 * Code by Matthew Zuberbuhler & Sergio Gutierrez 
 */
#include <stdio.h>

int main(void) {

   int x;
   scanf("%i", &x);
   while(x > 0) 
   {
      printf("%i", x % 2);
      x /= 2;
   }
   printf("\n");
   return 0;
}
