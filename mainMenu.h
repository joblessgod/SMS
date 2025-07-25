// Standard Libraries
#include <stdio.h>

// Custom Colors
#define RESET "\033[0m"
#define CYAN "\033[36m"

void showMainMenu()
{
    printf(CYAN "=== Student Management System ===\n" RESET);
    printf("1. Sign-up\n");
    printf("2. Login\n");
    printf("3. Exit\n");
    printf(CYAN "Enter your choice: " RESET);
}
