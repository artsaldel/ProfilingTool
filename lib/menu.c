
#include "../include/menu.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define SIZE_INST_MEM 32768

//Menu configuration
int selection = 0;

//Number of instruction in the current program
int numInstructions = 0;


//Processor configuration
char processorType[30];
char processorVersion[30];
int processorCPI;
int processorFrequency;

//Program variables
char* programPath[200];

void InitTool()
{
	strcpy(programPath, "program.c");
	PrincipalMenu();
}


void PrincipalMenu()
{
	system("clear");

	printf("\n*******Herramienta de perfilado para una arquitectura basada en RISC V*******\n\n");
	
	printf("Seleccione la acción a ejecutar:\n\n");
	
	printf("1. Crear un nuevo programa en C\n");
	printf("2. Cargar programa en C existente\n");
	printf("3. Crear un nuevo archivo de configuracion del procesador\n");
	printf("4. Verificar tiempo de ejecución del programa cargado\n");
	printf("5. Salir\n\n");
	
	printf("Ingresar # de accion: ");
	scanf("%10d", &selection);

	if ( selection == 1)
	{
		NewProgramMenu();
	}
	else if ( selection == 2)
	{
		LoadProgramMenu();
	}
	else if ( selection == 3)
	{
		NewProcConfigMenu();
	}
	else if ( selection == 4)
	{
		GetStatistics();
	}
	else if ( selection == 5)
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
	system("nano program.c");

	strcpy(programPath, "program.c");

	//Finishing this menu
	printf("\nListo, ver el archivo program.c!!!!\n");
	printf("path = %s", programPath);
	sleep(3); 
	//Calling the principal menu
	PrincipalMenu();
}

void LoadProgramMenu()
{
	system("clear");
	printf("Ingresar el nombre del programa (.c): ");
	scanf("%200s", &programPath);
	PrincipalMenu();
}

void NewProcConfigMenu()
{
	system("nano processor.config");

	//Finishing this menu
	printf("\nListo, ver el archivo processor.config!!!!\n");
	sleep(3); 
	//Calling the principal menu
	PrincipalMenu();
}

void GetStatistics()
{
	char compileCommand [200];
	strcpy(compileCommand, "riscv32-unknown-elf-gcc ");
	strcat(compileCommand, programPath);
	strcat(compileCommand, " -o program");
	//strcat(compileCommand, " -nostartfiles -Tlink.ld -o program");

	//Compiling
	system(compileCommand);
	//Creating the ObjDump
	system("riscv32-unknown-elf-objdump -d program > program.dump");
	//Creating the assembler
	system("elf2hex 4 32768 program > binary.txt");
	//Clean the final binary
	CleanBinary();
	//Create the final binary and get the number of instruction to execute
	GetNumInstructions();
	//Clearing the terminal
	system("clear");
	//Set the configuration of the processor
	SetProcessorConfig();
	//Writing to the output file the execution time
	GetExecutionTime(numInstructions);
	//Showing the file timeResult.txt
	system("cat timeResults.txt");
	//Finishing this menu
	printf("\nListo, ver el archivo timeResults.txt!!!!\n");
	sleep(3); 
	//Calling the principal menu
	PrincipalMenu();
}

void CleanBinary()
{
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
		} 
	}
	fclose(fullProgram);
	fclose(liteProgram);

	//Open the final binary 
	system("subl executableBinary.txt");
}

void GetNumInstructions()
{
	numInstructions = 0;
	system("rv-jit -P -l program > numInstructions.txt");
	FILE* fileNumInsructions = fopen("numInstructions.txt","r");
	while(!feof(fileNumInsructions))
	{
		int ch = fgetc(fileNumInsructions);
		if(ch == '\n')
		{
			numInstructions++;
  		}
	}
	printf("Numero de instrucciones = %d", numInstructions);
	system("rm numInstructions.txt");
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
