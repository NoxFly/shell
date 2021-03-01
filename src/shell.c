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


int main()
{
	struct cmdline *l;
	int i, j;
    int showPrompter = 1;

	while(!shouldTermClose()) {
        /* Display each command of the pipe */
        if(showPrompter) {
            printf("\033[0;31m%s@shell\033[0m:\033[0;90m%s\033[0m> ",
                getenv("USER"),
                strrep(getenv("PWD"), getenv("HOME"), "~")
            );
        }

        else {
            showPrompter = 1;
        }

        l = readcmd();

        if(l == NULL || l->seq[0] == NULL) {
            exit(1);
        }

        // ignore commentaries
        if(l->seq[0][0][0] == '#') {
            showPrompter = 0;
            continue;
        }


        commandTreatment(l);

        if (l->err) {
            /* Syntax error, read another command */
            printf("error: %s\n", l->err);
            continue;
        }
    
#ifdef VERBOSE
        if (l->in) printf("in: %s\n", l->in);
        if (l->out) printf("out: %s\n", l->out);

        for (i=0; l->seq[i]!=0; i++) {
			char **cmd = l->seq[i];
			printf("seq[%d]: ", i);

			for (j=0; cmd[j]!=0; j++) {
				printf("%s ", cmd[j]);
			}

        	printf("\n");
        }
#endif
	}
}
