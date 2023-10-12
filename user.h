#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <argon2.h>

/* Structure for each user */
struct user{
    char username[100];
    uint8_t *password_hash;
    struct user *next;
};

struct user* head2 = NULL;
#ifndef USER_H
#define USER_H
void save_data_to_file(struct user **head2);
void add_user_from_file(struct user **head2, char *username, char *password);
void load_data(struct user **head2);
void add_user(struct user **head2);
int check_user(const char *username, struct user **head2);
int verify_password( char *input_password, char *username, struct user **head2);
#endif