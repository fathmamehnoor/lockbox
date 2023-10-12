#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <argon2.h>


/* Structure for password entry for each user */
struct p_entry{
    char account[100];
    char password[100];
    struct p_entry *next;
};

struct p_entry* head = NULL;
/* Function to save data form linked list to file */
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

/* Function to add user to linked list from file */
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

/* Function to load data from file to linked list */
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

/* Function to add users */
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


/* Function to check if user has been added */
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

/* Function to verify password of the user */
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