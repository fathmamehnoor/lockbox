#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <argon2.h>
#include <inttypes.h>
#include <sodium.h>
#include "user.h"

struct User users[MAX_USERS];
int userCount = 0;

int register_user() {
    if (userCount >= MAX_USERS) {
        printf("User limit reached. Cannot register more users.\n");
        return 0;
    }

    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char password_hash[MAX_PASSWORD_LENGTH];

    printf("Enter a username: ");
    scanf("%s", username);

    // Check if the username is already taken
    for (int i = 0; i < userCount; i++) {
        if (strcmp(username, users[i].username) == 0) {
            printf("Username already exists. Please choose another.\n");
            return 0;
        }
    }

    printf("Enter a password: ");
    scanf("%s", password);

    // Store the new user's data
    strcpy(users[userCount].username, username);

    const char *salt = "random_salt";
    const int iterations = 10;
    const int memory = 65536;
    const int threads = 4;
    const size_t desired_hash_length = 16;

    int result = argon2_hash(iterations, memory, threads, password, strlen(password), salt, 
                            strlen(salt), password_hash, desired_hash_length, NULL,
                            0, Argon2_id, ARGON2_VERSION_13);

    if (result != ARGON2_OK) {
        fprintf(stderr, "Error hashing password: %s\n", argon2_error_message(result));
        exit(EXIT_FAILURE);
    }

    // Encode the password hash as Base64
    size_t base64Length = sodium_base64_encoded_len(desired_hash_length, sodium_base64_VARIANT_ORIGINAL);
    char base64Hash[base64Length];

    if (sodium_bin2base64(base64Hash, base64Length, password_hash, desired_hash_length, sodium_base64_VARIANT_ORIGINAL) == NULL) {
        perror("Base64 encoding error.\n");
        exit(EXIT_FAILURE);
    }

    strcpy(users[userCount].password_hash, base64Hash);
    userCount++;

    printf("Registration successful.\n");
    return 0;  // Reset loggedIn to 0 after registration
}

int verify_user(char *password, char *base_password){

    char password_hash[MAX_PASSWORD_LENGTH];

    const char *salt = "random_salt";
    const int iterations = 10;
    const int memory = 65536;
    const int threads = 4;
    const size_t desired_hash_length = 16;

     int result = argon2_hash(iterations, memory, threads, password, strlen(password), salt, 
                            strlen(salt), password_hash, desired_hash_length, NULL,
                            0, Argon2_id, ARGON2_VERSION_13);

    if (result != ARGON2_OK) {
        fprintf(stderr, "Error hashing password: %s\n", argon2_error_message(result));
        exit(EXIT_FAILURE);
    }

    size_t base64Length = sodium_base64_encoded_len(desired_hash_length, sodium_base64_VARIANT_ORIGINAL);
    char base64Hash[base64Length];

    if (sodium_bin2base64(base64Hash, base64Length, password_hash, desired_hash_length, sodium_base64_VARIANT_ORIGINAL) == NULL) {
        perror("Base64 encoding error.\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp(base_password, base64Hash) == 0) {
        return 1;  // Passwords match
    } else {
        return 0;  // Passwords do not match
    }
}

int authenticate_user() {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    if (userCount == 0) {
        printf("No users registered. Please register first.\n");
        return 0;
    }

    printf("Username: ");
    scanf("%s", username);

    printf("Password: ");
    scanf("%s", password);

    for (int i = 0; i < userCount; i++) {
        if (strcmp(username, users[i].username) == 0) {
            if (verify_user(password, users[i].password) == 1){
                printf("Login successful. Welcome, %s!\n", users[i].username);
                return 1;
            } else {
                printf("Incorrect password.\n");
                return 0;
            }
        }
    }

    printf("User not found.\n");
    return 0;
}


int load_users_from_file() {
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        return 0;
    }

    while (fscanf(file, "%s %s", users[userCount].username, users[userCount].password) == 2) {
        userCount++;
    }

    fclose(file);
    return 1;
}

int save_users_to_file() {
    FILE *file = fopen("users.txt", "w");
    if (file == NULL) {
        return 0;
    }

    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%s %s\n", users[i].username, users[i].password_hash);
    }

    fclose(file);
    return 1;
}
