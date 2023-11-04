#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <argon2.h>
#include "user.h"
#include "entries.h"

struct p_entry* head = NULL;

// Function to initialize the linked list
void initializeLinkedList2() {
    head = NULL;
}

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
void retrieve_password(struct p_entry** head, const char* account){

    struct p_entry *current = *head;
    while(current != NULL){
        if(strcmp(current->account, account)==0){
            printf("%s : %s \n",current->account, current->password);
            return;
        }
        current = current->next;
    }

    printf("Error: Password not been added!\n");
}

/* Function to update password */
void update_password(struct p_entry** head, const char* account){

    char new_password[100];
    struct p_entry *current = *head;
    while(current != NULL){
        if(strcmp(current->account, account)==0){
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

void save_data_to_file(struct p_entry* head,const char *filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file for writing");
        return;
    }

    struct p_entry* current = head;
    while (current != NULL) {
        fprintf(file, "%s %s\n", current->account, current->password);
        current = current->next;
    }

    fclose(file);
}

struct p_entry* load_data_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file for reading");
        return NULL;
    }

    struct p_entry* head = NULL;
    struct p_entry* tail = NULL;

    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    while (fscanf(file, "%s %s", username, password) == 2) {
        struct p_entry* new_user = (struct p_entry*)malloc(sizeof(struct p_entry));
        if (new_user == NULL) {
            perror("Memory allocation error: Unable to create a new user.");
            fclose(file);
            return NULL;
        }

        strcpy(new_user->account, username);
        strcpy(new_user->password, password);
        new_user->next = NULL;

        if (head == NULL) {
            head = new_user;
            tail = new_user;
        } else {
            tail->next = new_user;
            tail = new_user;
        }
    }

    fclose(file);
    return head;
}

void p_user(){

    int choice;
    char account[100];
    head = load_data_from_file("user_entries.txt");
    do
    {
        printf("1. Add Password\n2. Retrieve Password\n3. Update Password\n4. Delete Password\n5. List Passwords\n6. Exit\n");

        printf("Enter the choice:\n");
        scanf("%d",&choice);

        switch(choice){
            case 1:
                printf("Add Password\n");
                add_password(&head);
                save_data_to_file(head, "user_entries.txt");
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
