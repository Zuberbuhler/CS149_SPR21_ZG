/*
 * Code by Matthew Zuberbuhler & Sergio Gutierrez 
 */
#include <stdio.h>

int main(void) {
   int currentPrice;
   int lastMonthsPrice;
   
   scanf("%d", &currentPrice);
   scanf("%d", &lastMonthsPrice);
   
   printf(
      "This house is $%i. The change is $%i since last month.\n"
      "The estimated monthly mortgage is $%lf.\n",
      currentPrice,
      currentPrice - lastMonthsPrice,
      ((((float) currentPrice) * 0.051) / 12)
      );

   return 0;
}
