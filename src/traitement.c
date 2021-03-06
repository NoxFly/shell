#include "csapp.h"
#include "traitement.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>


int execCommand(struct cmdline *l, int pipe_in, int pipe_out)
{
    char **argv = l->seq[0];
    int file_in, file_out, save_in, save_out;

    if(pipe_in != -1){
        printf("pipe in detected\n");
    }

    else if (l->in)
    {
        file_in = open(l->in, O_RDONLY);
        
        // entry point not found
        if (file_in == -1)
        {
            perror("Erreur, le fichier d'entrée est introuvable\n");
            return -4;
        }
        
        save_in = dup(0); // 0 --> stdin

        // Erreur de redirection de l'entrée standard
        if (dup2(file_in, 0) == -1)
        {
            perror("Erreur, impossible de modifier l'entrée standard :\n");
            return -5;
        }
    }
    if (pipe_out != -1){
        printf("pipe out detected\n");
    }
    else if (l->out)
    {
        file_out = open(l->out, O_WRONLY | O_CREAT, S_IRWXU);

        // permission denied
        if (file_out == -1)
        {
            printf("%s : Permission denied\n", l->in);
            return -2;
        }

        save_out = dup(1); // 1 --> stdout

        // redirection
        if (dup2(file_out, 1) == -1)
        {
            perror("Erreur de redirection dans le fichier\n");
            return -3;
        }
    }

    // error
    if (l->err)
    {
        fprintf(stderr, "Une erreur de commande s'est produite\n");
        return -6;
    }

#ifdef VERBOSE
    printArgs(argv);
#endif

    // create child process
    pid_t pid_fils = fork();
    int status;

    // child
    if (pid_fils == 0)
    {
        execvp(argv[0], argv);
        // exit if an error occured
        exit(errno);
    }
    
    else
    {
        // wait for the child to return something
        int status;
	    while(wait(&status) > 0) {}

        // if the child exited and not safely, then print the error
        if(WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {
            char msg[100];

            // handled error (have a custom message for it)
            if(WEXITSTATUS(status) < LENERROR)
                strcpy(msg, errnoMessages[WEXITSTATUS(status)]);
            // default error message
            else
                strcpy(msg, "An error occured");

            fprintf(stderr, "%s : %s\n", argv[0], msg);
        }
    }

#ifdef VERBOSE
    printf("PID %d\n", pid_fils);
#endif

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
    // Kill(pid_fils, EXIT_SUCCESS);

    return 0;
}

void commandTreatment(struct cmdline *l)
{
    // security, even if it's normally treated in shell.c
    if (l == NULL || l->seq[0] == NULL)
    {
        return;
    }

    // quit or exit shell
    if (isEq(l->seq[0][0], "quit") || isEq(l->seq[0][0], "exit"))
    {
        printf("Exiting shell\n");
        exit(0);
    }

    // multiple commands
    if (l->seq[1] != 0)
    {
        if(l->seq[2] != 0){
            printf("Seulement 1 pipe marche pour l'instant");
            return;
        }
        int pipe_in, pipe_out;
        for(int i=0; l->seq[i] != 0; i++){
            if(i == 0) {
                // pipe_in
            }
        }

    }

    else {
        // execute the command
        int res = execCommand(l, -1, -1);
    }

#ifdef VERBOSE
    printf("resultat de la commande : %d\n", res);
#endif
}


void printArgs(char **command)
{
    int len = -1, i = 0;

    // recover number of argument passed in the array
    while (command[i++] != 0)
        len++;

    // create the array with a fixed size (allows to not do a malloc)
    char *argv[len];

    // recover each argument
    i = 0;
    while (i < len)
    {
        i++;
        argv[i - 1] = command[i];
    }

    // display
    printf("Commande : %s\nArgument%s (%d) : ", command[0], len > 1 ? "s" : "", len);

    for (int j = 0; j < len; j++)
        printf("%s%s", argv[j], (j < len - 1) ? ", " : "");
    printf("\n");
}
