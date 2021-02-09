/*
 * Code by Matthew Zuberbuhler & Sergio Gutierrez 
 */
#include <stdio.h>

int main(void) {
	const int NUM_ELEMENTS = 20;         // Number of input integers
	int userVals[NUM_ELEMENTS], small1, small2;

	int count;

	scanf("%i", &count);
	//printf("Count: %i\n", count);

	for (int i = 0; i < count; i++) {
		scanf("%i", &userVals[i]);
	}

	if(count == 2) {
		small1 = userVals[0];
		small2 = userVals[1];
		if (small1 < small2) 
		{
			printf("%i and %i\n", small1, small2);
		} 
		else 
		{
			printf("%i and %i\n", small2, small1);
		}
	}
	else 
	{

		int flag = 0;
		small1 = userVals[0];
		small2 = userVals[1];
		for(int i = 1; i < count; i++) 
		{
			if(small1 >= userVals[i] || small2 >= userVals[i]){
				if(small2 >= userVals[i] && small1 <= userVals[i]) 
				{
					small2 = userVals[i];
					continue;
				}
				if(flag == 0) {
					small1 = userVals[i];
					flag++;
				}
				if(flag == 1) 
				{
					small2 = small1;
					small1 = userVals[i]; 
				}

			}
		}
		printf("%i and %i\n", small1, small2);
	}

	return 0;
}