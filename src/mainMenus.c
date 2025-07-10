// Standard Libraries
#include <stdio.h>

// Custom Libraries
#include <mainMenus.h>

// Custom Colors
#define RESET "\033[0m"
#define CYAN "\033[36m"

void showMainMenu()
{
   printf(CYAN "=== User Management System ===\n" RESET);
    printf("1. Sign Up\n");
    printf("2. Login\n");
    printf("3. Exit\n");
}