#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int prompt;

static void sighandler(int signo)
{
    if (signo == SIGINT)
    {
        prompt = open("s", O_WRONLY);
        write(prompt, "kill", 256);
        close(prompt);
        kill(getpid(), SIGQUIT);
    }
}

int main()
{
    mkfifo("c", 0666); //create client pipe
    char input[256], back[256];
    //char msg[] = "Hi";
    //mkfifo("transfer", 0666); //create pipe
    //mkfifo("return", 0666); //create pipe
    //this sends a msg
    printf("Awaiting a connection...\n");
    prompt = open("s", O_WRONLY);
    write(prompt, "Hi", 256);
    close(prompt);
    while(1)
    {
        prompt = open("c", O_RDONLY);
        read(prompt, input, 256);
        if (!strcmp(input, "what"))
        {
            close(prompt);
            printf("Connected!\n");
            break;
        }
    }

    signal(SIGINT, sighandler);

    while (1)
    {
        printf("Input a word:\n");
        fgets(input, 256, stdin);
        prompt = open("s", O_WRONLY);
        write(prompt, input, 256);
        close(prompt);

        prompt = open("c", O_RDONLY);
        read(prompt, input, 256);
        close(prompt);
        printf("Encrypted msg: %s\n", input);
    }
    return 0;
}