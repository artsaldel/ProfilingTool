
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

//Starts the application
void InitTool()
{
	strcpy(programPath, "program.c");
	PrincipalMenu();
}

//Aux for starting application
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

	//Identify the user selection
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

//Create a new program
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

//Change the program to be compiled
void LoadProgramMenu()
{
	system("clear");
	printf("Ingresar el nombre del programa (.c): ");
	scanf("%200s", &programPath);
	PrincipalMenu();
}

//Change the configuration file
void NewProcConfigMenu()
{
	system("nano processor.config");

	//Finishing this menu
	printf("\nListo, ver el archivo processor.config!!!!\n");
	sleep(3); 
	//Calling the principal menu
	PrincipalMenu();
}

//Start getting the statistics
void GetStatistics()
{
	char compileCommand [200];
	strcpy(compileCommand, "riscv32-unknown-elf-gcc ");
	strcat(compileCommand, programPath);
	strcat(compileCommand, " -o program -Os");//-Os para optimizacion de velocidad
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
	GetExecutionTime();
	sleep(4);
	//Calling the principal menu
	PrincipalMenu();
}

//Clear the generated binary file
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
}

//Get the number of instruction generated in a program
void GetNumInstructions()
{
	//Counting the ammount of instructions, icluding loops and everything
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
	//printf("Numero de instrucciones = %d", numInstructions);
	system("rm numInstructions.txt");
}

//Change the configiration file
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

//Complete the statisttics
void GetExecutionTime()
{
	float executionTime = 0.0;
	if (CompareStrings(processorType, "multicycle"))
	{
		int cpiLoads = 0;
		int cpiStores = 0;
		int cpiArithmetics = 0;
		int cpiControl = 0;
		int cpiSystemCalls = 0;

		printf("Debido a que el procesador es multiciclo,\nnecesitamos saber el CPI de los diferentes tipos  de instrucciones:\n\n");
		printf("Ingresar CPI de instrucciones de carga: ");
		scanf("%10d", &cpiLoads);
		printf("Ingresar CPI de instrucciones de almacenamiento: ");
		scanf("%10d", &cpiStores);
		printf("Ingresar CPI de instrucciones aritméticas: ");
		scanf("%10d", &cpiArithmetics);
		printf("Ingresar CPI de instrucciones de control de flujo: ");
		scanf("%10d", &cpiControl);
		printf("Ingresar CPI de instrucciones de llamadas al sistema: ");
		scanf("%10d", &cpiSystemCalls);

		executionTime = ExecutionTimeOfMulticycle(cpiLoads, cpiStores, cpiArithmetics, cpiControl, cpiSystemCalls);

		//printf("Cargas = %d\nAlmacenamientos = %d\nAritmeticas = %d\nSysCalls = %d\n\n", cpiLoads, cpiStores, cpiArithmetics, cpiSystemCalls);
	}
	else if (CompareStrings(processorType, "pipeline"))
	{
		//Calculating the execution time
		executionTime = (processorCPI * numInstructions) / ((float) processorFrequency );
	}
	

	if (executionTime != 0.0)
	{
		//Writing the time to the file
		FILE* timesFile = fopen ("timeResults.txt", "w+");
		fprintf(timesFile, "%s","\n\n********** EXECUTION TIME **********\n\n");
		fprintf(timesFile, "Execution time = %.6f ms\n\n", executionTime * 1000.0 );
		fprintf(timesFile, "%s","***********************************");
		fclose(timesFile);

		//Showing the file timeResult.txt
		system("cat timeResults.txt");

		//Finishing this menu
		printf("\n\nListo, ver el archivo timeResults.txt!!!!\n");
	}
	else
	{
		printf("Configuración de procesador no reconocida, vuelva a intentarlo.\n");
	}
}

int CompareStrings (char *s1, char *s2)
{
	char s1c;
	char s2c;
	do
	{
		s1c = *(s1++);
		s2c = *(s2++);
		if (s1c == '\n' || s1c == '\0')
			s1c = 0;
		if (s2c == '\n' || s2c == '\0')
			s2c = 0;
		if (s1c != s2c)
			return 0;
	}
	while (s1c);        
	return 1;
}

void RemoveSpaces(char *str)
{
	// To keep track of non-space character count
    int count = 0;
 
    // Traverse the given string. If current character
    // is not space, then place it at index 'count++'
    for (int i = 0; str[i]; i++)
        if (str[i] != ' ')
            str[count++] = str[i]; // here count is
                                   // incremented
    str[count] = '\0';
}

float ExecutionTimeOfMulticycle(int cpiLoads, int cpiStores, int cpiArithmetics, int cpiControl, int cpiSystemCalls)
{
	int quantityLoads = 0;
	int quantityStores = 0;
	int quantityArithmetics = 0;
	int quantityControl = 0;
	int quantitySystemCalls = 0;
	int quantityOthers = 0;

	system("rv-bin histogram -I program > instructionsQuantity.txt");
	
	FILE * fp = fopen("instructionsQuantity.txt", "r");
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
    	//Getting the data of ammount of instruction to execute
        char *set1 = strtok(line, ".");
        set1 = strtok(NULL, ".");
        char *set2 = strtok(set1, "[");

        char instName[20];
        strcpy(instName, set2);
        RemoveSpaces(instName);
        

        set2 = strtok(NULL, "[");
        char *set3 = strtok(set2, "]");
        int quantity = atoi(set3);
        //printf("InstName = %s, Quantity = %d\n", instName, quantity);

        //Identifyeng the type of instruction
        if (IsLoad(instName))
        	quantityLoads += quantity;
        else if (IsStore(instName))
        	quantityStores += quantity;
        else if (IsArithmetic(instName))
        	quantityArithmetics += quantity;
        else if (IsControl(instName))
        	quantityControl += quantity;
        else if (IsSystemCall(instName))
        	quantitySystemCalls += quantity;
        else
        	quantityOthers += quantity;
    }
    fclose(fp);
    if (line)
        free(line);

	//Using the formula TE(multiciclo) = NumbInst * (%Loads * CPILoads + %Stores * CPIStores +
	//												 %Arith * CPIArith + %Control * CPIControl + 
	//												 %SysCalls * CPISysCalls + %Others * CPIOthers ) / frequency

    float percLoads = ((float)quantityLoads/numInstructions);
    float percStores = ((float)quantityStores/numInstructions);
    float percArith = ((float)quantityArithmetics/numInstructions);
    float percControl = ((float)quantityControl/numInstructions);
    float percSysCalls = ((float)quantitySystemCalls/numInstructions);
    float percOther = ((float)quantityOthers/numInstructions);

    float exeTime = (numInstructions * (percLoads * cpiLoads + percStores * cpiStores + percArith * cpiArithmetics + 
    									percControl * cpiControl + percSysCalls * cpiSystemCalls + percOther * 1 ) )/((float) processorFrequency );

	return exeTime;
}

int IsLoad (char *s1)
{
	if (CompareStrings(s1,"lui") || CompareStrings(s1,"lb") || CompareStrings(s1,"lh") || CompareStrings(s1,"lw") || CompareStrings(s1,"lbu") || 
		CompareStrings(s1,"lhu") )
	{
		return 1;
	}
	return 0;
}

int IsStore (char *s1)
{
	if (CompareStrings(s1,"sb") || CompareStrings(s1,"sh") || CompareStrings(s1,"sw") )
	{
		return 1;
	}
	return 0;
}

int IsArithmetic (char *s1)
{
	if (CompareStrings(s1,"addi") || CompareStrings(s1,"slti") || CompareStrings(s1,"sltiu") || CompareStrings(s1,"xori") || CompareStrings(s1,"ori") || 
		CompareStrings(s1,"andi") || CompareStrings(s1,"slli") || CompareStrings(s1,"srli") || CompareStrings(s1,"srai") || CompareStrings(s1,"add") ||
		CompareStrings(s1,"sub") || CompareStrings(s1,"sll") || CompareStrings(s1,"slt") || CompareStrings(s1,"sltu") || CompareStrings(s1,"xor") ||
		CompareStrings(s1,"srl") || CompareStrings(s1,"sra") || CompareStrings(s1,"or") || CompareStrings(s1,"and") || CompareStrings(s1,""))
	{
		return 1;
	}
	return 0;
}

int IsControl (char *s1)
{
	if (CompareStrings(s1,"auipc") || CompareStrings(s1,"jal") || CompareStrings(s1,"jalr") || CompareStrings(s1,"beq") || CompareStrings(s1,"bne") ||
		CompareStrings(s1,"blt") || CompareStrings(s1,"bge") || CompareStrings(s1,"bltu") || CompareStrings(s1,"bgeu"))
	{
		return 1;
	}
	return 0;
}

int IsSystemCall (char *s1)
{
	if (CompareStrings(s1,"fence") || CompareStrings(s1,"fence.i") || CompareStrings(s1,"scall") || CompareStrings(s1,"sbreak") || 
		CompareStrings(s1,"rdcycle") || CompareStrings(s1,"rdcycleh") || CompareStrings(s1,"rdtime") || CompareStrings(s1,"rdtimeh") ||
		CompareStrings(s1,"rdinstret") || CompareStrings(s1,"rdinstreth") || CompareStrings(s1,"ecall") )
	{
		return 1;
	}
	return 0;
}
