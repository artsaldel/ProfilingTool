int N = 9;

void IcNeighbors(float VD[N],float IC[N],float CONN[N][N])///////////////////////
{
	float V = 0;///////////////////////
	int i,j;
	float Facc = 0;///////////////////////
	float Vacc = 0;///////////////////////
	float f = 0;///////////////////////
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
		{
			V = VD[i]-VD[j];
			f = V*34.0;
			Facc = Facc + f*CONN[i][j];
			Vacc = Vacc + V*CONN[i][j];
		}
		IC[i] = 20.0*Facc + 80.0*Vacc ;
	}
}

int main()
{
	float VD[N];
	float IC[N];
	float CONN[N][N];
	IcNeighbors(VD, IC, CONN);
}