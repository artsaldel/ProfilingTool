#include <stdio.h>

int N = 25;
int BUFFER_MAX_SIZE = 25;

void IcNeighbors(int VD[BUFFER_MAX_SIZE],int IC[BUFFER_MAX_SIZE],int CONN[BUFFER_MAX_SIZE][BUFFER_MAX_SIZE])
{
	int V = 0;///////////////////////
	int i,j;
	int Facc = 0;///////////////////////
	int Vacc = 0;///////////////////////
	int f = 0;///////////////////////
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
		{
			V = VD[i]-VD[j];
			f = V*34;
			Facc += f*CONN[i][j];
			Vacc += V*CONN[i][j];
		}
		IC[i] = 20*Facc + 80*Vacc ;
	}
}

void main()
{
	/*float num = 0;
	for (int  i = 0; i < 25; i++)
	{
		for (int j = 0; j < 25; j++)
		{
			num = num + 1.0;
		}
	}*/

	
	int VD[N];
	int IC[N];
	int CONN[N][N];
	IcNeighbors(VD,IC,CONN);
}

