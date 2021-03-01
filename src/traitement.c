#include "csapp.h"
#include "traitement.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include <libexplain/execvp.h>

static int shouldClose = 0;

int execCommand(struct cmdline *l)
{
    char **argv = l->seq[0];
    int file_in;
    int file_out;

    int save_in;
    int save_out;

    if (l->in)
    {
        file_in = open(l->in, O_RDONLY);
        if (file_in == -1)
        {
            perror("Erreur, le fichier d'entrée est introuvable");
            return -4;
        }
        save_in = dup(0); // 0 --> stdin
        if (dup2(file_in, 0) == -1)
        {
            perror("Erreur");
            return -5;
        }
    }

    if (l->out)
    {
        file_out = open(l->out, O_WRONLY | O_CREAT);
        if (file_out == -1)
        {
            printf("%s : Permission denied", l->in);
            return -2;
        }
        save_out = dup(1); // 1 --> stdout
        if (dup2(file_out, 1) == -1)
        {
            perror("Erreur de redirection dans le fichier");
            return -3;
        }
    }

    if (l->err)
    {
        printf("Une erreur de commande c'est produite\n");
        return -6;
    }

#ifdef VERBOSE
    printArgs(argv);
#endif

    pid_t pid_fils = fork();
    int status;

    if (pid_fils == 0)
    {
        execvp(argv[0], argv);
    }

    printf("PID %d\n", pid_fils);

    pid_t ret_pid = waitpid(0, &status, 0);

    if (ret_pid == -1)
    {
        printf("%s : command not found\n", argv[0]);
        //Kill(ret_pid, EXIT_FAILURE);
        return -1;
    }
    if (l->in)
    {
        close(file_in);
        dup2(save_in, 0);
        close(save_in);
    }

    if (l->out)
    {
        fflush(stdout);
        close(file_out);
        dup2(save_out, 1);
        close(save_out);
    }
    //Kill(pid_fils, EXIT_SUCCESS);

    return 0;

#ifndef VERBOSE
    printf("pid_fils : %d\n", pid_fils);
#endif

    return 0;
}

void commandTreatment(struct cmdline *l)
{
    if (l == NULL)
    {
        return;
    }
    if (l->seq[0] == NULL)
    {
        return;
    }

    if (isEq(l->seq[0][0], "quit") || isEq(l->seq[0][0], "exit"))
    {
        printf("exiting shell\n");
        exit(0);
    }

    if (l->seq[1] != 0)
    {
        printf("Les commandes avec des pipes n'ont pas été implémentée\n");
        return;
    }

    int res = execCommand(l);

#ifdef VERBOSE
    printf("resultat de la commande : %d\n", res);
#endif
}

int shouldTermClose()
{
    return shouldClose;
}

void printArgs(char **command)
{
    int len = -1, i = 0;

    while (command[i++] != 0)
        len++;

    char *argv[len];

    i = 0;
    while (i < len)
    {
        i++;
        argv[i - 1] = command[i];
    }

    printf("Commande : %s\nArgument%s (%d) : ", command[0], len > 1 ? "s" : "", len);

    for (int j = 0; j < len; j++)
        printf("%s%s", argv[j], (j < len - 1) ? ", " : "");
    printf("\n");
}