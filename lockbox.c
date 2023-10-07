#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <argon2.h>


struct p_entry{
    char website[100];
    char password[100];
    struct p_entry *next;
};

struct user{
    char username[100];
    uint8_t *password_hash;
    struct user *next;
};

struct p_entry* head = NULL;
struct user* head2 = NULL;

void add_password(){

    struct p_entry* new = (struct p_entry*)malloc(sizeof(struct p_entry));
    if(new == NULL){
        perror("Memory allocation error!\n");
        exit(0);
    }
    printf("Account:\n");
    scanf("%99s",new->website);
    printf("Password:\n");
    scanf("%99s",new->password);

    new->next = head;
    head = new;
}

void add_user_from_file(struct user **head2, char *username, char *password)
{
    struct user* new_user = (struct user*)malloc(sizeof(struct user));
    if( new_user == NULL)
    {
    perror("Memory allocation error!\n");
    exit(EXIT_FAILURE);
    }

    strcpy(new_user->username, username);
    strcpy(new_user->password_hash, password);

    new_user->next = *head2;
    *head2 = new_user;
}

void load_data(struct user **head2){

    char username[100];
    char password[100];
    FILE *fp = fopen("users.txt", "r");
    if(fp == NULL){
        perror("Unable to open file for reading.\n");
        exit(EXIT_FAILURE);
    }

    while(fscanf(fp, "%s %s\n", username, password)==1){
        add_user_from_file(head2, username, password);
    }

    fclose(fp);

}

void save_data_to_file(struct user **head2){

    FILE *fp = fopen("users.txt","w");
    if(fp == NULL){
        perror("Unable to open file to store data.\n");
        exit(EXIT_FAILURE);
    }

    struct user *current = *head2;
    while(current!=NULL){
        fprintf(fp, "%s %s\n", current->username, current->password_hash);
        current = current->next;
    }

    fclose(fp);
}




void add_user(struct user **head2){

    char password[100];
    struct user* new = (struct user*)malloc(sizeof(struct user));
    if(new == NULL){
        perror("Memory allocation error: Unable to create a new user.\n");
        exit(EXIT_FAILURE);
    }
    printf("Username:\n");
    scanf("%s", new->username);
    printf("Password:\n");
    scanf("%s",password);

    const char *salt = "random_salt";
    const int iterations = 10;    // Number of iterations
    const int memory = 65536;     // Amount of memory (in KiB)
    const int threads = 4;
    const size_t desired_hash_length = 16; // Change this to your desired hash length in bytes

    new->password_hash = (uint8_t *)malloc(desired_hash_length);
    if (new->password_hash == NULL){
        perror("Memory allocation error: Unable to allocate memory for the password hash.\n");
        exit(EXIT_FAILURE);
    }


   int result = argon2_hash(iterations, memory, threads, password, strlen(password), salt, 
                        strlen(salt), new->password_hash, desired_hash_length, NULL,
                        0, Argon2_id, ARGON2_VERSION_13);

    if (result != ARGON2_OK) {
        fprintf(stderr, "Error hashing password: %s\n", argon2_error_message(result));
        exit(EXIT_FAILURE);
    }


    new->next = *head2;
    *head2 = new;
    printf("New user added.\n");
    save_data_to_file(head2);
    load_data(head2);

}

void retrieve_password(struct p_entry** head, const char* website){

    struct p_entry *current = *head;
    while(current != NULL){
        if(strcmp(current->website, website)==0){
            printf("%s : %s \n",current->website, current->password);
            return;
        }
        current = current->next;
    }

    printf("Error: Password not been added!\n");
}

void update_password(struct p_entry** head, const char* website){

    char new_password[100];
    struct p_entry *current = *head;
    while(current != NULL){
        if(strcmp(current->website, website)==0){
            printf("Current Password\n");
            printf("%s : %s \n",current->website, current->password);
            printf("Enter the new password:\n");
            scanf("%99s",new_password);
            strcpy(current->password, new_password);
            return;
        }
        current = current->next;
    }

    printf("Error: Password not been added!\n");

}


void delete_password(struct p_entry **head, const char* website){

    struct p_entry *current = *head;
    struct p_entry *prev = NULL;
    while(current != NULL && (strcmp(current->website, website)!=0)) {
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

void list_password(struct p_entry **head){

    struct p_entry *current = *head;

    while(current != NULL){
        printf("%s : %s\n",current->website, current->password);
        current = current->next;
    }

    printf("\n");
}

int check_user(const char *username, struct user **head2){

    struct user *current = *head2;

    while(current != NULL){
        if( strcmp(username, current->username) == 0 ){
            return 1; // Found a matching username
        }
        current = current->next;
    }
    return 0;

}

int verify_password( char *input_password, char *username, struct user **head2) {

    struct user *current = *head2;
    const size_t desired_hash_length = 16;
    while(current != NULL){
         if( strcmp(username, current->username)==0 ){
             // Hash the user input using the same Argon2 parameters as during storage
            char hash[desired_hash_length];
            const char *salt = "random_salt"; // Use the same salt
            const int iterations = 10;        // Use the same number of iterations
            const int memory = 65536;         // Use the same memory size
            const int threads = 4;            // Use the same number of threads

            int result = argon2_hash(iterations, memory, threads, input_password, strlen(input_password), salt,
                strlen(salt), hash, desired_hash_length, NULL, 0, Argon2_id, ARGON2_VERSION_13);
            if (result != ARGON2_OK) {
                fprintf(stderr, "Error hashing input password: %s\n", argon2_error_message(result));
                return 0; // Password verification failed
            }
            // Compare the computed hash with the stored hashed password
            if (memcmp(hash, current->password_hash, desired_hash_length) == 0) {
                return 1; // Passwords match; grant access
            } else {
                return 0; // Passwords do not match; deny access
            }
          }
         current = current->next;
    }

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
                int r=verify_password(password, username, &head2);
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