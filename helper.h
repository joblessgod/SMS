// Standard Libraries
#include <stdio.h>
#include <string.h>
#include <conio.h>

// Custom Event Definations
#define ENTER 13
#define TAB 9
#define BKSPCE 8

// Custom Colors
#define RESET "\033[0m"
#define RED "\033[31m"
#define CYAN "\033[36m"
#define GREEN "\033[32m"

void takeInput(char ch[50])
{
    fgets(ch, 50, stdin);
    ch[strlen(ch) - 1] = 0;
}

void takePassword(char pwd[50])

{
    int i = 0;
    char ch;
    while (1)
    {
        ch = getch();
        if (ch == ENTER || ch == TAB)
        {
            pwd[i] = '\0';
            break;
        }
        else if (ch == BKSPCE)
        {
            if (i > 0)
            {
                i--;
                printf("\b \b");
            }
        }
        else
        {
            pwd[i++] = ch;
            printf("* \b");
        }
    }
}

void terminateProgram()
{
    system("cls");
    for (int i = 3; i > 0; i--)
    {
        system("cls");
        printf("Terminating the program in %d second...\n", i);
        Sleep(1000);
    }
    system("cls");
    printf(RED "Program Terminated! Good Bye :)" RESET);
    system("exit");
}

void pressKeyToContinue()
{
    printf("\nPress ENTER to continue...");
    getchar();
}