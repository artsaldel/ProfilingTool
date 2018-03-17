
#include "../include/menu.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define SIZE_INST_MEM 1024

//Menu configuration
int selection = 0;

//Number of instruction in the current program
int numInstructions = 0;


//Processor configuration
char processorType[30];
char processorVersion[30];
int processorCPI;
int processorFrequency;


void PrincipalMenu()
{
	system("clear");

	printf("\n*******Herramienta de perfilado para una arquitectura basada en RISC V*******\n\n");
	
	printf("Seleccione la acción a ejecutar:\n\n");
	
	printf("1. Crear un nuevo programa en C\n");
	printf("2. Cargar programa en C existente\n");
	printf("3. Crear un nuevo archivo de configuracion del procesador\n");
	printf("4. Cargar archivo de configuracion del procesador\n");
	printf("5. Verificar tiempo de ejecución del programa cargado\n");
	printf("6. Salir\n\n");
	
	printf("Ingresar # de accion: ");
	scanf("%10d", &selection);

	if ( selection == 1)
	{
		
	}
	else if ( selection == 2)
	{
		
	}
	else if ( selection == 3)
	{
		
	}
	else if ( selection == 4)
	{

	}
	else if ( selection == 5)
	{
		GetStatistics();
	}
	else if ( selection == 6)
	{
		printf("\nHasta pronto!\n\n");
	}
	else
	{
		printf("\nValor incorrecto, vuelva a intentarlo\n");
		sleep(2); 
		PrincipalMenu();
	}

	
}

void NewProgramMenu()
{

}

void LoadProgramMenu()
{

}

void NewProcConfigMenu()
{

}

void LoadProcConfigMenu()
{

}

void GetStatistics()
{
	//Compiling
	system("riscv32-unknown-elf-gcc float.c -nostartfiles -Tlink.ld -o program");
	//Creating the ObjDump
	system("riscv32-unknown-elf-objdump -d program > program.dump");
	//Creating the assembler
	system("elf2hex 4 1024 program > binary.txt");
	//Create the final binary and get the number of instruction to execute
	GetNumInstructions();
	//Clearing the terminal
	system("clear");
	//Set the configuration of the processor
	SetProcessorConfig();
	//Writing to the output file the execution time
	GetExecutionTime(numInstructions);

	//Finishing this menu
	printf("\nListo, ver el archivo timeResults.txt!!!!\n");
	sleep(3); 
	//Calling the principal menu
	PrincipalMenu();
}

void GetNumInstructions()
{
	numInstructions = 0;
	FILE* fullProgram = fopen("binary.txt","r");
	FILE* liteProgram = fopen ("executableBinary.txt", "w+");
	char line[16];
	char cmp[8];
	strcpy(cmp, "00000000");
	for (int ctdr = 0; ctdr < SIZE_INST_MEM; ctdr++)
	{
		fgets (line, sizeof(line), fullProgram); 
		if(strncmp(line,cmp,8) != 0) 
		{
			fprintf(liteProgram, "%s",line);
			numInstructions++;
		} 
	}
	fclose(fullProgram);
	fclose(liteProgram);
}

void SetProcessorConfig()
{
	FILE* configFile = fopen("processor.config","r");
	char line[50];
	for (int ctdr = 0; ctdr < 4; ctdr++)
	{
		fgets(line, sizeof(line), configFile);
		char *set = strtok(line, "=");
		set = strtok(NULL, "=");
		if (set != NULL)
		{
			if (ctdr == 0)
				strcpy(processorType, set);
			else if (ctdr == 1)
				strcpy(processorVersion, set);
			else if (ctdr == 2)
				processorCPI = atoi(set);
			else if (ctdr == 3)
				processorFrequency = atoi(set);
		}
	}
	fclose(configFile);
}

void GetExecutionTime(int numInstructions)
{
	//Calculating the execution time
	float executionTime = (processorCPI * numInstructions) / ((float) processorFrequency );
	//Writing the time to the file
	FILE* timesFile = fopen ("timeResults.txt", "w+");
	fprintf(timesFile, "%s","********** EXECUTION TIME **********\n\n");
	fprintf(timesFile, "Execution time = %.6f ms\n\n", executionTime * 1000.0 );
	fprintf(timesFile, "%s","***********************************");
	fclose(timesFile);
}
