#ifndef LOGIN_H
#define LOGIN_H

#include "address.h"

#define MAX_ATTEMPTS 3
#define ROLE_ADMIN 0
#define ROLE_RIDER 1
#define ROLE_INVALID -1
#define MAX_USERS 100

typedef struct {
    int user_id;             // Primary Key (Unique User ID)
    char username[30];
    char password[30];
    int role;  // ROLE_ADMIN or ROLE_RIDER
    int assigned_address_id; // Mandatory for Riders (valid ID), 0 (NULL/N/A) for Admins
} User;

int login(User users[], int user_count);
void display_login_prompt(char *username, char *password);
int verify_credentials(User users[], int user_count, const char *username, const char *password);

// User Management
int register_rider(User users[], int *user_count, const char *username, const char *password, int assigned_address_id);
int update_user(User users[], int user_count, int user_id, const char *new_username, int new_role, int new_assigned_address_id);
int delete_user(User users[], int *user_count, int user_id);
void display_user_list(User users[], int user_count, Address addresses[], int addr_count);
void display_address_list_with_riders(Address addresses[], int addr_count, User users[], int user_count);

#endif
