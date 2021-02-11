/*
 * Code by Matthew Zuberbuhler & Sergio Gutierrez 
 */
#include <stdio.h>

double MilesToLaps(double userMiles)
{
   return userMiles / 0.25;
}

int main(void) {

   double miles;
   scanf("%lf", &miles);
   printf("%0.2lf\n", MilesToLaps(miles));

   return 0;
}
