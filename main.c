#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <conio.h>

#define ENTER 13
#define TAB 9
#define BKSPCE 8

#define RESET "\033[0m"
#define CYAN "\033[36m"
#define GREEN "\033[32m"
#define RED "\033[31m"

struct User
{
    char fullName[50];
    char email[50];
    char password[50];
    char phone[50];
} user;

void takeInput(char *);
void takePassword(char *);

void showMainMenu(){
    printf(CYAN"\t\t\t Welcome to Student Management System\t\t\t"RESET);
    printf(CYAN"\nPlease choose your option:"RESET);
    printf(CYAN"\n1) Signup"RESET);
    printf(CYAN"\n2) Login"RESET);
    printf(CYAN"\n3) Exit\n"RESET);
}

int main()
{
    FILE *fp;
    SetConsoleTitle("Student Management System");
    int choice;
    char conformPassword[50];

    showMainMenu();

    printf(CYAN"Enter your choice:\t"RESET);
    scanf("%d", &choice);
    while ((getchar()) != '\n')
        ;

    switch (choice)
    {
    case 1: //signup
        system("cls");
        printf(CYAN"Enter your fullName:\t"RESET);
        takeInput(user.fullName);

        printf(CYAN"Enter your email:\t"RESET);
        takeInput(user.email);

        printf(CYAN"Enter your phone:\t"RESET);
        takeInput(user.phone);

        printf(CYAN"\bEnter password:\t"RESET);
        takePassword(user.password);

        printf(CYAN"\b\nEnter confirm password:\t"RESET);
        takePassword(conformPassword);

        if (strcmp(user.password, conformPassword) != 0)
        {
            printf(RED"\nPassword not matched!"RESET);
        }
        else
        {
            fp = fopen("user.dat", "a+");
            if (fp == NULL)
            {
                printf(RED"Error opening file"RESET);
                return 1;
            }
            size_t written = fwrite(&user, sizeof(struct User), 1, fp);
            fputc('\n', fp);
            if (written != 0)
                printf(GREEN"\n%s, Your account has been created successfully!\n"RESET, user.fullName);
            else
                printf(RED"\nSorry! Something went wrong :("RESET);
            fclose(fp);
        }
        main();
        break;
    case 2: //login
        system("cls");
        {
            char email[50], password[50];
            int found = 0;
            FILE *fp;

            printf(CYAN"Enter your email:\t"RESET);
            takeInput(email);

            printf(CYAN"Enter your password:\t"RESET);
            takePassword(password);

            fp = fopen("user.dat", "r");
            if (fp == NULL)
            {
                printf(RED"\nNo users found. Please sign up first.\n"RESET);
                break;
            }

            while (fread(&user, sizeof(struct User), 1, fp))
            {
                if (strcmp(user.email, email) == 0 && strcmp(user.password, password) == 0)
                {
                    found = 1;
                    break;
                }
            }
            fclose(fp);

            if (found) {
                printf(GREEN"\nLogin successful! Welcome, %s.\n"RESET, user.fullName);
                printf(CYAN"\nYour Details:\n"RESET);
                printf("Full Name: %s\n", user.fullName);
                printf("Email: %s\n", user.email);
                printf("Phone: %s\n", user.phone);
            }
            else
                printf(RED"\nInvalid email or password.\n"RESET);
        }
        break;
    case 3:
        printf(GREEN"\t\t\tBye Bye :)\n"RESET);
        system("exit");
        break;
    }
}

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
