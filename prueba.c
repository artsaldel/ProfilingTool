#include <stdio.h>

void main()
{	
	int N = 10000;
	float contador1 = 0.0;
	float contador2 = 0.0;
	for (int i = 0; i < N; i++)
	{
		contador1 = contador1 + 1.0;
		for (int j = 0; j < N; j++)
		{
			contador2 = contador2 + 1.0;
		}
	}
}

