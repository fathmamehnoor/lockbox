#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <argon2.h>

#ifndef ENTRIES_H
#define ENTRIES_H

/* Structure for password entry for each user */
struct p_entry{
    char account[100];
    char password[100];
    struct p_entry *next;
};

extern struct p_entry* head;

void initializeLinkedList2();
void add_password();
void retrieve_password(struct p_entry** head, const char* account);
void update_password(struct p_entry** head, const char* account);
void delete_password(struct p_entry **head, const char* account);
void list_password(struct p_entry **head);
struct p_entry* load_data_from_file(const char* filename);
void save_data_to_file(struct p_entry* head,const char *filename);
void p_user();


#endif