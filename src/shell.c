
/*
 * Copyright (C) 2002, Simon Nieuviarts
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readcmd.h"
#include "csapp.h"
#include "traitement.h"
#include "utils.h"

int showPrompter = 1;

#define TRACE1 (0 == 1)
#define DEBUG_OPT_PROMPT (2 == 1)
#define OPT_PROMPT_DIESE (1 == 1)

int main()
{
    struct cmdline *l;
    int i, j;

    // while user didn't asked to quit shell
    while (1)
    {
        // show prompter
        if (!OPT_PROMPT_DIESE || showPrompter == 1)
        {
            printf("\033[0;31m%s@shell[%d]\033[0m:\033[0;90m%s\033[0m> ",
                   getenv("USER"),
                   getpid(),
                   strrep(getenv("PWD"), getenv("HOME"), "~"));
            fflush(stdout); // TODO
        }

        // if asked to hide prompter, reset it one round later
        else if (showPrompter == 0) // precise 0 cause -1 means never re-display it
        {
            showPrompter = 1;
        }

        // wait for a user action on the prompter
        l = readcmd();

        if (!l)
        {
            exit(1);
        }

        else if (l->err)
        {
            printf("Error : %s\n", l->err);
        }

        // An error occured
        else if (!l->seq)
        {
            fprintf(stderr, "Erreur : %s\n", l->err);
        }

        // nothing entered
        else if (!l->seq[0])
        {
            continue;
        }

        // ignore commentaries
        else if (l->seq[0][0][0] == '#')
        {
            if (TRACE1)
                printf("\t[DEBUG] # Lu, je desactive l'affichage prompt.\n");
            showPrompter = 0;
        }

        // treat given command
        else
        {
            commandTreatment(l);
        }

#ifdef VERBOSE
        if (l->in)
            printf("in: %s\n", l->in);
        if (l->out)
            printf("out: %s\n", l->out);

        printf("background : %d\n", l->background);

        for (i = 0; l->seq[i] != 0; i++)
        {
            char **cmd = l->seq[i];
            printf("seq[%d]: ", i);

            for (j = 0; cmd[j] != 0; j++)
            {
                printf("%s ", cmd[j]);
            }

            printf("\n");
        }
#endif
    }
}
