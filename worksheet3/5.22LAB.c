/*
 * Code by Matthew Zuberbuhler & Sergio Gutierrez 
 */
#include <stdio.h>

int main(void) {
	const int NUM_ELEMENTS = 20;         // Number of input integers
	int userVals[NUM_ELEMENTS], left, right;

	int count;

	scanf("%i", &count);
	//printf("Count: %i\n", count);

	for (int i = 0; i < count; i++) {
		scanf("%i", &userVals[i]);
	}
	//printf("Array contains:\n");
	//for (int i = 0; i < count; i++) {
	//	printf("%i ", userVals[i]);
	//}

	scanf("%i %i", &left, &right);
	//printf("Left: %i, Right: %i\n", left, right);

	for(int i = 0; i < count; i++) 
	{
		if(userVals[i] >= left && userVals[i] <= right) 
		{
			printf("%i,", userVals[i]);
		}
	}

	printf("\n");

	return 0;
}