#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <argon2.h>

#ifndef USER_H
#define USER_H

#define MAX_USERS 10
#define MAX_USERNAME_LENGTH 200
#define MAX_PASSWORD_LENGTH 200

/* Structure for each user */
struct User {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char password_hash[MAX_PASSWORD_LENGTH];
};

int register_user();
int verify_user(char *password, char *base_password);
int authenticate_user();
int load_users_from_file();
int save_users_to_file();
int load_users_from_file();
#endif