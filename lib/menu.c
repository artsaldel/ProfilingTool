
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
int processorCPI = 0;
int processorFrequency = 0;
int percentageBranchs = 0;

int cpiLoads = 0;
int cpiStores = 0;
int cpiArithmetics = 0;
int cpiControl = 0;
int cpiSystemCalls = 0;

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
	printf("3. Editar archivo de configuracion del procesador Pipeline\n");
	printf("4. Editar archivo de configuracion del procesador Multiciclo\n");
	printf("5. Verificar tiempo de ejecución del programa cargado\n");
	printf("6. Salir\n\n");
	
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
		NewProcConfigMenu(1);
	}
	else if ( selection == 4)
	{
		NewProcConfigMenu(2);
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

//Create a new program
void NewProgramMenu()
{
	system("nano program.c"); //Opening the program.c
	strcpy(programPath, "program.c"); //Creating the new path
	printf("\nListo, ver el archivo program.c!!!!\n"); //Finishing this menu
	sleep(3); 
	PrincipalMenu(); //Calling the principal menu
}

//Change the program to be compiled
void LoadProgramMenu()
{
	system("clear"); //Clearing the terminal
	printf("Ingresar el nombre del programa (.c): ");
	scanf("%200s", &programPath); //Copying the new path
	PrincipalMenu(); //Restart the principal menu
}

//Change the configuration file
void NewProcConfigMenu(int arch)
{
	if (arch == 1)
	{
		system("nano config_pipeline.config"); //Opening the required file for pipeline configuration
		printf("\nListo, ver el archivo config_pipeline.config!!!!\n"); //Finishing this menu
	}
	else if (arch == 2)
	{
		system("nano config_multicycle.config"); //Opening the required file for multicyle configuration
		printf("\nListo, ver el archivo config_multicycle.config!!!!\n"); //Finishing this menu
	}
	sleep(3); 
	PrincipalMenu(); //Calling the principal menu
}

//Start getting the statistics
void GetStatistics()
{
	char compileCommand [200];
	strcpy(compileCommand, "riscv32-unknown-elf-gcc ");
	strcat(compileCommand, programPath);
	strcat(compileCommand, " -o program");
	//strcat(compileCommand, " -o program -Os");//-Os para optimizacion de velocidad
	//strcat(compileCommand, " -nostartfiles -Tlink.ld -o program");
	system(compileCommand); //Compiling
	system("riscv32-unknown-elf-objdump -d program > program.dump"); //Creating the ObjDump
	system("elf2hex 4 32768 program > binary.txt"); //Creating the assembler
	CleanBinary(); //Clean the final binary
	GetNumInstructions(); //Create the final binary and get the number of instructions to execute
	system("clear");//Clearing the terminal
	printf("Sobre cuál arquitectura desea simular?\n\n"); //Set the configuration of the processor
	printf("1. Pipeline\n");
	printf("2. Multiciclo\n\n");
	printf("Ingrese la opcion: ");
	int archSelection = 0;
	scanf("%10d", &archSelection);
	SetProcessorConfig(archSelection);
	GetExecutionTime(); //Writing to the output file the execution time
	sleep(4);
	PrincipalMenu(); //Calling the principal menu
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
	//system("rm numInstructions.txt");
}

//Change the configuration file
void SetProcessorConfig(int archSelection)
{
	if (archSelection == 1) //Selection of the configuration: 1=pipeline, 2=multicycle
		SetProcessorConfigPipeline(); //Getting the configuration of the pipeline architecture
	else if (archSelection == 2)
		SetProcessorConfigMulticycle(); //Getting the configuration of the multicycle architecture
	else
	{
		system("clear"); //Clearing the terminal
		printf("Ingrese una opción válida\n\n"); //Warning message
		sleep(3); 
		GetStatistics(); //Returning to the Get Statistics Menu
	}
}

//Getting data for the pipeline configuration
void SetProcessorConfigPipeline()
{
	FILE* configFile = fopen("config_pipeline.config","r");
	char line[50];
	for (int ctdr = 0; ctdr < 5; ctdr++)
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
			else if (ctdr == 4)
				percentageBranchs = atoi(set);
		}
	}
	fclose(configFile);
}

//Getting data for the multicycle configuration
void SetProcessorConfigMulticycle()
{
	FILE* configFile = fopen("config_multicycle.config","r");
	char line[50];
	for (int ctdr = 0; ctdr < 8; ctdr++)
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
				cpiLoads = atoi(set);
			else if (ctdr == 3)
				cpiStores = atoi(set);
			else if (ctdr == 4)
				cpiArithmetics = atoi(set);
			else if (ctdr == 5)
				cpiControl = atoi(set);
			else if (ctdr == 6)
				cpiSystemCalls = atoi(set);
			else if (ctdr == 7)
				processorFrequency = atoi(set);
		}
	}
	fclose(configFile);
}

//Complete the statisttics
void GetExecutionTime()
{
	float executionTime = 0.0;
	if (CompareStrings(processorType, "pipeline"))
		executionTime = (processorCPI * numInstructions) / ((float) processorFrequency ); //Calculating the execution time of the pipeline architecture
	else if (CompareStrings(processorType, "multicycle"))
		executionTime = ExecutionTimeOfMulticycle(); //Calculating the execution time of the multicycle architecture

	if (executionTime != 0.0)
	{
		//Writing the time to the file
		FILE* timesFile = fopen ("timeResults.txt", "w+");
		fprintf(timesFile, "%s","\n\n********** EXECUTION TIME **********\n\n");
		fprintf(timesFile, "Execution time = %.6f ms\n\n", executionTime * 1000.0 );
		fprintf(timesFile, "%s","***********************************");
		fclose(timesFile);
		system("cat timeResults.txt"); //Showing the file timeResult.txt
		printf("\n\nListo, ver el archivo timeResults.txt!!!!\n"); //Finishing this menu
	}
	else
		printf("Configuración de procesador no reconocida, vuelva a intentarlo.\n");
}

float ExecutionTimeOfMulticycle()
{
	//Initiating the variables for the ammount of instructions per types
	int quantityLoads = 0;
	int quantityStores = 0;
	int quantityArithmetics = 0;
	int quantityControl = 0;
	int quantitySystemCalls = 0;
	int quantityOthers = 0;
	
	system("rv-bin histogram -I program > instructionsQuantity.txt"); //Creating a file to grab the ammount of instructions
	FILE * fp = fopen("instructionsQuantity.txt", "r"); //Reading the file with all the simulated program
    
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

        //Identifying the type of instruction
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

    printf("Perc loads = %f\n", percLoads);
    printf("Perc stores = %f\n", percStores);
    printf("Perc arithm = %f\n", percArith);
    printf("Perc control = %f\n", percControl);
    printf("Perc syscalls = %f\n", percSysCalls);
    printf("Perc others = %f\n", percOther);

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

//Comparing strings
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

//Removing all the spaces of a string
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