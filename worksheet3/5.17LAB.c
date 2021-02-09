/*
 * Code by Matthew Zuberbuhler & Sergio Gutierrez 
 */
#include <stdio.h>

int main(void) {
	const int NUM_ELEMENTS = 9;
	int userValues[NUM_ELEMENTS];    // Set of data specified by the user
	int count = 0;

	for (int i = 0; i < 20; i++) 
	{
		count++;
		scanf("%i", &userValues[i]);
		if(userValues[i] < 0 && i < 10) 
		{
			count--;
			break;
		}
		
		if (i > 9) {
		   count = 10;
		   break;
		   }
	}
	//printf("%i", count);
	if (count < 10) 
	{
	   printf("Middle item: %i\n", userValues[count / 2]);
   } 
   else
   {
      printf("Too many numbers\n");
   }

	return 0;
}