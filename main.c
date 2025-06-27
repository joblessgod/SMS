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
typedef enum
{
    ROLE_STUDENT,
    ROLE_FACULTY,
    ROLE_ADMIN
} UserRole;

struct User
{
    char fullName[50];
    char email[50];
    char password[50];
    char phone[50];
    UserRole role;
} user;

void takeInput(char *);
void takePassword(char *);

void showMainMenu()
{
    printf(CYAN "\t\t\t Welcome to Student Management System\t\t\t" RESET);
    printf(CYAN "\nPlease choose your option:" RESET);
    printf(CYAN "\n1) Signup" RESET);
    printf(CYAN "\n2) Login" RESET);
    printf(CYAN "\n3) Exit\n" RESET);
}
void selectUserRole(UserRole *role)
{
    int rchoice;
    printf(CYAN "\nSelect your role:\n" RESET);
    printf(CYAN "1) Student\n2) Faculty\n3) Admin\n" RESET);
    printf(CYAN "Enter your role (1-3):\t" RESET);
    scanf("%d", &rchoice);
    while ((getchar()) != '\n')
        ;
    switch (rchoice)
    {
    case 1:
        *role = ROLE_STUDENT;
        break;
    case 2:
        *role = ROLE_FACULTY;
        break;
    case 3:
        *role = ROLE_ADMIN;
        break;
    default:
        printf(RED "Invalid role. Defaulting to Student.\n" RESET);
        *role = ROLE_STUDENT;
    }
}
void showRoleMenu(UserRole role);

void handleRoleMenu(UserRole role)
{
    int choice;
    showRoleMenu(role);
    printf(CYAN "Enter your choice:\t" RESET);
    scanf("%d", &choice);
    while ((getchar()) != '\n')
        ;

    switch (role)
    {
    case ROLE_ADMIN:
        system("cls");
        switch (choice)
        {
        case 1:
            printf("// I need more code...\n");
            break;
        case 2:
            printf("// I need more code...\n");
            break;
        case 3:
            printf(GREEN "Logging out...\n" RESET);
            return;
        default:
            printf(RED "Invalid choice.\n" RESET);
        }
        handleRoleMenu(user.role);
        break;
    case ROLE_FACULTY:
        system("cls");
        switch (choice)
        {
        case 1:
            printf("// I need more code...\n");
            handleRoleMenu(user.role);
            break;
        case 2:
            printf("// I need more code...\n");
            handleRoleMenu(user.role);
            break;
        case 3:
            printf(GREEN "Logging out...\n" RESET);
            break;
        default:
            printf(RED "Invalid choice.\n" RESET);
        }
        break;
        handleRoleMenu(user.role);
    case ROLE_STUDENT:
        system("cls");
        switch (choice)
        {
        case 1:
            printf("// I need more code...\n");
            handleRoleMenu(user.role);
            break;
        case 2:
            printf("// I need more code...\n");
            handleRoleMenu(user.role);
            break;
        case 3:
            printf(GREEN "Logging out...\n" RESET);
            break;
        default:
            printf(RED "Invalid choice. Try again later.\n" RESET);
        }
        break;
        handleRoleMenu(user.role);
    default:
        printf(RED "Unknown role.\n" RESET);
    }
}
const char *getRoleString(UserRole role)
{
    switch (role)
    {
    case ROLE_STUDENT:
        return "Student";
    case ROLE_FACULTY:
        return "Faculty";
    case ROLE_ADMIN:
        return "Admin";
    default:
        return "Unknown";
    }
}
int main()
{
    FILE *fp;
    SetConsoleTitle("Student Management System");
    int choice;
    char conformPassword[50];

    showMainMenu();

    printf(CYAN "Enter your choice:\t" RESET);
    scanf("%d", &choice);
    while ((getchar()) != '\n');

    switch (choice)
    {
    case 1: // signup
        system("cls");
        printf(CYAN "Enter your fullName:\t" RESET);
        takeInput(user.fullName);

        printf(CYAN "Enter your email:\t" RESET);
        takeInput(user.email);

        printf(CYAN "Enter your phone:\t" RESET);
        takeInput(user.phone);

        selectUserRole(&user.role);

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
            fp = fopen("user.txt", "a+");
            if (fp == NULL)
            {
                printf(RED "Error opening file" RESET);
                return 1;
            }
            size_t written = fwrite(&user, sizeof(struct User), 1, fp);
            // fputc('\n', fp);
            if (written != 0)
                printf(GREEN "\n%s, Your account has been created successfully as %s!\n" RESET, user.fullName, getRoleString(user.role));
            else
                printf(RED "\nSorry! Something went wrong :(" RESET);
            fclose(fp);
        }
        Sleep(1000);
        system("cls");
        main();
        break;
    case 2: // login
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
                printf("Role:\t %s\n", getRoleString(user.role));
                
                if (user.role == ROLE_ADMIN)
                {
                    printf(GREEN "\nYou have admin privileges.\n" RESET);
                }
                else if (user.role == ROLE_FACULTY)
                {
                    printf(GREEN "\nYou have faculty privileges.\n" RESET);
                }
                else
                {
                    printf(GREEN "\nYou have student privileges.\n" RESET);
                }
            }
            else
            {
                printf(RED "\nInvalid email or password.\n" RESET);
                Sleep(1000);
                system("cls");
                main();
            }
        }

        handleRoleMenu(user.role);
        break;
    case 3:
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

void showRoleMenu(UserRole role)
{
    printf(CYAN "\n--- %s Menu ---\n" RESET, getRoleString(role));
    switch (role)
    {
    case ROLE_ADMIN:
        printf("1) View all users\n");
        printf("2) Manage system settings\n");
        printf("3) Logout\n");
        break;
    case ROLE_FACULTY:
        printf("1) View student list\n");
        printf("2) Enter grades\n");
        printf("3) Logout\n");
        break;
    case ROLE_STUDENT:
        printf("1) View profile\n");
        printf("2) View grades\n");
        printf("3) Logout\n");
        break;
    default:
        printf("1) Logout\n");
        break;
    }
}