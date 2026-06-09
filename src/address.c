#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/address.h"
#include "../include/validation.h"

/**
 * @brief Adds a new address to the system's address array.
 * 
 * If the address ID is 0, the function automatically assigns a unique incremented ID
 * based on the current maximum address ID in the array.
 * 
 * @param addresses Array of Address structures.
 * @param count Pointer to the integer tracking the current number of addresses.
 * @param new_addr The new Address structure to add.
 * @return int Returns 1 if the address was added successfully, or 0 if the limit is reached.
 */
int add_address(Address addresses[], int *count, Address new_addr) {
    if (*count >= 50) { // Using 50 as MAX_ADDRESSES from main.c
        return 0;
    }

    // Assign ID if not provided (for new addresses)
    if (new_addr.address_id == 0) {
        int max_id = 0;
        for (int i = 0; i < *count; i++) {
            if (addresses[i].address_id > max_id) max_id = addresses[i].address_id;
        }
        new_addr.address_id = max_id + 1;
    }

    addresses[*count] = new_addr;
    (*count)++;
    return 1;
}

/**
 * @brief Finds an address in the array by its unique address ID.
 * 
 * @param addresses Array of Address structures.
 * @param count Current number of addresses in the array.
 * @param address_id The unique ID to search for.
 * @return Address* Pointer to the matching Address structure, or NULL if not found.
 */
Address* find_address(Address addresses[], int count, int address_id) {
    for (int i = 0; i < count; i++) {
        if (addresses[i].address_id == address_id) {
            return &addresses[i];
        }
    }
    return NULL;
}

/**
 * @brief Interactively updates address details (street, city, and state).
 * 
 * Prompts the user to input new values. If the user presses Enter without typing,
 * the existing field value is preserved.
 * 
 * @param addresses Array of Address structures.
 * @param count Current number of addresses in the array.
 * @param address_id The unique ID of the address to update.
 * @return int Returns 1 if the address was updated successfully, or 0 if not found.
 */
int update_address(Address addresses[], int count, int address_id) {
    Address *addr = find_address(addresses, count, address_id);
    if (addr == NULL) return 0;

    char temp[100];
    char prompt_buf[150];

    printf("Updating Address ID: %d\n", address_id);

    snprintf(prompt_buf, sizeof(prompt_buf), "Enter New Street (Current: %s, leave empty to keep): ", addr->street);
    get_validated_string(prompt_buf, temp, 100, 0, 99, 0, 1);
    if (strlen(temp) > 0) {
        strcpy(addr->street, temp);
    }

    snprintf(prompt_buf, sizeof(prompt_buf), "Enter New City (Current: %s, leave empty to keep): ", addr->city);
    get_validated_string(prompt_buf, temp, 50, 0, 49, 0, 1);
    if (strlen(temp) > 0) {
        strcpy(addr->city, temp);
    }

    snprintf(prompt_buf, sizeof(prompt_buf), "Enter New State (Current: %s, leave empty to keep): ", addr->state);
    get_validated_string(prompt_buf, temp, 50, 0, 49, 0, 1);
    if (strlen(temp) > 0) {
        strcpy(addr->state, temp);
    }

    return 1;
}

/**
 * @brief Prints a formatted table of all registered addresses.
 * 
 * Displays the address ID, street, city, and state.
 * 
 * @param addresses Array of Address structures.
 * @param count Current number of addresses.
 */
void display_address_list(Address addresses[], int count) {
    if (count == 0) {
        printf("No addresses registered.\n");
        return;
    }

    printf("------------------------------------------------------------------\n");
    printf("%-5s | %-25s | %-15s | %-15s\n", "ID", "Street", "City", "State");
    printf("------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-5d | %-25.25s | %-15.15s | %-15.15s\n",
               addresses[i].address_id,
               addresses[i].street,
               addresses[i].city,
               addresses[i].state);
    }
    printf("------------------------------------------------------------------\n");
}

