/*
 * Code by Matthew Zuberbuhler & Sergio Gutierrez 
 */
#include <stdio.h>

int main(void) {
	const int NUM_ELEMENTS = 20;         // Number of input integers
	int userVals[NUM_ELEMENTS];          // Array to hold the user's input integers
	int count;

	scanf("%i", &count);

	for (int i = 0; i < count + 1; i++) {
		scanf("%i", &userVals[i]);
	}
	
	int thresh = userVals[count];

	for(int i = 0; i < count; i++) 
	{
		if(userVals[i] < thresh) 
		{
			printf("%i,", userVals[i]);
		}
	}
	
	printf("\n");

	return 0;
}
