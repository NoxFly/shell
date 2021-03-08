#include "csapp.h"
#include "traitement.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

void suppr_pipe(IO_manager manager, int num_command)
{
    if (num_command == 0)
    {
        if (manager->in)
        {
            close(manager->in);
        }
    }
    else
    {
        close(manager->pipes[num_command - 1][0]);
    }
    if (num_command == manager->nb_pipes)
    {
        if (manager->out)
        {
            close(manager->out);
        }
    }
    else
    {
        close(manager->pipes[num_command][1]);
    }
}

void IO_reset(IO_manager manager, int num_command)
{
    if (num_command == manager->nb_pipes)
    {
        if (manager->out != -1)
        {
            close(manager->out);
        }
    }
}

void IO_redirect(IO_manager manager, int num_command)
{
    // set the stdin
    if (num_command == 0)
    {
        if (manager->in != -1)
        {
            dup2(manager->in, STDIN_FILENO);
        }
    }
    else
    {
        dup2(manager->pipes[num_command - 1][0], STDIN_FILENO);
    }
    // set the stdout
    if (num_command == manager->nb_pipes)
    {
        if (manager->out != -1)
        {
            dup2(manager->out, STDOUT_FILENO);
        }
    }
    else
    {
        dup2(manager->pipes[num_command][1], STDOUT_FILENO);
    }

    return;
}

void free_IO(IO_manager manager)
{
    if (manager != NULL)
    {
        for (int i = 0; i < manager->nb_pipes; i++)
            free(manager->pipes[i]);
        free(manager->pipes);
        free(manager);
        manager = NULL;
    }
}

IO_manager init_IO(struct cmdline *l)
{
    IO_manager manager = malloc(sizeof(IO_manager));

    if (l->in)
    {
        manager->in = open(l->in, O_RDONLY);
        if (manager->in == -1)
        {
            perror("IO error stdin");
            free(manager);
            return NULL;
        }
    }
    else
    {
        manager->in = -1;
    }

    if (l->out)
    {
        manager->out = open(l->out, O_WRONLY | O_CREAT, 0600);
        if (manager->out == -1)
        {
            perror("IO error stdout");
            free(manager);
            return NULL;
        }
    }
    else
    {
        manager->out = -1;
    }

    int nb_cmd;
    for (nb_cmd = 0; l->seq[nb_cmd] != 0; nb_cmd++)
        ;

    manager->nb_pipes = nb_cmd > 0 ? nb_cmd - 1 : 0;
    manager->pipes = malloc(sizeof(int *) * manager->nb_pipes);

    for (int i = 0; i < manager->nb_pipes; i++)
    {
        manager->pipes[i] = malloc(sizeof(int) * 2);

        if (pipe(manager->pipes[i]) == -1)
        {
            manager->nb_pipes = i + 1;
            free_IO(manager);
            return manager;
        }
    }

    return manager;
}

void command_handler(int sig)
{
    int e = kill(-getpid(), SIGINT);
    if (e == -1)
        perror("shell.c:kill");
    return;
}

void child_handler(int sig)
{
#ifdef VERBOSE
    printf("Child_handler triggered\n");
#endif
    int status;
    pid_t pid_child;
    while ((pid_child = waitpid(-1, &status, WNOHANG)) != 0)
    {
        if (pid_child == -1)
        {
            break;
        }
        else
        {
            printf("process %d exited\n", pid_child);
        }
    }
    return;
}

void commandTreatment(struct cmdline *l)
{
    // security, even if it's normally treated in shell.c
    if (!l || l->err || !l->seq[0])
        return;

    // quit or exit shell
    if (isEq(l->seq[0][0], "quit") || isEq(l->seq[0][0], "exit"))
    {
        printf("Exiting shell\n");
        exit(0);
    }

    IO_manager manager = init_IO(l);

    if (!manager)
    {
        perror("Failed to create pipe");
        return;
    }

    for (int i = 0; l->seq[i] != 0; i++)
    {
        pid_t pid_fils;
        char **command = l->seq[i];

        // create fork : error
        if ((pid_fils = fork()) == -1)
        {
            perror("fork");
            return;
        }

        // parent
        else if (pid_fils)
        {
            int status;
            //Signal(SIGINT, command_handler);
            suppr_pipe(manager, i);
            if (l->background)
                Signal(SIGCHLD, child_handler);

            else if (waitpid(pid_fils, &status, 0) != pid_fils)
                printf("%s : Exit whith status : %d\n", command[0], status);

            IO_reset(manager, i);
        }
        else
        {
            IO_redirect(manager, i);
            execvp(command[0], command);
            exit(errno);
        }

#ifdef VERBOSE
        printf("PID %d\n", pid_fils);
#endif
    }

    free_IO(manager);
    return;
}