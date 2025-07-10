// Standard Libraries
#include <stdio.h>

// Custom Libraries
#include <mainMenus.h>

// Custom Colors
#define RESET "\033[0m"
#define CYAN "\033[36m"

void showMainMenu()
{
    printf(CYAN "\t\t\t\t Welcome to Student Management System\t\t\t" RESET);
    printf(CYAN "\n\t\tPlease choose your option:" RESET);
    printf(CYAN "\n\t\t1) Signup" RESET);
    printf(CYAN "\n\t\t2) Login" RESET);
    printf(CYAN "\n\t\t3) Exit" RESET);
}