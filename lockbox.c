#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <argon2.h>
#include "user.h"



/* Function that adds password entries */
void add_password(){

    struct p_entry* new = (struct p_entry*)malloc(sizeof(struct p_entry));
    if(new == NULL){
        perror("Memory allocation error!\n");
        exit(0);
    }
    printf("Account:\n");
    scanf("%99s",new->account);
    printf("Password:\n");
    scanf("%99s",new->password);

    new->next = head;
    head = new;
}

/* Function to retrieve the data sored in linked list */
void retrieve_password(struct p_entry** head, const char* website){

    struct p_entry *current = *head;
    while(current != NULL){
        if(strcmp(current->account, website)==0){
            printf("%s : %s \n",current->account, current->password);
            return;
        }
        current = current->next;
    }

    printf("Error: Password not been added!\n");
}

/* Function to update password */
void update_password(struct p_entry** head, const char* website){

    char new_password[100];
    struct p_entry *current = *head;
    while(current != NULL){
        if(strcmp(current->account, website)==0){
            printf("Current Password\n");
            printf("%s : %s \n",current->account, current->password);
            printf("Enter the new password:\n");
            scanf("%99s",new_password);
            strcpy(current->password, new_password);
            return;
        }
        current = current->next;
    }

    printf("Error: Password not been added!\n");

}

/* Function to delete password */
void delete_password(struct p_entry **head, const char* account){

    struct p_entry *current = *head;
    struct p_entry *prev = NULL;
    while(current != NULL && (strcmp(current->account, account)!=0)) {
        prev = current;
        current = current->next;
    }

    if(current == NULL){
        printf("Error: Website not added!\n");
        return;
    }

    if(prev == NULL){
        *head = current->next;
    }
    else{
        prev->next = current->next;
    }

    free(current);

}

/* Function to list passwords */
void list_password(struct p_entry **head){

    struct p_entry *current = *head;

    while(current != NULL){
        printf("%s : %s\n",current->account, current->password);
        current = current->next;
    }

    printf("\n");
}

void p_user(){

    int choice;
    char account[100];
    do
    {
        printf("1. Add Password\n2. Retrieve Password\n3. Update Password\n4. Delete Password\n5. List Passwords\n6. Exit\n");

        printf("Enter the choice:\n");
        scanf("%d",&choice);

        switch(choice){
            case 1:
                printf("Add Password\n");
                add_password(&head);
                break;
            case 2:
                printf("Retrieve Password\n");
                printf("Account:\n");
                scanf("%s",account);
                retrieve_password(&head, account);
                break;
            case 3:
                printf("Update Password\n");
                printf("Account:\n");
                scanf("%s",account);
                update_password(&head, account);
                break;
            case 4:
                printf("Delete Password\n");
                printf("Website:\n");
                scanf("%s",account);
                delete_password(&head, account);
                break;
            case 5:
                printf("List Passwords\n");
                list_password(&head);
                break;
            case 6:
                printf("Exit\n");
                exit(0);
            default:
                printf("Enter a valid choice.\n");
        }
    }while(choice!=6);

}

int main(){

    int choice;
    char username[100];
    char password[100];
    do{
    printf("1. Login\n2. Add Account\n3. Exit\n");
    printf("Enter a choice\n");
    scanf("%d",&choice);
    switch(choice){
        case 1:
            printf("Username:\n");
            scanf("%s",username);
            if(check_user(username, &head2)){
                printf("Password:\n");
                scanf("%s",password);
                int r = verify_password(password, username, &head2);
                printf("result of verify password%d\n",r);
                if(verify_password(password, username, &head2)==1){
                    p_user();
                }
                else{
                    printf("Invalid password!\n");
                }
            }
            else{
                printf("Invalid username!\n");
            }
            break;
        case 2:
            add_user(&head2);
            break;
        case 3:
            exit(0);
        default:
            printf("Enter a valid choice!\n");

     }
    }while(choice!=3);

}