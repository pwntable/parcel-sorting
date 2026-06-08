#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/address.h"
#include "../include/validation.h"

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

Address* find_address(Address addresses[], int count, int address_id) {
    for (int i = 0; i < count; i++) {
        if (addresses[i].address_id == address_id) {
            return &addresses[i];
        }
    }
    return NULL;
}

int update_address(Address addresses[], int count, int address_id) {
    Address *addr = find_address(addresses, count, address_id);
    if (addr == NULL) return 0;

    char temp[100];

    printf("Updating Address ID: %d\n", address_id);

    printf("Enter New Street (Current: %s, leave empty to keep): ", addr->street);
    safe_read_string(temp, 100);
    trim_whitespace(temp);
    if (strlen(temp) > 0) {
        strcpy(addr->street, temp);
    }

    printf("Enter New City (Current: %s, leave empty to keep): ", addr->city);
    safe_read_string(temp, 50);
    trim_whitespace(temp);
    if (strlen(temp) > 0) {
        strcpy(addr->city, temp);
    }

    printf("Enter New State (Current: %s, leave empty to keep): ", addr->state);
    safe_read_string(temp, 50);
    trim_whitespace(temp);
    if (strlen(temp) > 0) {
        strcpy(addr->state, temp);
    }

    return 1;
}

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
