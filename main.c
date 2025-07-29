// Standard Libraries
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>

// Custom Libraries
#include "library/mainMenu.h"
#include "library/helper.h"

// Color Definitions
#define RESET "\033[0m"
#define RED "\033[31m"
#define CYAN "\033[36m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"

// User's structures and constants
#define MAX_NAME_LENGTH 50
#define MAX_EMAIL_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define MAX_PHONE_LENGTH 20
#define MAX_ROLE_LENGTH 20
#define MAX_LINE_LENGTH 300

// File names for storing data
#define USER_FILE "users.txt"
#define MARKS_FILE "marks.txt"
#define FEES_FILE "fees.txt"
#define ATTENDANCE_FILE "attendance.txt"

// USER's data strucutre
typedef struct
{
    char fullName[MAX_NAME_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char role[MAX_ROLE_LENGTH];
    char phone[MAX_PHONE_LENGTH];
} User;

// User's prototypes
int checkEmailExists(const char *email);
int signupUser(const User *user);
int loginUser(const char *email, const char *password, User *loggedInUser);
void displayUserInfo(const User *user);
int isValidEmail(const char *email);
int isValidPhone(const char *phone);
void createDefaultAdmin();

// System's prototype
void showRoleBasedMenu(const User *user);
void showStudentMenu(const User *user);
void showTeacherMenu(const User *user);
void showAdminMenu(const User *user);
void generateMarksheet(const User *currentUser);
int getStudentMarksForMarksheet(const char *email, int marks[5]);
int getStudentAttendanceForMarksheet(const char *email, int present[5], int total[5]);
int getStudentFeesForMarksheet(const char *email, float amounts[5], float paid[5]);
User *findStudentByEmail(const char *email);

void generateUserReport(const User *user);
void generateStudentReport(const User *user);
void generateTeacherReport(const User *user);
void generateAdminReport(const User *user);

// Student's functions prototype
void listAllUsers();
void viewMyMarks(const char *studentEmail);
void viewMyFeeStatus(const char *studentEmail);
void viewMyAttendance(const char *studentEmail);

// Teacher's functions prototype
void addStudentMarks(const char *teacherEmail);
void markStudentAttendance(const char *teacherEmail);
void listStudentOnly();

// Admin's functions prototype
int changeUserRole(const char *email, const char *newRole);
void manageFees(const char *adminEmail);
int deleteUserData(const char *userEmail);
void showUserDeletionMenu();
void listTeacherOnly();
void listAdminOnly();

// THE MAIN FUNCTION - GOAT
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
                    printf(RED "Error: Please enter a valid phone number (10 digits).\n" RESET);
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

// Checks if your input email already exits in database or not
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

// Checks if your input email is valid or not
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

// Checks if your input phone number is valid or not
int isValidPhone(const char *phone)
{
    // Simple phone validation - only digits and reasonable length
    int len = strlen(phone);
    if (len!= 10)
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

// Grabs User info. Creates account in database and Logs in through it.
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

// Grabs User info. Logs in through database
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

// Grabs User info. Throws in format
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

// creates a default id pass for admin login
void createDefaultAdmin()
{
    FILE *file = fopen(USER_FILE, "r");
    int adminExists = 0;

    if (file != NULL)
    {
        char line[MAX_LINE_LENGTH];
        User tempUser;

        while (fgets(line, sizeof(line), file) != NULL)
        {
            if (sscanf(line, "%49[^,],%49[^,],%19[^,],%19[^,],%49[^\n]",
                       tempUser.fullName, tempUser.email, tempUser.phone,
                       tempUser.role, tempUser.password) == 5)
            {
                if (strcmp(tempUser.email, "admin@system.com") == 0)
                {
                    adminExists = 1;
                    break;
                }
            }
        }
        fclose(file);
    }

    if (!adminExists)
    {
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

// Shows menu via user's role
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

// Student's Menu
void showStudentMenu(const User *user)
{
    int choice;
    do
    {
        clearScreen();
        displayUserInfo(user);
        printf(CYAN "=== Student Dashboard - Welcome %s ===\n" RESET, user->fullName);
        printf(BLUE "|-------------------------------------|\n");
        printf("|            STUDENT MENU             |\n");
        printf("|-------------------------------------|\n");
        printf("| 1. View My Marks                    |\n");
        printf("| 2. View My Fee Status               |\n");
        printf("| 3. View My Attendance               |\n");
        printf("| 4. Generate Marksheet               |\n");
        printf("| 5. Logout                           |\n");
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
            viewMyMarks(user->email);
            break;
        case 2:
            viewMyFeeStatus(user->email);
            break;
        case 3:
            viewMyAttendance(user->email);
            break;
        case 4:
            generateMarksheet(user);
            break;
        case 5:
            printf(GREEN "\nLogging out...\n" RESET);
            return;
        default:
            printf(RED "\nInvalid choice! Please try again.\n" RESET);
            Sleep(1500);
        }
    } while (choice != 7);
}

// Teacher's Menu
void showTeacherMenu(const User *user)
{
    int choice;
    do
    {
        clearScreen();
        displayUserInfo(user);
        printf(CYAN "=== Teacher Dashboard - Welcome %s ===\n" RESET, user->fullName);
        printf(MAGENTA "|-------------------------------------|\n");
        printf("|            TEACHER MENU             |\n");
        printf("|-------------------------------------|\n");
        printf("| 1. View Student List                |\n");
        printf("| 2. Manage Marks                     |\n");
        printf("| 3. Manage Attendance                |\n");
        printf("| 4. Generate Reports                 |\n");
        printf("| 5. Generate Marksheet               |\n");
        printf("| 6. Logout                           |\n");
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
            listStudentOnly();
            break;
        case 2:
            addStudentMarks(user->email);
            break;
        case 3:
            markStudentAttendance(user->email);
            break;
        case 4:
            generateUserReport(user);
            break;
        case 5:
            generateMarksheet(user);
            break;
        case 6:
            printf(GREEN "\nLogging out...\n" RESET);
            return;
        default:
            printf(RED "\nInvalid choice! Please try again.\n" RESET);
            Sleep(1500);
        }
    } while (choice != 8);
}

// Admin's Menu
void showAdminMenu(const User *user)
{
    int choice;
    do
    {
        clearScreen();
        displayUserInfo(user);
        printf(CYAN "=== Admin Dashboard - Welcome %s ===\n" RESET, user->fullName);
        printf(RED "|-------------------------------------|\n");
        printf("|             ADMIN MENU              |\n");
        printf("|-------------------------------------|\n");
        printf("| 1. User Management                  |\n");
        printf("| 2. Role Management                  |\n");
        printf("| 3. Fee Management                   |\n");
        printf("| 4. View All Users                   |\n");
        printf("| 5. List of Student Only             |\n");
        printf("| 6. List of Teacher Only             |\n");
        printf("| 7. List of Admin Only               |\n");
        printf("| 8. Generate Reports                 |\n");
        printf("| 9. Logout                           |\n");
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
            showUserDeletionMenu();
            break;
        case 2:
        {
            clearScreen();
            printf(CYAN "=== Change User Role ===\n" RESET);
            char email[MAX_EMAIL_LENGTH];
            char newRole[MAX_ROLE_LENGTH];
            int roleChoice;

            do
            {
                printf(CYAN "Enter user email: " RESET);
                takeInput(email, MAX_EMAIL_LENGTH);
                if (strlen(email) == 0)
                {
                    printf(RED "Error: Email cannot be empty.\n" RESET);
                }
            } while (strlen(email) == 0);

            printf(CYAN "\nAvailable Roles:\n" RESET);
            printf("1. Student\n2. Teacher\n3. Admin\n");
            printf(CYAN "Enter role choice (1-3): " RESET);

            if (scanf("%d", &roleChoice) != 1)
            {
                while (getchar() != '\n')
                    ;
                printf(RED "\nInvalid input!\n" RESET);
                pressKeyToContinue();
                break;
            }
            else
            {
                while (getchar() != '\n')
                    ;
            }

            switch (roleChoice)
            {
            case 1:
                strcpy(newRole, "Student");
                break;
            case 2:
                strcpy(newRole, "Teacher");
                break;
            case 3:
                strcpy(newRole, "Admin");
                break;
            default:
                printf(RED "\nInvalid choice! Please select 1-3.\n" RESET);
                pressKeyToContinue();
                break;
            }

            if (roleChoice >= 1 && roleChoice <= 3)
            {
                if (changeUserRole(email, newRole))
                {
                    printf(GREEN "\nRole updated successfully to %s!\n" RESET, newRole);
                }
                else
                {
                    printf(RED "\nFailed to update role.\n" RESET);
                }
            }
            pressKeyToContinue();
        }
        break;
        case 3:
            manageFees(user->email);
            break;
        case 4:
            listAllUsers();
            break;
        case 5:
            listStudentOnly();
            break;
        case 6:
            listTeacherOnly();
            break;
        case 7:
            listAdminOnly();
            break;
        case 8:
            generateUserReport(user);
            break;
        case 9:
            printf(GREEN "\nLogging out...\n" RESET);
            return;
        default:
            printf(RED "\nInvalid choice! Please try again.\n" RESET);
            Sleep(1500);
        }
    } while (choice != 10);
}

// change user's role
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
    pressKeyToContinue();
}

// Shows all users without filter
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
    pressKeyToContinue();
}

// Shows only Student's list
void listStudentOnly()
{

    clearScreen();
    printf(CYAN "=== Student List ===\n" RESET);
    FILE *file = fopen(USER_FILE, "r");
    if (file == NULL)
    {
        printf(RED "No users found or unable to open file.\n" RESET);
        return;
    }
    char line[MAX_LINE_LENGTH];
    User tempUser;
    int userCount = 0;
    printf(BLUE "%-4s %-20s %-25s %-15s %-10s\n", "No.", "Name", "Email", "Phone", "Role");
    printf("---------------------------------------------------------------------\n" RESET);

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "%49[^,],%49[^,],%19[^,],%19[^,],%49[^\n]",
                   tempUser.fullName, tempUser.email, tempUser.phone,
                   tempUser.role, tempUser.password) == 5)
        {

            if (strcmp(tempUser.role, "Student") == 0)
            {
                userCount++;
                printf("%-4d %-20s %-25s %-15s ", userCount, tempUser.fullName, tempUser.email, tempUser.phone);
            }
            if (strcmp(tempUser.role, "Student") == 0)
            {
                printf(BLUE "%-10s\n" RESET, tempUser.role);
            }
        }
    }
    fclose(file);
    printf(CYAN "\nTotal Users: %d\n" RESET, userCount);
    pressKeyToContinue();
}

// Shows only Teacher's list
void listTeacherOnly()
{

    clearScreen();
    printf(CYAN "=== Teacher List ===\n" RESET);
    FILE *file = fopen(USER_FILE, "r");
    if (file == NULL)
    {
        printf(RED "No users found or unable to open file.\n" RESET);
        return;
    }
    char line[MAX_LINE_LENGTH];
    User tempUser;
    int userCount = 0;
    printf(BLUE "%-4s %-20s %-25s %-15s %-10s\n", "No.", "Name", "Email", "Phone", "Role");
    printf("---------------------------------------------------------------------\n" RESET);

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "%49[^,],%49[^,],%19[^,],%19[^,],%49[^\n]",
                   tempUser.fullName, tempUser.email, tempUser.phone,
                   tempUser.role, tempUser.password) == 5)
        {

            if (strcmp(tempUser.role, "Teacher") == 0)
            {
                userCount++;
                printf("%-4d %-20s %-25s %-15s ", userCount, tempUser.fullName, tempUser.email, tempUser.phone);
            }
            if (strcmp(tempUser.role, "Teacher") == 0)
            {
                printf(MAGENTA "%-10s\n" RESET, tempUser.role);
            }
        }
    }
    fclose(file);
    printf(CYAN "\nTotal Users: %d\n" RESET, userCount);
    pressKeyToContinue();
}

// Shows only Admin's list
void listAdminOnly()
{

    clearScreen();
    printf(CYAN "=== Admin List ===\n" RESET);
    FILE *file = fopen(USER_FILE, "r");
    if (file == NULL)
    {
        printf(RED "No users found or unable to open file.\n" RESET);
        return;
    }
    char line[MAX_LINE_LENGTH];
    User tempUser;
    int userCount = 0;
    printf(BLUE "%-4s %-20s %-25s %-15s %-10s\n", "No.", "Name", "Email", "Phone", "Role");
    printf("---------------------------------------------------------------------\n" RESET);

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "%49[^,],%49[^,],%19[^,],%19[^,],%49[^\n]",
                   tempUser.fullName, tempUser.email, tempUser.phone,
                   tempUser.role, tempUser.password) == 5)
        {

            if (strcmp(tempUser.role, "Admin") == 0)
            {
                userCount++;
                printf("%-4d %-20s %-25s %-15s ", userCount, tempUser.fullName, tempUser.email, tempUser.phone);
            }
            if (strcmp(tempUser.role, "Admin") == 0)
            {
                printf(RED "%-10s\n" RESET, tempUser.role);
            }
        }
    }
    fclose(file);
    printf(CYAN "\nTotal Users: %d\n" RESET, userCount);
    pressKeyToContinue();
}

// Show Student's marks
void viewMyMarks(const char *studentEmail)
{
    clearScreen();
    printf(CYAN "=== My Marks ===\n" RESET);

    // Hardcoded subjects list
    char subjects[][20] = {"English", "Math", "Science", "History", "Computer"};
    int numSubjects = 5;

    FILE *file = fopen(MARKS_FILE, "r");
    if (file == NULL)
    {
        printf(RED "No marks found!\n" RESET);
        printf(YELLOW "Contact your teacher to add marks.\n" RESET);
        pressKeyToContinue();
        return;
    }

    char line[200];
    char email[50];
    int marks[5] = {-1, -1, -1, -1, -1}; // Initialize with -1 (not found)
    int found = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        // First, just get the email part
        if (sscanf(line, "%49[^,]", email) == 1)
        {
            if (strcmp(email, studentEmail) == 0)
            {
                // Found the student, now parse the marks
                sscanf(line, "%49[^,],%d,%d,%d,%d,%d",
                       email, &marks[0], &marks[1], &marks[2], &marks[3], &marks[4]);
                found = 1;
                break;
            }
        }
    }
    fclose(file);

    printf(BLUE "Subject          Marks    Grade\n");
    printf("--------------------------------\n" RESET);

    float total = 0;
    int count = 0;

    for (int i = 0; i < numSubjects; i++)
    {
        if (marks[i] == -1)
        {
            printf("%-15s  %-7s  %s\n", subjects[i], "N/A", "-");
        }
        else
        {
            // Determine grade
            char grade;
            if (marks[i] >= 90)
                grade = 'A';
            else if (marks[i] >= 80)
                grade = 'B';
            else if (marks[i] >= 70)
                grade = 'C';
            else if (marks[i] >= 60)
                grade = 'D';
            else
                grade = 'F';

            printf("%-15s  %-7d  %c\n", subjects[i], marks[i], grade);
            total += marks[i];
            count++;
        }
    }

    if (count > 0)
    {
        printf("--------------------------------\n");
        printf(GREEN "Average: %.1f\n" RESET, total / count);
    }

    if (!found)
    {
        printf(YELLOW "\nNo marks record found for your account!\n" RESET);
    }

    pressKeyToContinue();
}

// Show Student's fees status and remaining (for studnet)
void viewMyFeeStatus(const char *studentEmail)
{
    clearScreen();
    printf(CYAN "=== My Fee Status ===\n" RESET);

    FILE *file = fopen(FEES_FILE, "r");
    if (file == NULL)
    {
        printf(RED "No fee records found!\n" RESET);
        pressKeyToContinue();
        return;
    }

    char line[300];
    char email[50];
    float amounts[5] = {0}, paid[5] = {0}; // tuition, library, lab, sports, exam
    char feeTypes[][20] = {"Tuition", "Library", "Lab", "Sports", "Exam"};
    int found = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "%49[^,],%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
                   email, &amounts[0], &paid[0], &amounts[1], &paid[1],
                   &amounts[2], &paid[2], &amounts[3], &paid[3],
                   &amounts[4], &paid[4]) >= 1)
        {

            if (strcmp(email, studentEmail) == 0)
            {
                found = 1;
                break;
            }
        }
    }
    fclose(file);

    if (!found)
    {
        printf(YELLOW "No fee records found for your account.\n" RESET);
        pressKeyToContinue();
        return;
    }

    printf(BLUE "Fee Type         Amount    Paid      Due       Status\n");
    printf("----------------------------------------------------\n" RESET);

    float totalDue = 0;

    for (int i = 0; i < 5; i++)
    {
        if (amounts[i] > 0)
        {
            float due = amounts[i] - paid[i];
            char status[20];

            if (due <= 0)
            {
                strcpy(status, "Paid");
                printf(GREEN "%-15s  %-8.0f  %-8.0f  %-8.0f  %s\n" RESET,
                       feeTypes[i], amounts[i], paid[i], due, status);
            }
            else
            {
                strcpy(status, "Pending");
                printf(RED "%-15s  %-8.0f  %-8.0f  %-8.0f  %s\n" RESET,
                       feeTypes[i], amounts[i], paid[i], due, status);
                totalDue += due;
            }
        }
    }

    if (totalDue > 0)
    {
        printf("----------------------------------------------------\n");
        printf(RED "Total Due: Rs. %.0f\n" RESET, totalDue);
    }
    pressKeyToContinue();
}

// Teachers adds marks of subject fucntion (for teacher)
void addStudentMarks(const char *teacherEmail)
{
    clearScreen();
    printf(CYAN "=== Add/Update Student Marks ===\n" RESET);

    char studentEmail[50];
    char subjects[][20] = {"English", "Math", "Science", "History", "Computer"};
    int numSubjects = 5;
    int choice, newMarks;

    printf(CYAN "Enter student email: " RESET);
    takeInput(studentEmail, sizeof(studentEmail));

    // Check if student exists
    if (!checkEmailExists(studentEmail))
    {
        printf(RED "Student not found!\n" RESET);
        pressKeyToContinue();
        return;
    }

    // Show subjects
    printf(CYAN "\nSelect Subject:\n" RESET);
    for (int i = 0; i < numSubjects; i++)
    {
        printf("%d. %s\n", i + 1, subjects[i]);
    }

    printf(CYAN "Enter choice (1-%d): " RESET, numSubjects);
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > numSubjects)
    {
        printf(RED "Invalid choice!\n" RESET);
        while (getchar() != '\n')
            ;
        pressKeyToContinue();
        return;
    }
    while (getchar() != '\n')
        ;

    printf(CYAN "Enter marks (0-100): " RESET);
    if (scanf("%d", &newMarks) != 1 || newMarks < 0 || newMarks > 100)
    {
        printf(RED "Invalid marks!\n" RESET);
        while (getchar() != '\n')
            ;
        pressKeyToContinue();
        return;
    }
    while (getchar() != '\n')
        ;

    // Read existing data
    FILE *file = fopen(MARKS_FILE, "r");
    FILE *tempFile = fopen("temp_marks.txt", "w");

    if (tempFile == NULL)
    {
        printf(RED "Error creating temporary file!\n" RESET);
        if (file)
            fclose(file);
        pressKeyToContinue();
        return;
    }

    char line[200];
    char email[50];
    int marks[5] = {-1, -1, -1, -1, -1};
    int studentFound = 0;

    if (file != NULL)
    {
        while (fgets(line, sizeof(line), file) != NULL)
        {
            if (sscanf(line, "%49[^,],%d,%d,%d,%d,%d",
                       email, &marks[0], &marks[1], &marks[2], &marks[3], &marks[4]) >= 1)
            {

                if (strcmp(email, studentEmail) == 0)
                {
                    // Update the specific subject marks
                    marks[choice - 1] = newMarks;
                    studentFound = 1;
                }

                // Write to temp file
                fprintf(tempFile, "%s,%d,%d,%d,%d,%d\n",
                        email, marks[0], marks[1], marks[2], marks[3], marks[4]);

                // Reset marks for next iteration
                for (int i = 0; i < 5; i++)
                    marks[i] = -1;
            }
        }
        fclose(file);
    }

    // If student not found, add new record
    if (!studentFound)
    {
        for (int i = 0; i < 5; i++)
            marks[i] = -1;
        marks[choice - 1] = newMarks;
        fprintf(tempFile, "%s,%d,%d,%d,%d,%d\n",
                studentEmail, marks[0], marks[1], marks[2], marks[3], marks[4]);
    }

    fclose(tempFile);

    // Replace original file
    remove(MARKS_FILE);
    rename("temp_marks.txt", MARKS_FILE);

    printf(GREEN "Marks updated successfully!\n");
    printf("Student: %s\n", studentEmail);
    printf("Subject: %s\n", subjects[choice - 1]);
    printf("Marks: %d\n" RESET, newMarks);
    pressKeyToContinue();
}

// Admin manage Fees function (for admin)
void manageFees(const char *adminEmail)
{
    clearScreen();
    printf(CYAN "=== Manage Student Fees ===\n" RESET);

    char studentEmail[50];
    char feeTypes[][20] = {"Tuition", "Library", "Lab", "Sports", "Exam"};
    int numFeeTypes = 5;
    int choice;
    float amount, paidAmount;

    printf(CYAN "Enter student email: " RESET);
    takeInput(studentEmail, sizeof(studentEmail));

    if (!checkEmailExists(studentEmail))
    {
        printf(RED "Student not found!\n" RESET);
        pressKeyToContinue();
        return;
    }

    // Show fee types
    printf(CYAN "\nSelect Fee Type:\n" RESET);
    for (int i = 0; i < numFeeTypes; i++)
    {
        printf("%d. %s\n", i + 1, feeTypes[i]);
    }

    printf(CYAN "Enter choice (1-%d): " RESET, numFeeTypes);
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > numFeeTypes)
    {
        printf(RED "Invalid choice!\n" RESET);
        while (getchar() != '\n')
            ;
        pressKeyToContinue();
        return;
    }
    while (getchar() != '\n')
        ;

    printf(CYAN "Enter total amount: " RESET);
    scanf("%f", &amount);
    while (getchar() != '\n')
        ;

    printf(CYAN "Enter paid amount: " RESET);
    scanf("%f", &paidAmount);
    while (getchar() != '\n')
        ;

    // Read existing data
    FILE *file = fopen(FEES_FILE, "r");
    FILE *tempFile = fopen("temp_fees.txt", "w");

    if (tempFile == NULL)
    {
        printf(RED "Error creating temporary file!\n" RESET);
        if (file)
            fclose(file);
        pressKeyToContinue();
        return;
    }

    char line[300];
    char email[50];
    float amounts[5] = {0}, paid[5] = {0};
    int studentFound = 0;

    if (file != NULL)
    {
        while (fgets(line, sizeof(line), file) != NULL)
        {
            if (sscanf(line, "%49[^,],%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
                       email, &amounts[0], &paid[0], &amounts[1], &paid[1],
                       &amounts[2], &paid[2], &amounts[3], &paid[3],
                       &amounts[4], &paid[4]) >= 1)
            {

                if (strcmp(email, studentEmail) == 0)
                {
                    // Update the specific fee type
                    amounts[choice - 1] = amount;
                    paid[choice - 1] = paidAmount;
                    studentFound = 1;
                }

                // Write to temp file
                fprintf(tempFile, "%s,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f\n",
                        email, amounts[0], paid[0], amounts[1], paid[1],
                        amounts[2], paid[2], amounts[3], paid[3],
                        amounts[4], paid[4]);

                // Reset for next iteration
                for (int i = 0; i < 5; i++)
                {
                    amounts[i] = 0;
                    paid[i] = 0;
                }
            }
        }
        fclose(file);
    }

    // If student not found, add new record
    if (!studentFound)
    {
        for (int i = 0; i < 5; i++)
        {
            amounts[i] = 0;
            paid[i] = 0;
        }
        amounts[choice - 1] = amount;
        paid[choice - 1] = paidAmount;
        fprintf(tempFile, "%s,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f\n",
                studentEmail, amounts[0], paid[0], amounts[1], paid[1],
                amounts[2], paid[2], amounts[3], paid[3],
                amounts[4], paid[4]);
    }

    fclose(tempFile);

    // Replace original file
    remove(FEES_FILE);
    rename("temp_fees.txt", FEES_FILE);

    char status[20];
    if (paidAmount >= amount)
    {
        strcpy(status, "Paid");
    }
    else
    {
        strcpy(status, "Pending");
    }

    printf(GREEN "Fee record updated successfully!\n");
    printf("Student: %s\n", studentEmail);
    printf("Fee Type: %s\n", feeTypes[choice - 1]);
    printf("Amount: %.0f, Paid: %.0f\n", amount, paidAmount);
    printf("Status: %s\n" RESET, status);
    pressKeyToContinue();
}

// Function to delete user and all associated data
int deleteUserData(const char *userEmail)
{
    printf(CYAN "=== Delete User Data ===\n" RESET);
    printf(YELLOW "WARNING: This will permanently delete user: %s\n" RESET, userEmail);
    printf(RED "Press ANY KEY within 5 seconds to CANCEL, or wait to confirm deletion...\n" RESET);

    // Simple 5-second countdown with cancel option
    for (int i = 5; i > 0; i--)
    {
        printf(YELLOW "Deleting in %d seconds... (Press any key to cancel)\n" RESET, i);

// Check if any key is pressed using kbhit() equivalent
#ifdef _WIN32
        if (_kbhit())
        {
            _getch(); // Clear the pressed key
            printf(GREEN "Deletion cancelled by user.\n" RESET);
            return 0;
        }
#endif

        Sleep(1000); // Wait a second
    }

    printf(RED "Proceeding with deletion...\n" RESET);

    // Delete from users.txt
    FILE *userFile = fopen(USER_FILE, "r");
    FILE *tempUserFile = fopen("temp_users.txt", "w");
    int userDeleted = 0;

    if (userFile != NULL && tempUserFile != NULL)
    {
        char line[MAX_LINE_LENGTH];
        User tempUser;

        while (fgets(line, sizeof(line), userFile) != NULL)
        {
            if (sscanf(line, "%49[^,],%49[^,],%19[^,],%19[^,],%49[^\n]",
                       tempUser.fullName, tempUser.email, tempUser.phone,
                       tempUser.role, tempUser.password) == 5)
            {
                if (strcmp(tempUser.email, userEmail) != 0)
                {
                    fprintf(tempUserFile, "%s", line);
                }
                else
                {
                    userDeleted = 1;
                }
            }
        }
        fclose(userFile);
        fclose(tempUserFile);

        if (userDeleted)
        {
            remove(USER_FILE);
            rename("temp_users.txt", USER_FILE);
        }
        else
        {
            remove("temp_users.txt");
        }
    }

    if (!userDeleted)
    {
        printf(RED "User not found!\n" RESET);
        return 0;
    }

    // Delete from other files (marks, fees, attendance)
    char *files[] = {MARKS_FILE, FEES_FILE, ATTENDANCE_FILE};
    char *tempFiles[] = {"temp_marks.txt", "temp_fees.txt", "temp_attendance.txt"};

    for (int fileIndex = 0; fileIndex < 3; fileIndex++)
    {
        FILE *file = fopen(files[fileIndex], "r");
        FILE *tempFile = fopen(tempFiles[fileIndex], "w");

        if (file != NULL && tempFile != NULL)
        {
            char line[MAX_LINE_LENGTH];
            char email[MAX_EMAIL_LENGTH];

            while (fgets(line, sizeof(line), file) != NULL)
            {
                if (sscanf(line, "%49[^,]", email) == 1)
                {
                    if (strcmp(email, userEmail) != 0)
                    {
                        fprintf(tempFile, "%s", line);
                    }
                }
            }
            fclose(file);
            fclose(tempFile);

            remove(files[fileIndex]);
            rename(tempFiles[fileIndex], files[fileIndex]);
        }
        else
        {
            if (file)
                fclose(file);
            if (tempFile)
                fclose(tempFile);
        }
    }

    printf(GREEN "User '%s' and all associated data deleted successfully!\n" RESET, userEmail);
    return 1;
}

// Function to show user deletion menu (for admin)
void showUserDeletionMenu()
{
    clearScreen();
    printf(RED "=== DELETE USER ===\n" RESET);

    char userEmail[MAX_EMAIL_LENGTH];
    printf(CYAN "Enter user email to delete: " RESET);
    takeInput(userEmail, sizeof(userEmail));

    // Confirm deletion (y/n, default is y)
    char confirm[10];
    printf(RED "Are you sure you want to delete user '%s'? (Y/N): " RESET, userEmail);
    takeInput(confirm, sizeof(confirm));

    // If user presses enter (empty input) or types 'y' or 'Y', proceed
    if (strlen(confirm) == 0 || strcmp(confirm, "y") == 0 || strcmp(confirm, "Y") == 0)
    {
        // continue --> deletion
    }
    else if (strcmp(confirm, "n") == 0 || strcmp(confirm, "N") == 0)
    {
        printf(GREEN "User deletion cancelled.\n" RESET);
        pressKeyToContinue();
        return;
    }
    else
    {
        printf(RED "Invalid input. User deletion cancelled.\n" RESET);
        pressKeyToContinue();
        return;
    }

    if (strlen(userEmail) == 0)
    {
        printf(RED "Error: Email cannot be empty.\n" RESET);
        pressKeyToContinue();
        return;
    }

    // Check if trying to delete system admin
    if (strcmp(userEmail, "admin@system.com") == 0)
    {
        printf(RED "Error: Cannot delete system admin account!\n" RESET);
        pressKeyToContinue();
        return;
    }

    if (deleteUserData(userEmail))
    {
        printf(GREEN "User deletion completed!\n" RESET);
    }

    pressKeyToContinue();
}

// show student's attendence with (for studnet)
void viewMyAttendance(const char *studentEmail)
{
    clearScreen();
    // Hardcoded subjects list
    char subjects[][20] = {"English", "Math", "Science", "History", "Computer"};
    int numSubjects = 5;
    printf(CYAN "=== My Attendance Record ===\n" RESET);
    printf("Student: %s\n\n", studentEmail);

    FILE *file = fopen(ATTENDANCE_FILE, "r");
    if (file == NULL)
    {
        printf(RED "No attendance records found!\n" RESET);
        printf(YELLOW "Contact your teacher to mark attendance.\n" RESET);
        pressKeyToContinue();
        return;
    }

    char line[MAX_LINE_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    int present[5] = {0};                     // Present days for each subject
    int total[5] = {120, 120, 120, 120, 120}; // Total days for each subject (default 120)
    int found = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        // Parse: email,english_present,english_total,math_present,math_total,...
        if (sscanf(line, "%49[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                   email, &present[0], &total[0], &present[1], &total[1],
                   &present[2], &total[2], &present[3], &total[3],
                   &present[4], &total[4]) >= 1)
        {
            if (strcmp(email, studentEmail) == 0)
            {
                found = 1;
                break;
            }
        }
    }
    fclose(file);

    if (!found)
    {
        printf(YELLOW "No attendance record found for your account!\n" RESET);
        pressKeyToContinue();
        return;
    }

    // Display attendance table
    printf(BLUE "Subject          Present/Total    Percentage    Status\n");
    printf("-------------------------------------------------------\n" RESET);

    int totalPresent = 0, totalDays = 0;

    for (int i = 0; i < numSubjects; i++)
    {
        float percentage = (total[i] > 0) ? (float)present[i] * 100 / total[i] : 0;
        char status[20];

        if (percentage >= 75)
        {
            strcpy(status, "Good");
            printf(GREEN "%-15s  %3d/%-3d        %6.1f%%      %s\n" RESET,
                   subjects[i], present[i], total[i], percentage, status);
        }
        else if (percentage >= 65)
        {
            strcpy(status, "Warning");
            printf(YELLOW "%-15s  %3d/%-3d        %6.1f%%      %s\n" RESET,
                   subjects[i], present[i], total[i], percentage, status);
        }
        else
        {
            strcpy(status, "Critical");
            printf(RED "%-15s  %3d/%-3d        %6.1f%%      %s\n" RESET,
                   subjects[i], present[i], total[i], percentage, status);
        }

        totalPresent += present[i];
        totalDays += total[i];
    }

    // Overall attendance
    if (totalDays > 0)
    {
        float overallPercentage = (float)totalPresent * 100 / totalDays;
        printf("-------------------------------------------------------\n");
        if (overallPercentage >= 75)
        {
            printf(GREEN "Overall:         %3d/%-3d        %6.1f%%      Good\n" RESET,
                   totalPresent, totalDays, overallPercentage);
        }
        else if (overallPercentage >= 65)
        {
            printf(YELLOW "Overall:         %3d/%-3d        %6.1f%%      Warning\n" RESET,
                   totalPresent, totalDays, overallPercentage);
        }
        else
        {
            printf(RED "Overall:         %3d/%-3d        %6.1f%%      Critical\n" RESET,
                   totalPresent, totalDays, overallPercentage);
        }

        // Attendance advice
        printf("\n");
        if (overallPercentage >= 75)
        {
            printf(GREEN "%c Excellent attendance! Keep it up!\n" RESET, 251);
        }
        else if (overallPercentage >= 65)
        {
            printf(YELLOW "%c Warning: Attendance below 75%%. Please improve.\n" RESET, 127);
        }
        else
        {
            printf(RED "%c Critical: Attendance below 65%%. Risk of academic issues!\n" RESET, 127);
        }
    }

    pressKeyToContinue();
}

// Function for teachers to mark attendance
void markStudentAttendance(const char *teacherEmail)
{
    clearScreen();
    // Hardcoded subjects list
    char subjects[][20] = {"English", "Math", "Science", "History", "Computer"};
    int numSubjects = 5;
    printf(CYAN "=== Mark Student Attendance ===\n" RESET);

    char studentEmail[MAX_EMAIL_LENGTH];
    int subjectChoice, attendanceChoice;

    printf(CYAN "Enter student email: " RESET);
    takeInput(studentEmail, sizeof(studentEmail));

    // Check if student exists
    if (!checkEmailExists(studentEmail))
    {
        printf(RED "Student not found!\n" RESET);
        pressKeyToContinue();
        return;
    }

    // Show subjects
    printf(CYAN "\nSelect Subject:\n" RESET);
    for (int i = 0; i < numSubjects; i++)
    {
        printf("%d. %s\n", i + 1, subjects[i]);
    }

    printf(CYAN "Enter subject choice (1-%d): " RESET, numSubjects);
    if (scanf("%d", &subjectChoice) != 1 || subjectChoice < 1 || subjectChoice > numSubjects)
    {
        printf(RED "Invalid choice!\n" RESET);
        while (getchar() != '\n')
            ;
        pressKeyToContinue();
        return;
    }
    while (getchar() != '\n')
        ;

    // Attendance options
    printf(CYAN "\nAttendance Action:\n" RESET);
    printf("1. Update Present Days\n");
    printf("2. Update Total Days\n");
    printf(CYAN "Enter choice (1-2): " RESET);

    if (scanf("%d", &attendanceChoice) != 1 || attendanceChoice < 1 || attendanceChoice > 2)
    {
        printf(RED "Invalid choice!\n" RESET);
        while (getchar() != '\n')
            ;
        pressKeyToContinue();
        return;
    }
    while (getchar() != '\n')
        ;

    // Read existing attendance data
    FILE *file = fopen(ATTENDANCE_FILE, "r");
    FILE *tempFile = fopen("temp_attendance.txt", "w");

    if (tempFile == NULL)
    {
        printf(RED "Error creating temporary file!\n" RESET);
        if (file)
            fclose(file);
        pressKeyToContinue();
        return;
    }

    char line[MAX_LINE_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    int present[5] = {0}, total[5] = {120, 120, 120, 120, 120}; // Default 120 total days
    int studentFound = 0;

    if (file != NULL)
    {
        while (fgets(line, sizeof(line), file) != NULL)
        {
            if (sscanf(line, "%49[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                       email, &present[0], &total[0], &present[1], &total[1],
                       &present[2], &total[2], &present[3], &total[3],
                       &present[4], &total[4]) >= 1)
            {
                if (strcmp(email, studentEmail) == 0)
                {
                    studentFound = 1;

                    // Update based on choice
                    switch (attendanceChoice)
                    {
                    case 1: // Update Present Days
                    {
                        int newPresentDays;
                        printf(CYAN "Current present days for %s in %s: %d/%d\n" RESET,
                               studentEmail, subjects[subjectChoice - 1], present[subjectChoice - 1], total[subjectChoice - 1]);
                        printf(CYAN "Enter number of present days (0-%d): " RESET, total[subjectChoice - 1]);

                        int inputResult = scanf("%d", &newPresentDays);
                        while (getchar() != '\n')
                            ; // Clear input buffer

                        if (inputResult == 1 && newPresentDays >= 0 && newPresentDays <= total[subjectChoice - 1])
                        {
                            present[subjectChoice - 1] = newPresentDays;
                            printf(GREEN "Present days updated for %s in %s (Present: %d/%d)\n" RESET,
                                   studentEmail, subjects[subjectChoice - 1],
                                   present[subjectChoice - 1], total[subjectChoice - 1]);
                        }
                        else
                        {
                            printf(RED "Invalid number of present days! Must be between 0 and %d.\n" RESET, total[subjectChoice - 1]);
                        }
                        break;
                    }

                    case 2: // Update Total Days
                    {
                        int newTotal;
                        printf(CYAN "Current total days for %s: %d\n" RESET,
                               subjects[subjectChoice - 1], total[subjectChoice - 1]);
                        printf(CYAN "Enter new total days (1-365): " RESET);

                        // Better input validation
                        int inputResult = scanf("%d", &newTotal);
                        while (getchar() != '\n')
                            ; // Clear input buffer

                        if (inputResult == 1 && newTotal > 0 && newTotal <= 365)
                        {
                            total[subjectChoice - 1] = newTotal;
                            // Ensure present days don't exceed new total
                            if (present[subjectChoice - 1] > newTotal)
                                present[subjectChoice - 1] = newTotal;
                            printf(GREEN "Total days updated for %s in %s (Present: %d/%d)\n" RESET,
                                   studentEmail, subjects[subjectChoice - 1],
                                   present[subjectChoice - 1], total[subjectChoice - 1]);
                        }
                        else
                        {
                            printf(RED "Invalid total days! Please enter a number between 1 and 365.\n" RESET);
                        }
                        break;
                    }
                    default:
                        break;
                    }
                }
                // Write updated or unchanged record
                fprintf(tempFile, "%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                        email, present[0], total[0], present[1], total[1],
                        present[2], total[2], present[3], total[3],
                        present[4], total[4]);
            }
            else
            {
                // Write lines that don't match expected format as-is
                fprintf(tempFile, "%s", line);
            }
        }
        fclose(file);
    }

    // If student not found, add new record
    if (!studentFound)
    {
        // Initialize with default values
        for (int i = 0; i < 5; i++)
        {
            present[i] = 0;
            total[i] = 120;
        }

        // Handle the selected action for new student
        switch (attendanceChoice)
        {
        case 1: // Update Present Days
        {
            int newPresentDays;
            printf(CYAN "Enter number of present days for %s in %s (0-%d): " RESET,
                   studentEmail, subjects[subjectChoice - 1], total[subjectChoice - 1]);

            int inputResult = scanf("%d", &newPresentDays);
            while (getchar() != '\n')
                ; // Clear input buffer

            if (inputResult == 1 && newPresentDays >= 0 && newPresentDays <= total[subjectChoice - 1])
            {
                present[subjectChoice - 1] = newPresentDays;
                printf(GREEN "New student record created. Present days set for %s in %s (Present: %d/%d)\n" RESET,
                       studentEmail, subjects[subjectChoice - 1], newPresentDays, total[subjectChoice - 1]);
            }
            else
            {
                printf(RED "Invalid number of present days! Using default value of 0.\n" RESET);
                printf(GREEN "New student record created with default values (Present: 0/%d)\n" RESET, total[subjectChoice - 1]);
            }
            break;
        }

        case 2: // Update Total Days
        {
            int newTotal;
            printf(CYAN "Enter total days for %s (new student): " RESET, subjects[subjectChoice - 1]);

            int inputResult = scanf("%d", &newTotal);
            while (getchar() != '\n')
                ; // Clear input buffer

            if (inputResult == 1 && newTotal > 0 && newTotal <= 365)
            {
                total[subjectChoice - 1] = newTotal;
                printf(GREEN "New student record created. Total days set for %s in %s (Present: 0/%d)\n" RESET,
                       studentEmail, subjects[subjectChoice - 1], newTotal);
            }
            else
            {
                printf(RED "Invalid total days! Using default value of 120.\n" RESET);
                printf(GREEN "New student record created with default values (Present: 0/120)\n" RESET);
            }
            break;
        }
        default:
            break;
        }

        fprintf(tempFile, "%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                studentEmail, present[0], total[0], present[1], total[1],
                present[2], total[2], present[3], total[3],
                present[4], total[4]);
    }

    fclose(tempFile);

    // Replace original file
    remove(ATTENDANCE_FILE);
    rename("temp_attendance.txt", ATTENDANCE_FILE);

    printf(GREEN "Attendance record updated successfully!\n" RESET);
    pressKeyToContinue();
}

void generateUserReport(const User *user)
{
    clearScreen();
    printf(CYAN "=== Generate User Report ===\n" RESET);

    char targetEmail[MAX_EMAIL_LENGTH];
    printf(CYAN "Enter user email to generate report: " RESET);
    takeInput(targetEmail, sizeof(targetEmail));

    if (strlen(targetEmail) == 0)
    {
        printf(RED "Error: Email cannot be empty.\n" RESET);
        pressKeyToContinue();
        return;
    }

    // Check if the target user exists
    if (!checkEmailExists(targetEmail))
    {
        printf(RED "Error: User with email '%s' not found.\n" RESET, targetEmail);
        pressKeyToContinue();
        return;
    }

    // Find the target user's role
    FILE *file = fopen(USER_FILE, "r");
    if (file == NULL)
    {
        printf(RED "Error: Unable to open user file.\n" RESET);
        pressKeyToContinue();
        return;
    }

    char line[MAX_LINE_LENGTH];
    User targetUser;
    int found = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "%49[^,],%49[^,],%19[^,],%19[^,],%49[^\n]",
                   targetUser.fullName, targetUser.email, targetUser.phone,
                   targetUser.role, targetUser.password) == 5)
        {
            if (strcmp(targetUser.email, targetEmail) == 0)
            {
                found = 1;
                break;
            }
        }
    }
    fclose(file);

    if (!found)
    {
        printf(RED "Error: Unable to retrieve user information.\n" RESET);
        pressKeyToContinue();
        return;
    }

    // Role-based access control
    if (strcmp(user->role, "Teacher") == 0)
    {
        // Teachers can only generate reports for students
        if (strcmp(targetUser.role, "Student") != 0)
        {
            printf(RED "Error: Teachers can only generate reports for students.\n" RESET);
            printf(RED "Cannot generate report for %s role.\n" RESET, targetUser.role);
            pressKeyToContinue();
            return;
        }

        // Generate student report for teacher
        clearScreen();
        printf(BLUE "=====================================\n");
        printf("       STUDENT REPORT (Teacher View) \n");
        printf("=====================================\n" RESET);
        generateStudentReport(&targetUser);
    }
    else if (strcmp(user->role, "Admin") == 0)
    {
        // Admins can generate reports for anyone
        clearScreen();
        // Color header based on role
        if (strcmp(targetUser.role, "Student") == 0)
        {
            printf(BLUE "=====================================\n");
            printf("       STUDENT REPORT (Admin View)   \n");
            printf("=====================================\n" RESET);
            generateStudentReport(&targetUser);
        }
        else if (strcmp(targetUser.role, "Teacher") == 0)
        {
            printf(MAGENTA "=====================================\n");
            printf("       TEACHER REPORT (Admin View)   \n");
            printf("=====================================\n" RESET);
            generateTeacherReport(&targetUser);
        }
        else if (strcmp(targetUser.role, "Admin") == 0)
        {
            printf(RED "=====================================\n");
            printf("        ADMIN REPORT (Admin View)    \n");
            printf("=====================================\n" RESET);
            generateAdminReport(&targetUser);
        }
        else
        {
            printf(RED "Error: Unknown user role '%s'\n" RESET, targetUser.role);
        }
    }
    else
    {
        printf(RED "Error: Only Teachers and Admins can generate user reports.\n" RESET);
    }

    printf(CYAN "\n=====================================\n");
    printf("         END OF REPORT               \n");
    printf("=====================================\n" RESET);

    pressKeyToContinue();
}

// Updated generateStudentReport function
void generateStudentReport(const User *user)
{
    displayUserInfo(user);
    printf(GREEN "\n=== ACADEMIC PERFORMANCE ===\n" RESET);
    viewMyMarks(user->email);
    viewMyAttendance(user->email);
    viewMyFeeStatus(user->email);
}

// Updated generateTeacherReport function
void generateTeacherReport(const User *user)
{
    displayUserInfo(user);
    printf(MAGENTA "\n--- TEACHING SUMMARY ---\n" RESET);
    printf("Current Semester: Active\n");
    printf("Teaching Load:    Full Time\n");
    printf("Office Hours:     Mon-Fri 10:00-12:00\n");
}

// Updated generateAdminReport function
void generateAdminReport(const User *user)
{
    displayUserInfo(user);
    printf(RED "\n--- ADMIN SUMMARY ---\n" RESET);
    printf("Admin Level: Full Access\n");
    printf("Department: System Administration\n");
    printf("Last Login: Active Session\n");
}

// Function to get student marks using your existing file format
int getStudentMarksForMarksheet(const char *email, int marks[5])
{
    FILE *file = fopen(MARKS_FILE, "r");
    if (file == NULL)
    {
        for (int i = 0; i < 5; i++)
            marks[i] = -1;
        return 0;
    }

    char line[200];
    char fileEmail[50];

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "%49[^,],%d,%d,%d,%d,%d",
                   fileEmail, &marks[0], &marks[1], &marks[2], &marks[3], &marks[4]) >= 1)
        {
            if (strcmp(fileEmail, email) == 0)
            {
                fclose(file);
                return 1;
            }
        }
    }
    fclose(file);

    for (int i = 0; i < 5; i++)
        marks[i] = -1;
    return 0;
}

// Function to get student attendance using your existing file format
int getStudentAttendanceForMarksheet(const char *email, int present[5], int total[5])
{
    FILE *file = fopen(ATTENDANCE_FILE, "r");
    if (file == NULL)
    {
        for (int i = 0; i < 5; i++)
        {
            present[i] = 0;
            total[i] = 120;
        }
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    char fileEmail[MAX_EMAIL_LENGTH];

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "%49[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                   fileEmail, &present[0], &total[0], &present[1], &total[1],
                   &present[2], &total[2], &present[3], &total[3],
                   &present[4], &total[4]) >= 1)
        {
            if (strcmp(fileEmail, email) == 0)
            {
                fclose(file);
                return 1;
            }
        }
    }
    fclose(file);

    for (int i = 0; i < 5; i++)
    {
        present[i] = 0;
        total[i] = 120;
    }
    return 0;
}

// Function to get student fees using your existing file format
int getStudentFeesForMarksheet(const char *email, float amounts[5], float paid[5])
{
    FILE *file = fopen(FEES_FILE, "r");
    if (file == NULL)
    {
        for (int i = 0; i < 5; i++)
        {
            amounts[i] = 0;
            paid[i] = 0;
        }
        return 0;
    }

    char line[300];
    char fileEmail[50];

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "%49[^,],%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
                   fileEmail, &amounts[0], &paid[0], &amounts[1], &paid[1],
                   &amounts[2], &paid[2], &amounts[3], &paid[3],
                   &amounts[4], &paid[4]) >= 1)
        {
            if (strcmp(fileEmail, email) == 0)
            {
                fclose(file);
                return 1;
            }
        }
    }
    fclose(file);

    for (int i = 0; i < 5; i++)
    {
        amounts[i] = 0;
        paid[i] = 0;
    }
    return 0;
}

// Function to find user by email using your existing file format
User *findStudentByEmail(const char *email)
{
    static User foundUser;
    FILE *file = fopen(USER_FILE, "r");
    if (file == NULL)
    {
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "%49[^,],%49[^,],%19[^,],%19[^,],%49[^\n]",
                   foundUser.fullName, foundUser.email, foundUser.phone,
                   foundUser.role, foundUser.password) == 5)
        {
            if (strcmp(foundUser.email, email) == 0)
            {
                fclose(file);
                return &foundUser;
            }
        }
    }
    fclose(file);
    return NULL;
}

// Main Marksheet Generator Function
void generateMarksheet(const User *currentUser)
{
    char targetEmail[MAX_EMAIL_LENGTH];

    // Determine whose marksheet to generate based on current user's role
    if (strcmp(currentUser->role, "Student") == 0)
    {
        // If student, generate their own marksheet
        strcpy(targetEmail, currentUser->email);
    }
    else if (strcmp(currentUser->role, "Teacher") == 0 || strcmp(currentUser->role, "Admin") == 0)
    {
        // If teacher or admin, ask for student email
        clearScreen();
        printf(CYAN "=== MARKSHEET GENERATOR ===\n" RESET);
        printf(CYAN "Enter student's email: " RESET);
        takeInput(targetEmail, MAX_EMAIL_LENGTH);

        if (strlen(targetEmail) == 0)
        {
            printf(RED "Error: Email cannot be empty.\n" RESET);
            pressKeyToContinue();
            return;
        }
    }
    else
    {
        printf(RED "Access denied!\n" RESET);
        pressKeyToContinue();
        return;
    }

    // Check if student exists
    if (!checkEmailExists(targetEmail))
    {
        printf(RED "Student not found!\n" RESET);
        pressKeyToContinue();
        return;
    }

    // Find student data
    User *student = findStudentByEmail(targetEmail);
    if (student == NULL || strcmp(student->role, "Student") != 0)
    {
        printf(RED "Invalid student account!\n" RESET);
        pressKeyToContinue();
        return;
    }

    // Get academic data
    int marks[5];
    int present[5], total[5];
    float amounts[5], paid[5];

    getStudentMarksForMarksheet(targetEmail, marks);
    getStudentAttendanceForMarksheet(targetEmail, present, total);
    getStudentFeesForMarksheet(targetEmail, amounts, paid);

    char subjects[][20] = {"English", "Math", "Science", "History", "Computer"};

    // Calculate totals
    float totalMarks = 0, validSubjects = 0;
    int totalPresent = 0, totalDays = 0;
    float totalFeeAmount = 0, totalFeePaid = 0;

    for (int i = 0; i < 5; i++)
    {
        if (marks[i] != -1)
        {
            totalMarks += marks[i];
            validSubjects++;
        }
        totalPresent += present[i];
        totalDays += total[i];
        totalFeeAmount += amounts[i];
        totalFeePaid += paid[i];
    }

    float percentage = (validSubjects > 0) ? (totalMarks / (validSubjects * 100)) * 100 : 0;
    float attendancePercentage = (totalDays > 0) ? ((float)totalPresent / totalDays) * 100 : 0;
    float feeDue = totalFeeAmount - totalFeePaid;

    // Determine result
    char *result = "FAIL";
    if (percentage >= 40 && attendancePercentage >= 75)
    {
        result = "PASS";
    }

    // Get current date
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // Generate simple marksheet
    clearScreen();
    printf("\n");
    printf("================================================================================\n");
    printf("                               POKHARA UNIVERSITY                               \n");
    printf("                      DEPARTMENT OF B.COMPUTER APPLICATION                      \n");
    printf("                              STUDENT MARKSHEET                                 \n");
    printf("================================================================================\n");
    printf(" Date: %02d/%02d/%d                          Academic Session: 2024-25         \n",
           tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    printf("================================================================================\n" RESET);

    printf("\n" BLUE "STUDENT INFORMATION:\n");
    printf("--------------------------------------------------------------------------------\n");
    printf(" Name           : %s\n", student->fullName);
    printf(" Email          : %s\n", student->email);
    printf(" Phone          : %s\n", student->phone);
    printf("--------------------------------------------------------------------------------\n" RESET);

    printf("\n" MAGENTA "ACADEMIC PERFORMANCE:\n");
    printf("--------------------------------------------------------------------------------\n");
    if (validSubjects > 0)
    {
        printf(" Total Marks    : %.0f out of %.0f\n", totalMarks, validSubjects * 100);
        printf(" Percentage     : %.1f%%\n", percentage);

        // Grade calculation
        char overallGrade = 'F';
        if (percentage >= 90)
            overallGrade = 'A';
        else if (percentage >= 80)
            overallGrade = 'B';
        else if (percentage >= 70)
            overallGrade = 'C';
        else if (percentage >= 60)
            overallGrade = 'D';

        printf(" Overall Grade  : %c\n", overallGrade);
    }
    else
    {
        printf(" Total Marks    : No marks available\n");
        printf(" Percentage     : N/A\n");
        printf(" Overall Grade  : Not Assigned\n");
    }
    printf("--------------------------------------------------------------------------------\n" RESET);

    printf("\n" YELLOW "ATTENDANCE RECORD:\n");
    printf("--------------------------------------------------------------------------------\n");
    printf(" Present Days   : %d out of %d\n", totalPresent, totalDays);
    printf(" Attendance     : %.1f%%\n", attendancePercentage);

    char *attStatus = "Critical";
    if (attendancePercentage >= 85)
        attStatus = "Excellent";
    else if (attendancePercentage >= 75)
        attStatus = "Good";
    else if (attendancePercentage >= 65)
        attStatus = "Warning";

    printf(" Status         : %s\n", attStatus);
    printf("--------------------------------------------------------------------------------\n" RESET);

    printf("\n" CYAN "FEE DETAILS:\n");
    printf("--------------------------------------------------------------------------------\n");
    if (totalFeeAmount > 0)
    {
        printf(" Total Amount   : Rs. %.0f\n", totalFeeAmount);
        printf(" Amount Paid    : Rs. %.0f\n", totalFeePaid);
        printf(" Balance Due    : Rs. %.0f\n", feeDue);
        printf(" Payment Status : %s\n", (feeDue <= 0) ? "Fully Paid" : "Outstanding");
    }
    else
    {
        printf(" No fee records available\n");
    }
    printf("--------------------------------------------------------------------------------\n" RESET);

    // Result section
    printf("\n");
    printf("================================================================================\n");
    if (strcmp(result, "PASS") == 0)
    {
        printf(GREEN "                                RESULT: PASS                                   \n" RESET);
        printf("================================================================================\n");
        printf(" Congratulations! You have successfully met all academic requirements.\n");
    }
    else
    {
        printf(RED "                                RESULT: FAIL                                   \n" RESET);
        printf("================================================================================\n");
        printf(" Academic requirements not satisfied. Please consult your advisor.\n");
    }
    printf("================================================================================\n");

    // Remarks section
    printf("\n" YELLOW "REMARKS:\n");
    printf("--------------------------------------------------------------------------------\n");
    if (validSubjects == 0)
    {
        printf(" * No academic records found. Please contact your instructor.\n");
    }
    else if (percentage >= 85)
    {
        printf(" * Excellent academic performance. Keep up the outstanding work!\n");
    }
    else if (percentage >= 70)
    {
        printf(" * Good performance. Continue working towards excellence.\n");
    }
    else if (percentage >= 40)
    {
        printf(" * Satisfactory performance with room for improvement.\n");
    }
    else
    {
        printf(" * Requires significant academic improvement.\n");
    }

    if (attendancePercentage < 75)
    {
        printf(" * Warning: Attendance below minimum requirement (75%%).\n");
    }

    if (feeDue > 0)
    {
        printf(" * Notice: Outstanding fee payment required.\n");
    }
    printf("--------------------------------------------------------------------------------\n" RESET);

    printf("\n" BLUE "================================================================================\n");
    printf(" This report is computer generated and does not require signature.\n");
    printf(" Generated by: Student Management System | Team: Sapphire\n");
    printf(" Report Date: %02d/%02d/%d\n",
           tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    printf("================================================================================\n" RESET);

    pressKeyToContinue();
}