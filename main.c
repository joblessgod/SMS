// Standard Libraries
#include <stdio.h>
#include <windows.h>

// Custom Libraries
#include "mainMenu.h"
#include "helper.h"

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
    char role[50];
    char phone[50];
};

int checkEmailExits(struct User user)
{
    char line[300];
    FILE *fpCheck = fopen("user.txt", "r");
    struct User checkUser;
    if (fpCheck == NULL)
    {
        printf(RED "Error opening file" RESET);
        pressKeyToContinue();
        return 0;
    }
    // checks if email already exits or not
    while (fgets(line, sizeof(line), fpCheck) != NULL)
    {
        // Parse only the email field (skip first field, get second field)
        if (sscanf(line, "%*[^,],%49[^,]", checkUser.email) == 1)
        {
            printf("Checking email: %s\n", checkUser.email);

            // Compare emails
            if (strcmp(checkUser.email, user.email) == 0)
            {
                printf(RED "\nEmail already exists! Please try a different email.\n" RESET);
                fclose(fpCheck); // Close file before returning
                pressKeyToContinue();
                return 1; // Email exists
            }
        }
    }
}

int main()
{
    SetConsoleTitle("Student Management System | Team Sapphire"); // Changes the Terminal name into this
    struct User user;
    int choice;

    system("cls");
    showMainMenu();

    printf(CYAN "Enter your choice: " RESET);
    scanf("%d", &choice);
    while ((getchar()) != '\n')
        ;
    switch (choice)
    {
    case 1: // signupModule
        system("cls");

        char confirmPassword[50];
        printf(CYAN "=== Sign Up ===\n" RESET);

        printf(CYAN "Enter your fullName:\t" RESET);
        takeInput(user.fullName);

        printf(CYAN "Enter your email:\t" RESET);
        takeInput(user.email);
        checkEmailExits(user);

        printf(CYAN "Enter your phone:\t" RESET);
        takeInput(user.phone);

        printf(CYAN "\bEnter password:\t" RESET);
        takePassword(user.password);

        printf(CYAN "\b\nEnter confirm password:\t" RESET);
        takePassword(confirmPassword);

        if (strcmp(user.password, confirmPassword) != 0)
        {
            printf(RED "\nPassword not matched!" RESET);
            pressKeyToContinue();
            return 0;
        }
        else
        {
            FILE *fp = fopen("user.txt", "a+");
            if (fp == NULL)
            {
                printf(RED "Error opening file" RESET);
                pressKeyToContinue();
                return 0;
            }
            int written = fprintf(fp, "%s,%s,%s,%s,%s\n", user.fullName, user.email, user.phone, "student", user.password);

            if (written > 0)
            {
                printf(GREEN "\n%s, Your account has been created successfully as %s!\n" RESET, user.fullName, "Student");
            }
            else
            {
                printf(RED "\nSorry! Something went wrong :(" RESET);
            }
            fclose(fp);
            pressKeyToContinue();
        }
        break;
    case 2: // loginModule
        system("cls");

        char email[50], password[50];
        int found = 0;

        printf(CYAN "Enter your email:\t" RESET);
        takeInput(email);

        printf(CYAN "Enter your password:\t" RESET);
        takePassword(password);

        // Read from TXT file
        FILE *fp = fopen("user.txt", "r");
        if (fp == NULL)
        {
            printf(RED "No users found. Please sign up first!\n" RESET);
            pressKeyToContinue();
            return 0;
        }

        char line[300];
        while (fgets(line, sizeof(line), fp))
        {
            // Parse TXT line
            sscanf(line, "%49[^,],%49[^,],%49[^,],%49[^,],%49[^\n]", user.fullName, user.email, user.phone, user.role, user.password);
            if (strcmp(user.email, email) == 0 && strcmp(user.password, password) == 0)
            {
                found = 1;
                break;
            }
        }
        fclose(fp);

        if (found)
        {
            system("cls");
            printf(GREEN "Login successful! Welcome, %s.\n" RESET, user.fullName);
            printf(CYAN "\nYour Details:\n" RESET);
            printf("Full Name:\t %s\n", user.fullName);
            printf("Email:\t %s\n", user.email);
            printf("Phone:\t %s\n", user.phone);
            printf("Role:\t %s\n", "Student");
        }
        else
        {
            printf(RED "\nInvalid email or password!\n" RESET);
        }

        pressKeyToContinue();
        break;
    case 3: // exitModule
        terminateProgram();
        break;

    default:
        printf(RED "\nInvalid choice. Please enter between 1, 2, or 3.\n" RESET);
        Sleep(1000);
        system("cls");
        main();
        break;
    }
    return 0;
}