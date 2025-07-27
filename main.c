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

//
#define MAX_NAME_LENGTH 50
#define MAX_EMAIL_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define MAX_PHONE_LENGTH 20
#define MAX_ROLE_LENGTH 20
#define MAX_LINE_LENGTH 300
#define USER_FILE "users.txt"

typedef struct
{
    char fullName[MAX_NAME_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char role[MAX_ROLE_LENGTH];
    char phone[MAX_PHONE_LENGTH];
} User;

// Function prototypes
int checkEmailExists(const char *email);
int signupUser(const User *user);
int loginUser(const char *email, const char *password, User *loggedInUser);
void displayUserInfo(const User *user);
int isValidEmail(const char *email);
int isValidPhone(const char *phone);

int checkEmailExists(const char *email)
{
    FILE *file = fopen(USER_FILE, "r");
    if (file == NULL)
    {
        return 0; // File doesn't exist, so email doesn't exist
    }

    char line[MAX_LINE_LENGTH];
    User tempUser;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "%49[^,],%49[^,],%19[^,],%19[^,],%49[^\n]",
                   tempUser.fullName, tempUser.email, tempUser.phone,
                   tempUser.role, tempUser.password) == 5)
        {

            if (strcmp(tempUser.email, email) == 0)
            {
                fclose(file);
                return 1; // Email exists
            }
        }
    }

    fclose(file);
    return 0; // Email doesn't exist
}
int isValidEmail(const char *email)
{
    // Simple email validation - contains @ and at least one dot after @
    char *atSign = strchr(email, '@');
    if (atSign == NULL || atSign == email)
    {
        return 0;
    }

    char *dot = strchr(atSign, '.');
    return (dot != NULL && dot > atSign + 1 && strlen(dot) > 1);
}

int isValidPhone(const char *phone)
{
    // Simple phone validation - only digits and reasonable length
    int len = strlen(phone);
    if (len < 7 || len > 15)
    {
        return 0;
    }

    for (int i = 0; i < len; i++)
    {
        if (phone[i] < '0' || phone[i] > '9')
        {
            return 0;
        }
    }
    return 1;
}

int signupUser(const User *user)
{
    FILE *file = fopen(USER_FILE, "a");
    if (file == NULL)
    {
        printf(RED "Error: Unable to open file for writing.\n" RESET);
        return 0;
    }

    int result = fprintf(file, "%s,%s,%s,%s,%s\n",
                         user->fullName, user->email, user->phone,
                         user->role, user->password);

    fclose(file);

    if (result > 0)
    {
        printf(GREEN "\nSuccess! Account created for %s as %s.\n" RESET,
               user->fullName, user->role);
        return 1;
    }
    else
    {
        printf(RED "\nError: Failed to create account.\n" RESET);
        return 0;
    }
}
int loginUser(const char *email, const char *password, User *loggedInUser)
{
    FILE *file = fopen(USER_FILE, "r");
    if (file == NULL)
    {
        printf(RED "No users found. Please sign up first!\n" RESET);
        return 0;
    }

    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "%49[^,],%49[^,],%19[^,],%19[^,],%49[^\n]",
                   loggedInUser->fullName, loggedInUser->email,
                   loggedInUser->phone, loggedInUser->role,
                   loggedInUser->password) == 5)
        {

            if (strcmp(loggedInUser->email, email) == 0 &&
                strcmp(loggedInUser->password, password) == 0)
            {
                fclose(file);
                return 1; // Login successful
            }
        }
    }

    fclose(file);
    return 0; // Login failed
}

void displayUserInfo(const User *user)
{
    clearScreen();
    printf(GREEN "Login successful! Welcome, %s.\n" RESET, user->fullName);
    printf(CYAN "\n=== Your Profile ===\n" RESET);
    printf("Name:  %s\n", user->fullName);
    printf("Email: %s\n", user->email);
    printf("Phone: %s\n", user->phone);
    printf("Role:  %s\n", user->role);
}

int main()
{
    SetConsoleTitle("Student Management System | Team Sapphire"); // "Terminal" rename into this
    User user;
    int choice;

    do
    {
        clearScreen();
        showMainMenu();

        if (scanf("%d", &choice) != 1)
        {
            // Clear invalid input
            while (getchar() != '\n')
                ;
            choice = -1;
        }
        else
        {
            while (getchar() != '\n')
                ; // Clear input buffer
        }

        switch (choice)
        {
        case 1: // Sign Up
            clearScreen();
            printf(CYAN "=== Create New Account ===\n" RESET);

            // Get full name
            printf(CYAN "Enter your full name: " RESET);
            takeInput(user.fullName, MAX_NAME_LENGTH);

            if (strlen(user.fullName) == 0)
            {
                printf(RED "Error: Name cannot be empty.\n" RESET);
                pressKeyToContinue();
                continue;
            }
            // Get and validate email
            do
            {
                printf(CYAN "Enter your email: " RESET);
                takeInput(user.email, MAX_EMAIL_LENGTH);

                if (!isValidEmail(user.email))
                {
                    printf(RED "Error: Please enter a valid email address.\n" RESET);
                    continue;
                }

                if (checkEmailExists(user.email))
                {
                    printf(RED "Error: Email already exists. Please use a different email.\n" RESET);
                    continue;
                }
                break;
            } while (1);

            // Get and validate phone
            do
            {
                printf(CYAN "Enter your phone number: " RESET);
                takeInput(user.phone, MAX_PHONE_LENGTH);

                if (!isValidPhone(user.phone))
                {
                    printf(RED "Error: Please enter a valid phone number (7-15 digits).\n" RESET);
                    continue;
                }
                break;
            } while (1);

            // Set role
            strcpy(user.role, "Student");

            // Get password
            char confirmPassword[MAX_PASSWORD_LENGTH];
            do
            {
                printf(CYAN "Enter password: " RESET);
                takePassword(user.password, MAX_PASSWORD_LENGTH);

                if (strlen(user.password) < 4)
                {
                    printf(RED "\nError: Password must be at least 4 characters long.\n" RESET);
                    continue;
                }

                printf(CYAN "\nConfirm password: " RESET);
                takePassword(confirmPassword, MAX_PASSWORD_LENGTH);

                if (strcmp(user.password, confirmPassword) != 0)
                {
                    printf(RED "\nError: Passwords do not match.\n" RESET);
                    continue;
                }
                break;
            } while (1);
            // Create account
            if (signupUser(&user))
            {
                printf(YELLOW "\nRedirecting to login...\n" RESET);
                Sleep(2000);

                // auto-login after signup
                User loggedInUser;
                if (loginUser(user.email, user.password, &loggedInUser))
                {
                    displayUserInfo(&loggedInUser);
                    pressKeyToContinue();
                }
            }
            else
            {
                pressKeyToContinue();
            }
            break;
        case 2: // Login
            clearScreen();
            printf(CYAN "=== Login to Your Account ===\n" RESET);
            char email[MAX_EMAIL_LENGTH];
            char password[MAX_PASSWORD_LENGTH];

            printf(CYAN "Enter your email: " RESET);
            takeInput(email, MAX_EMAIL_LENGTH);

            printf(CYAN "Enter your password: " RESET);
            takePassword(password, MAX_PASSWORD_LENGTH);

            User loggedInUser;
            if (loginUser(email, password, &loggedInUser))
            {
                displayUserInfo(&loggedInUser);
            }
            else
            {
                printf(RED "\nError: Invalid email or password.\n" RESET);
            }
            pressKeyToContinue();
            break;
        case 3: // exit
            terminateProgram();
            break;

        default:
            printf(RED "\nError: Invalid choice. Please enter 1, 2, or 3.\n" RESET);
            Sleep(1500);
            break;
        }
    } while (choice != 3);
    return 0;
}