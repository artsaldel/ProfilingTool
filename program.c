#include <stdio.h>

int SumInt(int,int);
int SubInt(int,int);
int MultInt(int,int);
int DivInt(int,int);

float SumFloat(float,float);
float SubFloat(float,float);
float MultFloat(float,float);
float DivFloat(float,float);

void main()
{
	int sumInt = SumInt(1, 2);
	int multInt = MultInt(2, 3);
	int subInt = SubInt(5, 2);
	int divInt = DivInt(3, 3);	

	float sumFloat = SumFloat(2.3, 1.7);
	float subFloat = SubFloat(9.2, 2.1);
	float multFloat = MultFloat(2.9, 3.1);
	float divFloat = DivFloat(2.1, 6.2);

	for( int i = 0 ; i < 10; i++)
	{
		SumInt(1,i);
	}
}

int SumInt(int a, int b)
{
	return a + b;
}

float SumFloat(float a, float b)
{
	return a + b;
}

int SubInt(int a, int b)
{
	return a - b;
}

float SubFloat(float a, float b)
{
	return a - b;
}

int MultInt(int a, int b)
{
	return a * b;
}

float MultFloat(float a, float b)
{
	return a * b;
}

int DivInt(int a, int b)
{
	return a / b;
}

float DivFloat(float a, float b)
{
	return a / b;
}
