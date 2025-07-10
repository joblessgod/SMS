// Standard Libraries
#include <stdio.h>
#include <windows.h>

// Custom Libraries
#include <mainMenus.h>
#include <helper.h>

// Custom Colors
#define RESET "\033[0m"
#define RED "\033[31m"
#define CYAN "\033[36m"
#define GREEN "\033[32m"

struct User
{
    char fullName[50];
    char email[50];
    char password[50];
    char phone[50];
} user;

int main()
{
    SetConsoleTitle("Student Management System | Team Sapphire"); // ignroe it
    int choice;
    char conformPassword[50];

    system("cls");
    showMainMenu();

    printf(CYAN "\n\t\tEnter your choice:\t" RESET);
    scanf("%d", &choice);
    while ((getchar()) != '\n')
        ;
    switch (choice)
    {
    case 1: // signupModule
        system("cls");
        printf(CYAN "Enter your fullName:\t" RESET);
        takeInput(user.fullName);

        printf(CYAN "Enter your email:\t" RESET);
        takeInput(user.email);

        printf(CYAN "Enter your phone:\t" RESET);
        takeInput(user.phone);

        printf(CYAN "\bEnter password:\t" RESET);
        takePassword(user.password);

        printf(CYAN "\b\nEnter confirm password:\t" RESET);
        takePassword(conformPassword);

        if (strcmp(user.password, conformPassword) != 0)
        {
            printf(RED "\nPassword not matched!" RESET);
        }
        else
        {

            FILE *fp = fopen("user.txt", "a+");
            if (fp == NULL)
            {
                printf(RED "Error opening file" RESET);
                return 1;
            }
            size_t written = fwrite(&user, sizeof(struct User), 1, fp);
            fprintf(fp, "Full Name: %s\nEmail: %s\nPhone: %s\nRole: %s\nPassword: %s\n",
                    user.fullName, user.email, user.phone, "Student", user.password);
            if (written != 0)
                printf(GREEN "\n%s, Your account has been created successfully as %s!\n" RESET, user.fullName, "Student");
            else
                printf(RED "\nSorry! Something went wrong :(" RESET);
            fclose(fp);
        }
        Sleep(1000);
        system("cls");
        main();
        break;
    case 2: // loginModule
        system("cls");
        {
            char email[50], password[50];
            int found = 0;
            FILE *fp;

            printf(CYAN "Enter your email:\t" RESET);
            takeInput(email);

            printf(CYAN "Enter your password:\t" RESET);
            takePassword(password);

            fp = fopen("user.txt", "r");
            if (fp == NULL)
            {
                printf(RED "\nDatabase not found! Please sign up first.\n" RESET);
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

            if (found)
            {
                printf(GREEN "\nLogin successful! Welcome, %s.\n" RESET, user.fullName);
                printf(CYAN "\nYour Details:\n" RESET);
                printf("Full Name:\t %s\n", user.fullName);
                printf("Email:\t %s\n", user.email);
                printf("Phone:\t %s\n", user.phone);
                printf("Role:\t %s\n", "Student");
            }
            else
            {
                printf(RED "\nInvalid email or password.\n" RESET);
                Sleep(1000);
                system("cls");
                main();
            }

            // handleRoleMenu(user.role);
            break;
        case 3: // ecitModule
            printf(GREEN "\t\t\tBye Bye :)\n" RESET);
            system("exit");
            break;

        default:
            printf(RED "\nInvalid choice. Please enter between 1, 2, or 3.\n" RESET);
            Sleep(1000);
            system("cls");
            main();
            break;
        }
    }
    return 0;
}