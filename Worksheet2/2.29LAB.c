/*
 * Code by Matthew Zuberbuhler & Sergio Gutierrez 
 */
#include <stdio.h>
#include <math.h>

int main(void) {
   double x;
   double y;
   double z;

   scanf("%lf %lf %lf", &x, &y, &z);
   
   printf("%0.2lf %0.2lf %0.2lf %0.2lf\n",
      pow(x, z),
      pow(x, pow(y, 2)),
      fabs(y),
      sqrt(pow(x * y, z)));
   return 0;
}
