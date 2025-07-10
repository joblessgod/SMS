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
    char role[50];
    char phone[50];
};

int main()
{
    SetConsoleTitle("Student Management System | Team Sapphire"); // ignroe it
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

        printf(CYAN "Enter your phone:\t" RESET);
        takeInput(user.phone);

        printf(CYAN "\bEnter password:\t" RESET);
        takePassword(user.password);

        printf(CYAN "\b\nEnter confirm password:\t" RESET);
        takePassword(confirmPassword);

        if (strcmp(user.password, confirmPassword) != 0)
        {
            printf(RED "\nPassword not matched!" RESET);
            printf("\nPress any key to continue...");
            getchar();
            return 0; // Return to main menu instead of recursive cal 0l
        }
        else
        {

            FILE *fp = fopen("user.txt", "a+");
            if (fp == NULL)
            {
                printf(RED "Error opening file" RESET);
                printf("\nPress any key to continue...");
                getchar();
                return 0;
            }
            int written = fprintf(fp, "%s,%s,%s,%s,%s\n", user.fullName, user.email, user.phone, "student", user.password);

            // // size_t written = fwrite(&user, sizeof(struct User), 1, fp);
            // // size_t written = fprintf(fp, "Full Name: %s\nEmail: %s\nPhone: %s\nRole: %s\nPassword: %s\n",
            // //         user.fullName, user.email, user.phone, "Student", user.password);

            // size_t written = fprintf(fp, "%s,%s,%s,%s,%s\n", user.fullName, user.email, user.phone, "student", user.password);

            if (written > 0)
            {
                printf(GREEN "\n%s, Your account has been created successfully as %s!\n" RESET, user.fullName, "Student");
            }
            else
            {
                printf(RED "\nSorry! Something went wrong :(" RESET);
            }
            fclose(fp);
            printf("\nPress any key to continue...");
            getchar();
        }
        break;
    case 2: // loginModule
        system("cls");
        {
            char email[50], password[50];
            int found = 0;

            printf(CYAN "Enter your email:\t" RESET);
            takeInput(email);

            printf(CYAN "Enter your password:\t" RESET);
            takePassword(password);

            // Read from CSV file
            FILE *fp = fopen("user.txt", "r");
            if (fp == NULL)
            {
                printf(RED "No users found. Please sign up first!\n" RESET);
                printf("\nPress any key to continue...");
                getchar();
                return 0;
            }

            char line[300];
            while (fgets(line, sizeof(line), fp))
            {
                // Parse CSV line
                sscanf(line, "%49[^,],%49[^,],%49[^,],%49[^,],%49[^\n]", user.fullName, user.email, user.phone, user.role, user.password);
                if (strcmp(user.email, email) == 0 && strcmp(user.password, password) == 0)
                {
                    found = 1;
                    break;
                }
            }
            // while (fread(&user, sizeof(struct User), 1, fp))
            // {
            //     if (strcmp(user.email, email) == 0 && strcmp(user.password, password) == 0)
            //     {
            //         found = 1;
            //         break;
            //     }
            // }
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
                printf(RED "\nInvalid email or password!\n" RESET);
            }

            printf("\nPress any key to continue...");
            getchar();
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