// Standard Libraries
#include <stdio.h>

// Custom Colors
#define RESET "\033[0m"
#define CYAN "\033[36m"

void showMainMenu()
{
    printf(CYAN "================================\n" RESET);
    printf(CYAN "  Student Management System\n" RESET);
    printf(CYAN "================================\n" RESET);
    printf("1. Sign Up\n");
    printf("2. Login\n");
    printf("3. Exit\n");
    printf(CYAN "--------------------------------\n" RESET);
    printf(CYAN "Enter your choice (1-3): " RESET);
}
