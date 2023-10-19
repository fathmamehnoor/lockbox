#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <argon2.h>
#include <inttypes.h>
#include <sodium.h>
#include "user.h"

int main(){

    int choice;
    int loggedIn = 0;
    if (!load_users_from_file()) {
        printf("Unable to load user data.\n");
        return 1;
    }

    do{
    printf("1. Login\n2. Add Account\n3. Exit\n");
    printf("Enter a choice\n");
    scanf("%d",&choice);
    switch(choice){
        case 1:
            if (loggedIn == 0) {
                    loggedIn = authenticate_user();
                } else {
                    printf("You are already logged in.\n");
                }
            break;
        case 2:
            loggedIn=register_user();
            if (!save_users_to_file()) {
                    printf("Unable to save user data.\n");
                }
            break;
        case 3:
            exit(0);
        default:
            printf("Enter a valid choice!\n");

     }
    }while(choice!=3);
}