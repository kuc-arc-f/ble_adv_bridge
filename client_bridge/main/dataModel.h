#include <stdio.h>
#include <string.h>


void dataModel_init_proc();
void dataModel_set_datByAdvname(char *adv_name, char *value, int field );
//char *dataModel_get_datByAdvname(char *adv_name, int field );
void dataModel_get_datByAdvname(char *adv_name, int field ,char *cValue);

	
void dataModel_clear();
void dataModel_debug_printDat();
int dataModel_isComplete();
int dataModel_recvCount();
int dataModel_validAdvName(char *advname );

void dataModel_clear();
void dataModel_set_advName(int iNum,  char *adv_name );

//debug
// char *mystrcat(char *a, char *b);
	