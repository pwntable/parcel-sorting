#include <stdio.h>
#include <string.h>
#include "../include/address.h"

int add_address(sqlite3 *db, const char *lot, const char *street) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO Address (lot, street) VALUES (?, ?);";
    int rc;

    if (db == NULL || street == NULL || strlen(street) == 0) {
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
    }

    return 1;
}

int update_address(sqlite3 *db, int address_id, const char *lot, const char *street) {
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE Address SET lot = ?, street = ? WHERE address_id = ?;";
    int rc;

    if (db == NULL || address_id <= 0 || street == NULL || strlen(street) == 0) {
        return 0;
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

        printf("%-5d | %-15s | %-40s\n",
               address_id,
               lot == NULL ? "" : (const char *)lot,
               street == NULL ? "" : (const char *)street);
        count++;
    }

    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        print_error(db, "Failed while reading address list");
        return 0;
    }

    if (count == 0) {
        printf("No addresses found.\n");
    }

    return count;
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
    }

    sqlite3_finalize(stmt);
    return 0;
}
