#include <stdio.h>
#include <string.h>
#include "../include/login.h"
#include "../include/validation.h"

/**
 * @brief Prompts the user to enter their username and password.
 * 
 * @param username Buffer to store the input username.
 * @param password Buffer to store the input password.
 */
void display_login_prompt(char *username, char *password) {
    get_validated_string("Enter Username: ", username, 30, 1, 29, 0, 0);
    get_validated_string("Enter Password: ", password, 30, 1, 29, 0, 0);
}

/**
 * @brief Verifies if the provided username and password match any registered user.
 * 
 * @param users Array of User structures.
 * @param user_count Current number of users.
 * @param username Username input.
 * @param password Password input.
 * @return int The index of the matching user in the array, or ROLE_INVALID (-1) on failure.
 */
int verify_credentials(User users[], int user_count, const char *username, const char *password) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return i; // Return user index on success
        }
    }
    return ROLE_INVALID; // Returns -1
}

/**
 * @brief Handles user login with a maximum number of login attempts.
 * 
 * Prompts the user iteratively. Limits to MAX_ATTEMPTS (3).
 * 
 * @param users Array of User structures.
 * @param user_count Current number of users.
 * @return int The logged-in user index, or ROLE_INVALID (-1) on failure.
 */
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

/**
 * @brief Registers a new Rider user, ensuring duplicate usernames and road assignments are prevented.
 * 
 * Validates that the Rider has a mandatory assigned road and that the road is not already
 * assigned to another Rider. Generates a unique user ID automatically.
 * 
 * @param users Array of User structures.
 * @param user_count Pointer to the current number of users.
 * @param username New username.
 * @param password New password.
 * @param assigned_address_id Address ID of the road to assign to the Rider.
 * @return int 1 on success, 0 if limit reached, -1 if username duplicate, -2 if road id is invalid/missing, -3 if duplicate road assignment.
 */
int register_rider(User users[], int *user_count, const char *username, const char *password, int assigned_address_id) {
    if (*user_count >= 10) return 0; // Limit reached
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

/**
 * @brief Updates a user's details, enforcing validation on duplicate usernames and duplicate Rider roads.
 * 
 * If a user's role is changed to Admin, their road assignment is automatically reset to 0.
 * If changed to Rider, they must have a valid and unique assigned road.
 * 
 * @param users Array of User structures.
 * @param user_count Current number of users.
 * @param user_id The ID of the user to update.
 * @param new_username New username (leave empty to keep current).
 * @param new_role New role (0: Admin, 1: Rider, -1 to keep current).
 * @param new_assigned_address_id New road ID assignment (-1 to keep current).
 * @return int 1 on success, 0 if user not found, -1 if username conflict, -2 if missing road for Rider, -3 if duplicate road assignment.
 */
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

/**
 * @brief Deletes a user from the system by their user ID.
 * 
 * Prevents deletion of Admin users. Shifts subsequent users in the array to fill the gap.
 * 
 * @param users Array of User structures.
 * @param user_count Pointer to the current number of users.
 * @param user_id The ID of the user to delete.
 * @return int 1 on success, 0 if user not found, -2 if target is Admin.
 */
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

/**
 * @brief Displays a formatted list of all registered users.
 * 
 * Shows user ID, username, role, and assigned road.
 * 
 * @param users Array of User structures.
 * @param user_count Current number of users.
 * @param addresses Array of Address structures.
 * @param addr_count Current number of addresses.
 */
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

/**
 * @brief Displays a list of all addresses along with their assigned riders (if any).
 * 
 * @param addresses Array of Address structures.
 * @param addr_count Current number of addresses.
 * @param users Array of User structures.
 * @param user_count Current number of users.
 */
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
