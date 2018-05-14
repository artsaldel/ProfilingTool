
#include <stdlib.h>
#include <stdio.h>

#ifndef _MENU_H
#define _MENU_H

void InitTool();
void PrincipalMenu();
void NewProgramMenu();
void LoadProgramMenu();
void NewProcConfigMenu(int);
void LoadProcConfigMenu();
void GetStatistics();
void SetProcessorConfig(int);
void SetProcessorConfigPipeline();
void SetProcessorConfigMulticycle();
void GetExecutionTime();
void CleanBinary();
void GetNumInstructions();
int CompareStrings(char*, char*);
void RemoveSpaces(char*);
float ExecutionTimeOfMulticycle();
int IsLoad(char*);
int IsStore(char*);
int IsArithmetic(char*);
int IsControl(char*);
int IsSystemCall(char*);
 
#endif

