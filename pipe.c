
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    int p[2];
    pid_t pid;

    /*le premier tube*/

    if (pipe(p) == -1)
    {
        perror("pipe");
        exit(1);
    }

    if ((pid = fork()) == -1)
    {
        perror("fork");
        exit(1);
    }

    /*le pere*/
    if (pid)
    {
        close(p[0]);
        dup2(p[1], 1);
        execlp("ps", "ps", "-l", NULL);
        perror("execlp");
        exit(1);
    }
    close(p[1]);
    dup2(p[0], 0);

    /*le deuxieme tube*/

    if (pipe(p) == -1)
    {
        perror("pipe");
        exit(1);
    }

    if ((pid = fork()) == -1)
    {
        perror("fork");
        exit(1);
    }

    /*le père*/
    if (pid)
    {
        close(p[0]);
        dup2(p[1], 1);
        execlp("grep", "grep", "bash", NULL);
        perror("execlp");
        exit(1);
    }

    /*le fils*/
    close(p[1]);
    dup2(p[0], 0);
    execlp("wc", "wc", "-l", NULL);
    perror("execlp");
    exit(1);
}
