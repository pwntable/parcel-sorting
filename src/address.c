#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/address.h"
#include "../include/validation.h"

int add_address(sqlite3 *db, const char *lot, const char *street) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO Address (lot, street) VALUES (?, ?);";
    int rc;

    if (db == NULL || street == NULL || strlen(street) == 0) {
int add_address(Address addresses[], int *count, Address new_addr) {
    if (*count >= 50) { // Using 50 as MAX_ADDRESSES from main.c
        return 0;
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        print_error(db, "Failed to prepare add address");
        return 0;
    }

    sqlite3_bind_text(stmt, 1, lot == NULL ? "" : lot, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, street, -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        print_error(db, "Failed to add address");
        return 0;
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

int update_address(sqlite3 *db, int address_id, const char *lot, const char *street) {
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE Address SET lot = ?, street = ? WHERE address_id = ?;";
    int rc;

    if (db == NULL || address_id <= 0 || street == NULL || strlen(street) == 0) {
        return 0;
Address* find_address(Address addresses[], int count, int address_id) {
    for (int i = 0; i < count; i++) {
        if (addresses[i].address_id == address_id) {
            return &addresses[i];
        }
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        print_error(db, "Failed to prepare update address");
        return 0;
    }

    sqlite3_bind_text(stmt, 1, lot == NULL ? "" : lot, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, street, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, address_id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        print_error(db, "Failed to update address");
        return 0;
    }

    return sqlite3_changes(db) > 0;
    return NULL;
}

int display_address_list(sqlite3 *db) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT address_id, lot, street FROM Address ORDER BY address_id;";
    int rc;
    int count = 0;

    if (db == NULL) {
        return 0;
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        print_error(db, "Failed to prepare address list");
        return 0;
    }

    printf("\n================ ADDRESS LIST ================\n");
    printf("%-5s | %-15s | %-40s\n", "ID", "Lot", "Street");
    printf("-------------------------------------------------------------\n");

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int address_id = sqlite3_column_int(stmt, 0);
        const unsigned char *lot = sqlite3_column_text(stmt, 1);
        const unsigned char *street = sqlite3_column_text(stmt, 2);
int update_address(Address addresses[], int count, int address_id) {
    Address *addr = find_address(addresses, count, address_id);
    if (addr == NULL) return 0;

        printf("%-5d | %-15s | %-40s\n",
               address_id,
               lot == NULL ? "" : (const char *)lot,
               street == NULL ? "" : (const char *)street);
        count++;
    }
    printf("Updating Address ID: %d\n", address_id);
    printf("Enter New Street (Current: %s): ", addr->street);
    safe_read_string(addr->street, 100);
    trim_whitespace(addr->street);

    sqlite3_finalize(stmt);
    printf("Enter New City (Current: %s): ", addr->city);
    safe_read_string(addr->city, 50);
    trim_whitespace(addr->city);

    if (rc != SQLITE_DONE) {
        print_error(db, "Failed while reading address list");
        return 0;
    }

    if (count == 0) {
        printf("No addresses found.\n");
    }
    printf("Enter New State (Current: %s): ", addr->state);
    safe_read_string(addr->state, 50);
    trim_whitespace(addr->state);

    return count;
    return 1;
}

int find_address(sqlite3 *db, int address_id, Address *address) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT address_id, lot, street FROM Address WHERE address_id = ?;";
    int rc;

    if (db == NULL || address_id <= 0 || address == NULL) {
        return 0;
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        print_error(db, "Failed to prepare find address");
        return 0;
void display_address_list(Address addresses[], int count) {
    if (count == 0) {
        printf("No addresses registered.\n");
        return;
    }

    sqlite3_bind_int(stmt, 1, address_id);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        const unsigned char *lot = sqlite3_column_text(stmt, 1);
        const unsigned char *street = sqlite3_column_text(stmt, 2);

        address->address_id = sqlite3_column_int(stmt, 0);
        snprintf(address->lot, sizeof(address->lot), "%s", lot == NULL ? "" : (const char *)lot);
        snprintf(address->street, sizeof(address->street), "%s", street == NULL ? "" : (const char *)street);
        sqlite3_finalize(stmt);
        return 1;
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

    sqlite3_finalize(stmt);
    return 0;
    printf("------------------------------------------------------------------\n");
}
