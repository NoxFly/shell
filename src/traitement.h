#ifndef __TRAITEMENT_H__
#define __TRAITEMENT_H__

//#define VERBOSE

#include "readcmd.h"

void commandTreatment(struct cmdline* l);
int shouldTermClose();
void printArgs(char** command);

#endif