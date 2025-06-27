#include <stdio.h>
#include <windows.h>
#include <mainmenu.h>

int main() {
     int choice;
     showMainMenu();

     printf("Enter your choice:\t");
     scanf("%d", &choice);

     switch (choice) {
     case 1:
          printf("Sign up \n");
          break;
     case 2:
          printf("Login up \n");
          break;
     case 3:
          printf("\t\t\tBye Bye :)");
          system("exit");
          break;
     }
     return 0;
}