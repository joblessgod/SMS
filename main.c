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
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"

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
void createDefaultAdmin();
void showRoleBasedMenu(const User *user);
void showStudentMenu(const User *user);
void showTeacherMenu(const User *user);
void showAdminMenu(const User *user);
int changeUserRole(const char *email, const char *newRole);
void listAllUsers();

int main()
{
    SetConsoleTitle("Student Management System | Team Sapphire");
    User user;
    createDefaultAdmin();
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
                    showRoleBasedMenu(&loggedInUser);
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
                showRoleBasedMenu(&loggedInUser);
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
    if (len < 7 || len > 11)
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

void createDefaultAdmin()
{
    FILE *file = fopen(USER_FILE, "r");
    if (file == NULL)
    {
        // File doesn't exist, create default admin
        file = fopen(USER_FILE, "w");
        if (file != NULL)
        {
            fprintf(file, "System Admin,admin@system.com,1234567890,Admin,admin123\n");
            fclose(file);
            printf(GREEN "Default admin account created!\n");
            printf(CYAN "Email: admin@system.com\n");
            printf(CYAN "Password: admin123\n" RESET);
        }
    }
    else
    {
        // Check if admin exists
        char line[MAX_LINE_LENGTH];
        User tempUser;
        int adminExists = 0;

        while (fgets(line, sizeof(line), file) != NULL)
        {
            if (sscanf(line, "%49[^,],%49[^,],%19[^,],%19[^,],%49[^\n]",
                       tempUser.fullName, tempUser.email, tempUser.phone,
                       tempUser.role, tempUser.password) == 5)
            {

                if (strcmp(tempUser.role, "Admin") == 0)
                {
                    adminExists = 1;
                    break;
                }
            }
        }
        fclose(file);

        if (!adminExists)
        {
            // Add default admin
            file = fopen(USER_FILE, "a");
            if (file != NULL)
            {
                fprintf(file, "System Admin,admin@system.com,1234567890,Admin,admin123\n");
                fclose(file);
                printf(GREEN "Default admin account created!\n");
                printf(CYAN "Email: admin@system.com\n");
                printf(CYAN "Password: admin123\n" RESET);
            }
        }
    }
}
void showRoleBasedMenu(const User *user)
{
    if (strcmp(user->role, "Admin") == 0)
    {
        showAdminMenu(user);
    }
    else if (strcmp(user->role, "Teacher") == 0)
    {
        showTeacherMenu(user);
    }
    else if (strcmp(user->role, "Student") == 0)
    {
        showStudentMenu(user);
    }
    else
    {
        printf(RED "Error: Unknown role '%s'\n" RESET, user->role);
    }
}

void showStudentMenu(const User *user)
{
    int choice;
    do
    {
        clearScreen();
        printf(CYAN "=== Student Dashboard - Welcome %s ===\n" RESET, user->fullName);
        printf(BLUE "|-------------------------------------|\n");
        printf("|            STUDENT MENU             |\n");
        printf("|-------------------------------------|\n");
        printf("| 1. View My Marks                   |\n");
        printf("| 2. View My Assignments             |\n");
        printf("| 3. View My Fee Status              |\n");
        printf("| 4. Update Profile                  |\n");
        printf("| 5. View My Attendance              |\n");
        printf("| 6. Submit Assignment               |\n");
        printf("| 7. Logout                          |\n");
        printf("|-------------------------------------|\n" RESET);
        printf(CYAN "Enter your choice: " RESET);

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
                ;
            choice = -1;
        }
        else
        {
            while (getchar() != '\n')
                ;
        }

        switch (choice)
        {
        case 1:
            printf(YELLOW "\n[Student Marks Module] - Coming Soon!\n" RESET);
            pressKeyToContinue();
            break;
        case 2:
            printf(YELLOW "\n[Assignment Module] - Coming Soon!\n" RESET);
            pressKeyToContinue();
            break;
        case 3:
            printf(YELLOW "\n[Fee Module] - Coming Soon!\n" RESET);
            pressKeyToContinue();
            break;
        case 4:
            printf(YELLOW "\n[Profile Update] - Coming Soon!\n" RESET);
            pressKeyToContinue();
            break;
        case 5:
            printf(YELLOW "\n[Attendance Module] - Coming Soon!\n" RESET);
            pressKeyToContinue();
            break;
        case 6:
            printf(YELLOW "\n[Assignment Submission] - Coming Soon!\n" RESET);
            pressKeyToContinue();
            break;
        case 7:
            printf(GREEN "\nLogging out...\n" RESET);
            return;
        default:
            printf(RED "\nInvalid choice! Please try again.\n" RESET);
            Sleep(1500);
        }
    } while (choice != 7);
}

void showTeacherMenu(const User *user)
{
    int choice;
    do
    {
        //clearScreen();
        printf(CYAN "=== Teacher Dashboard - Welcome %s ===\n" RESET, user->fullName);
        printf(MAGENTA "|-------------------------------------|\n");
        printf("|            TEACHER MENU             |\n");
        printf("|-------------------------------------|\n");
        printf("| 1. Manage Student Marks            |\n");
        printf("| 2. Create/View Assignments         |\n");
        printf("| 3. View Student List               |\n");
        printf("| 4. Mark Attendance                 |\n");
        printf("| 5. Grade Assignments               |\n");
        printf("| 6. Generate Reports                |\n");
        printf("| 7. Update Profile                  |\n");
        printf("| 8. Logout                          |\n");
        printf("|-------------------------------------|\n" RESET);
        printf(CYAN "Enter your choice: " RESET);

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
                ;
            choice = -1;
        }
        else
        {
            while (getchar() != '\n')
                ;
        }

        switch (choice)
        {
        case 1:
            printf(YELLOW "\n[Marks Management] - Coming Soon!\n" RESET);
            pressKeyToContinue();
            break;
        case 2:
            printf(YELLOW "\n[Assignment Creation] - Coming Soon!\n" RESET);
            pressKeyToContinue();
            break;
        case 3:
            printf(YELLOW "\n[Student List] - Coming Soon!\n" RESET);
            pressKeyToContinue();
            break;
        case 4:
            printf(YELLOW "\n[Attendance Management] - Coming Soon!\n" RESET);
            pressKeyToContinue();
            break;
        case 5:
            printf(YELLOW "\n[Assignment Grading] - Coming Soon!\n" RESET);
            pressKeyToContinue();
            break;
        case 6:
            printf(YELLOW "\n[Report Generation] - Coming Soon!\n" RESET);
            pressKeyToContinue();
            break;
        case 7:
            printf(YELLOW "\n[Profile Update] - Coming Soon!\n" RESET);
            pressKeyToContinue();
            break;
        case 8:
            printf(GREEN "\nLogging out...\n" RESET);
            return;
        default:
            printf(RED "\nInvalid choice! Please try again.\n" RESET);
            Sleep(1500);
        }
    } while (choice != 8);
}

void showAdminMenu(const User *user)
{
    int choice;
    do
    {
        clearScreen();
        printf(CYAN "=== Admin Dashboard - Welcome %s ===\n" RESET, user->fullName);
        printf(RED "|-------------------------------------|\n");
        printf("|             ADMIN MENU              |\n");
        printf("|-------------------------------------|\n");
        printf("| 1. User Management                 |\n");
        printf("| 2. Change User Roles               |\n");
        printf("| 3. View All Users                  |\n");
        printf("| 4. Manage Student Records          |\n");
        printf("| 5. Manage Teacher Records          |\n");
        printf("| 6. Fee Management                  |\n");
        printf("| 7. System Reports                  |\n");
        printf("| 8. Database Backup                 |\n");
        printf("| 9. Settings                        |\n");
        printf("| 10. Logout                         |\n");
        printf("|-------------------------------------|\n" RESET);
        printf(CYAN "Enter your choice: " RESET);

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
                ;
            choice = -1;
        }
        else
        {
            while (getchar() != '\n')
                ;
        }

        switch (choice)
        {
        case 1:
            printf(YELLOW "\n[User Management] - Coming Soon!\n" RESET);
            pressKeyToContinue();
            break;
        case 2:
        {
            clearScreen();
            printf(CYAN "=== Change User Role ===\n" RESET);
            char email[MAX_EMAIL_LENGTH];
            char newRole[MAX_ROLE_LENGTH];

            printf(CYAN "Enter user email: " RESET);
            takeInput(email, MAX_EMAIL_LENGTH);

            printf(CYAN "\nAvailable Roles:\n" RESET);
            printf("1. Student\n2. Teacher\n3. Admin\n");
            printf(CYAN "Enter new role: " RESET);
            takeInput(newRole, MAX_ROLE_LENGTH);

            if (changeUserRole(email, newRole))
            {
                printf(GREEN "\nRole updated successfully!\n" RESET);
            }
            else
            {
                printf(RED "\nFailed to update role.\n" RESET);
            }
            pressKeyToContinue();
        }
        break;
        case 3:
            listAllUsers();
            pressKeyToContinue();
            break;
        case 4:
            printf(YELLOW "\n[Student Records Management] - Coming Soon!\n" RESET);
            pressKeyToContinue();
            break;
        case 5:
            printf(YELLOW "\n[Teacher Records Management] - Coming Soon!\n" RESET);
            pressKeyToContinue();
            break;
        case 6:
            printf(YELLOW "\n[Fee Management] - Coming Soon!\n" RESET);
            pressKeyToContinue();
            break;
        case 7:
            printf(YELLOW "\n[System Reports] - Coming Soon!\n" RESET);
            pressKeyToContinue();
            break;
        case 8:
            printf(YELLOW "\n[Database Backup] - Coming Soon!\n" RESET);
            pressKeyToContinue();
            break;
        case 9:
            printf(YELLOW "\n[Settings] - Coming Soon!\n" RESET);
            pressKeyToContinue();
            break;
        case 10:
            printf(GREEN "\nLogging out...\n" RESET);
            return;
        default:
            printf(RED "\nInvalid choice! Please try again.\n" RESET);
            Sleep(1500);
        }
    } while (choice != 10);
}
int changeUserRole(const char *email, const char *newRole)
{
    FILE *file = fopen(USER_FILE, "r");
    FILE *tempFile = fopen("temp_users.txt", "w");
    if (file == NULL || tempFile == NULL)
    {
        printf(RED "Error: Unable to open files.\n" RESET);
        if (file)
            fclose(file);
        if (tempFile)
            fclose(tempFile);
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    User tempUser;
    int found = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "%49[^,],%49[^,],%19[^,],%19[^,],%49[^\n]",
                   tempUser.fullName, tempUser.email, tempUser.phone,
                   tempUser.role, tempUser.password) == 5)
        {

            if (strcmp(tempUser.email, email) == 0)
            {
                strcpy(tempUser.role, newRole);
                found = 1;
            }
            fprintf(tempFile, "%s,%s,%s,%s,%s\n",
                    tempUser.fullName, tempUser.email, tempUser.phone,
                    tempUser.role, tempUser.password);
        }
    }
    fclose(file);
    fclose(tempFile);
    if (found)
    {
        remove(USER_FILE);
        rename("temp_users.txt", USER_FILE);
        return 1;
    }
    else
    {
        remove("temp_users.txt");
        printf(RED "Error: User with email '%s' not found.\n" RESET, email);
        return 0;
    }
}

void listAllUsers()
{

    clearScreen();
    printf(CYAN "=== All Registered Users ===\n" RESET);
    FILE *file = fopen(USER_FILE, "r");
    if (file == NULL)
    {
        printf(RED "No users found or unable to open file.\n" RESET);
        return;
    }
    char line[MAX_LINE_LENGTH];
    User tempUser;
    int userCount = 0;
    printf(BLUE "%-20s %-25s %-15s %-10s\n", "Name", "Email", "Phone", "Role");
    printf("---------------------------------------------------------------------\n" RESET);

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "%49[^,],%49[^,],%19[^,],%19[^,],%49[^\n]",
                   tempUser.fullName, tempUser.email, tempUser.phone,
                   tempUser.role, tempUser.password) == 5)
        {

            userCount++;
            printf("%-20s %-25s %-15s ", tempUser.fullName, tempUser.email, tempUser.phone);

            if (strcmp(tempUser.role, "Admin") == 0)
            {
                printf(RED "%-10s\n" RESET, tempUser.role);
            }
            else if (strcmp(tempUser.role, "Teacher") == 0)
            {
                printf(MAGENTA "%-10s\n" RESET, tempUser.role);
            }
            else
            {
                printf(BLUE "%-10s\n" RESET, tempUser.role);
            }
        }
    }
    fclose(file);
    printf(CYAN "\nTotal Users: %d\n" RESET, userCount);
}