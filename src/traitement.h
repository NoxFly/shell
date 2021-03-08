#ifndef TRAITEMENT_H
#define TRAITEMENT_H

#include "readcmd.h"

// #define VERBOSE

struct IO_mngr
{
    int in;
    int out;
    int nb_pipes;
    int **pipes;
};
typedef struct IO_mngr *IO_manager;

/**
 * Treat the given command.
 * If 'quit' or 'exit' written, then exit the program, 
 * otherwise execute the command.
 * @param l the cmdline struct pointer to the entered command
 */
void commandTreatment(struct cmdline *l);

/**
 * Displays the command and its argument(s)
 * @param command the string containing the written command followed by optional arguments
 * @code
 *  user@shell> ls -la
 *  Commande : ls
 *  Argument (1) : -la
 * @endcode
 */
void printArgs(char **command);

#define LENERROR 3
static char *errnoMessages[] = {"", "Permission denied", "Command not found"};

#endif