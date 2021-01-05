#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

void bob(char *input)
{
    char temp[256];
    int len = strlen(input);

    for (int i = 0; i < len / 2; i++)
    {
        temp[i] = input[len - i - 1];
        input[len - i - 1] = input[i];
        input[i] = temp[i];
    }
}

int main()
{
    mkfifo("s", 0666); //server pipe
    while (1)
    {
        int prompt;
        char input[256];
        printf("Awaiting a connection...\n");
        while (1)
        {
            prompt = open("s", O_RDONLY);
            read(prompt, input, 256);
            if (!strcmp(input, "Hi"))
            {
                close(prompt);
                printf("Connected!\n");
                break;
            }
        }
        prompt = open("c", O_WRONLY);
        write(prompt, "what", 256);
        close(prompt);

        while (1)
        {
            prompt = open("s", O_RDONLY); //send prompt to encrypt.c
            read(prompt, input, 256);
            if (!strcmp(input, "kill"))
            {
                printf("Connection ended. ");
                break;
            }
            close(prompt);
            printf("Received msg: %s\n", input);
            bob(input);
            prompt = open("c", O_WRONLY);
            write(prompt, input, 256);
            close(prompt);
        }
    }
    return 0;
}