// Standard Libraries
#include <stdio.h>
#include <string.h>
#include <conio.h>

// Custom Event Definations
#define ENTER 13
#define TAB 9
#define BACKSPACE 8

// Color definitions
#define RESET "\033[0m"
#define RED "\033[31m"
#define CYAN "\033[36m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"

void takeInput(char *input, int maxSize)
{
    if (fgets(input, maxSize, stdin) != NULL)
    {
        // Remove newline character if present
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n')
        {
            input[len - 1] = '\0';
        }
    }
}

void takePassword(char *password, int maxSize)
{
    int i = 0;
    char ch;

    while (i < maxSize - 1)
    {
        ch = getch();

        if (ch == ENTER || ch == TAB)
        {
            password[i] = '\0';
            break;
        }
        else if (ch == BACKSPACE)
        {
            if (i > 0)
            {
                i--;
                printf("\b \b");
            }
        }
        else
        {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
}

void clearScreen()
{
    system("cls");
}

void terminateProgram()
{
    clearScreen();
    for (int i = 3; i > 0; i--)
    {
        clearScreen();
        printf(YELLOW "Terminating program in %d second(s)...\n" RESET, i);
        Sleep(1000);
    }
    clearScreen();
    printf(RED "Program terminated! Goodbye! :)\n" RESET);
    exit(0);
}

void pressKeyToContinue()
{
    printf(CYAN "\nPress ENTER to continue..." RESET);
    getchar();
}
