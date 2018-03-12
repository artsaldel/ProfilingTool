
#include "../include/menu.h"
#include <unistd.h>

int selection = 0;

void PrincipalMenu()
{
	system("clear");

	printf("\n*******Herramienta de perfilado para una arquitectura basada en RISC V*******\n\n");
	
	printf("Seleccione la acción a ejecutar:\n\n");
	
	printf("1. Crear un nuevo programa en C\n");
	printf("2. Cargar programa en C existente\n");
	printf("3. Crear un nuevo archivo de configuracion del procesador\n");
	printf("4. Cargar archivo de configuracion del procesador\n");
	printf("5. Salir\n\n");
	
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

