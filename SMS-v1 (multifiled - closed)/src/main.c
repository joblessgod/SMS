#include <stdio.h>
#include <windows.h>
#include <mainmenu.h>
#include <helper.h>

struct user
{
     char fullName[50];
     char email[50];
     char password[50];
     char phone[50];
} user;

int main()
{
     int choice;
     showMainMenu();

     printf("Enter your choice:\t");
     scanf("%d", &choice);
     while ((getchar()) != '\n');
     switch (choice)
     {
     case 1:
          system("cls");
          printf("\n\nEnter your fullName:\t");
          takeInput(user.fullName);
          
          printf("\nEnter your email:\t");
          takeInput(user.email);

          printf("\nEnter password:\t");
          takePassword(user.password);

          printf("\nEnter your phone:\t");
          takeInput(user.phone);

          printf("\nFull Name:\t %s", user.fullName);
          printf("\nEmail:\t %s", user.email);
          printf("\nPassword:\t %s", user.password);
          printf("\nPhone:\t %s", user.phone);
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