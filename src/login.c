#include <stdio.h>
#include <string.h>
#include "../include/login.h"
#include "../include/validation.h"

void display_login_prompt(char *username, char *password) {
    printf("Enter Username: ");
    safe_read_string(username, 30);
    trim_whitespace(username);

    printf("Enter Password: ");
    safe_read_string(password, 30);
    trim_whitespace(password);
}

int verify_credentials(User users[], int user_count, const char *username, const char *password) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return i; // Return user index on success
        }
    }
    return ROLE_INVALID; // Returns -1
}

int login(User users[], int user_count) {
    char username[30], password[30];
    int attempt_count = 0;

    clear_screen();
    printf("=== PARCEL SORTING SYSTEM ===\n");
    printf("Login\n");

    while (attempt_count < MAX_ATTEMPTS) {
        display_login_prompt(username, password);

        if (strlen(username) == 0 || strlen(password) == 0) {
            printf("Field cannot be empty.\n\n");
            attempt_count++;
            continue;
        }

        int user_idx = verify_credentials(users, user_count, username, password);
        if (user_idx != ROLE_INVALID) {
            return user_idx; // Return user index
        }

        printf("Invalid credentials.\n");
        attempt_count++;
        if (MAX_ATTEMPTS - attempt_count > 0) {
            printf("Attempts remaining: %d\n\n", MAX_ATTEMPTS - attempt_count);
        }
    }

    printf("Too many failed attempts. Exiting.\n");
    return ROLE_INVALID;
}

int register_rider(User users[], int *user_count, const char *username, const char *password, int assigned_address_id) {
    if (*user_count >= MAX_USERS) return 0; // Limit reached
    if (assigned_address_id <= 0) return -2; // Road assignment is mandatory for Rider

    // Check if user already exists
    for (int i = 0; i < *user_count; i++) {
        if (strcmp(users[i].username, username) == 0) return -1; // Duplicate username
    }

    // Check duplicate road for Riders
    for (int i = 0; i < *user_count; i++) {
        if (users[i].role == ROLE_RIDER && users[i].assigned_address_id == assigned_address_id) {
            return -3; // Road already assigned to another rider
        }
    }

    // Auto-generate primary key ID (max_id + 1)
    int max_id = 0;
    for (int i = 0; i < *user_count; i++) {
        if (users[i].user_id > max_id) max_id = users[i].user_id;
    }

    users[*user_count].user_id = max_id + 1;
    strcpy(users[*user_count].username, username);
    strcpy(users[*user_count].password, password);
    users[*user_count].role = ROLE_RIDER;
    users[*user_count].assigned_address_id = assigned_address_id;
    (*user_count)++;
    return 1;
}

int update_user(User users[], int user_count, int user_id, const char *new_username, int new_role, int new_assigned_address_id) {
    int found_idx = -1;
    for (int i = 0; i < user_count; i++) {
        if (users[i].user_id == user_id) {
            found_idx = i;
            break;
        }
    }

    if (found_idx == -1) return 0; // User not found

    // If changing username, check if new username already exists elsewhere
    if (strlen(new_username) > 0 && strcmp(users[found_idx].username, new_username) != 0) {
        for (int i = 0; i < user_count; i++) {
            if (strcmp(users[i].username, new_username) == 0) return -1; // Username conflict
        }
        strcpy(users[found_idx].username, new_username);
    }

    // Handle role & address assignment
    int final_role = (new_role == -1) ? users[found_idx].role : new_role;
    int final_assigned_address_id = users[found_idx].assigned_address_id;
    if (final_role == ROLE_RIDER) {
        if (new_assigned_address_id > 0) {
            final_assigned_address_id = new_assigned_address_id;
        } else if (users[found_idx].assigned_address_id <= 0) {
            // Rider must have a valid road assigned
            return -2;
        }

        // Check duplicate road for other users
        for (int i = 0; i < user_count; i++) {
            if (i != found_idx && users[i].role == ROLE_RIDER && users[i].assigned_address_id == final_assigned_address_id) {
                return -3; // Road already assigned to another rider
            }
        }
    }
    
    if (final_role == ROLE_ADMIN) {
        users[found_idx].role = ROLE_ADMIN;
        users[found_idx].assigned_address_id = 0; // Admin has NULL/0 road
    } else if (final_role == ROLE_RIDER) {
        users[found_idx].role = ROLE_RIDER;
        users[found_idx].assigned_address_id = final_assigned_address_id;
    }

    return 1; // Success
}

int delete_user(User users[], int *user_count, int user_id) {
    int found = -1;
    for (int i = 0; i < *user_count; i++) {
        if (users[i].user_id == user_id) {
            if (users[i].role == ROLE_ADMIN) return -2; // Cannot delete admin
            found = i;
            break;
        }
    }

    if (found != -1) {
        for (int i = found; i < *user_count - 1; i++) {
            users[i] = users[i+1];
        }
        (*user_count)--;
        return 1;
    }
    return 0;
}

void display_user_list(User users[], int user_count, Address addresses[], int addr_count) {
    printf("--------------------------------------------------------------------------------\n");
    printf("%-6s | %-20s | %-10s | %-35s\n", "ID", "Username", "Role", "Assigned Road");
    printf("--------------------------------------------------------------------------------\n");
    for (int i = 0; i < user_count; i++) {
        char road_str[50] = "N/A (NULL)";
        if (users[i].role == ROLE_RIDER) {
            Address *addr = find_address(addresses, addr_count, users[i].assigned_address_id);
            if (addr) {
                snprintf(road_str, sizeof(road_str), "%s (%s)", addr->street, addr->city);
            } else {
                snprintf(road_str, sizeof(road_str), "Unknown Address ID: %d", users[i].assigned_address_id);
            }
        }
        printf("%-6d | %-20s | %-10s | %-35s\n", 
               users[i].user_id,
               users[i].username,
               (users[i].role == ROLE_ADMIN) ? "Admin" : "Rider",
               road_str);
    }
    printf("--------------------------------------------------------------------------------\n");
}

void display_address_list_with_riders(Address addresses[], int addr_count, User users[], int user_count) {
    if (addr_count == 0) {
        printf("No addresses registered.\n");
        return;
    }

    printf("----------------------------------------------------------------------------------------------------\n");
    printf("%-5s | %-25s | %-15s | %-15s | %-20s\n", "ID", "Street", "City", "State", "Assigned Rider");
    printf("----------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < addr_count; i++) {
        // Find if any rider is assigned to this address
        char rider_uname[30] = "NULL";
        for (int j = 0; j < user_count; j++) {
            if (users[j].role == ROLE_RIDER && users[j].assigned_address_id == addresses[i].address_id) {
                strcpy(rider_uname, users[j].username);
                break;
            }
        }
        printf("%-5d | %-25.25s | %-15.15s | %-15.15s | %-20s\n",
               addresses[i].address_id,
               addresses[i].street,
               addresses[i].city,
               addresses[i].state,
               rider_uname);
    }
    printf("----------------------------------------------------------------------------------------------------\n");
}
