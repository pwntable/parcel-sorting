#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../include/status.h"
#include "../include/database.h"

int is_valid_transition(const char *current_status, const char *new_status) {
    if (current_status == NULL || new_status == NULL) {
        return 0;
    }

    if (strcmp(current_status, "pending") == 0 &&
        strcmp(new_status, "out_to_delivery") == 0) {
        return 1;
    }

    if (strcmp(current_status, "out_to_delivery") == 0 &&
        strcmp(new_status, "delivered") == 0) {
        return 1;
    }

    return 0;
}

void display_status_options(void) {
    printf("\nAvailable parcel status options:\n");
    printf("1. pending\n");
    printf("2. out_to_delivery\n");
    printf("3. delivered\n");
}

void get_current_time(char *buffer, int size) {
    time_t now;
    struct tm *time_info;

    if (buffer == NULL || size <= 0) {
        return;
    }

    now = time(NULL);
    time_info = localtime(&now);

    if (time_info == NULL) {
        snprintf(buffer, size, "");
        return;
    }

    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", time_info);
}

int update_parcel_status(sqlite3 *db, int parcel_id, const char *new_status) {
    sqlite3_stmt *stmt;
    const char *select_sql = "SELECT status FROM Parcel WHERE parcel_id = ?;";
    const char *update_status_sql = "UPDATE Parcel SET status = ? WHERE parcel_id = ?;";
    const char *update_delivered_sql = "UPDATE Parcel SET status = ?, time_out = ? WHERE parcel_id = ?;";
    char current_status[30];
    char current_time[30];
    int rc;

    if (db == NULL || parcel_id <= 0 || new_status == NULL) {
        return 0;
    }

    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        print_error(db, "Failed to prepare parcel status search");
        return 0;
    }

    sqlite3_bind_int(stmt, 1, parcel_id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return 0;
    }

    snprintf(current_status, sizeof(current_status), "%s", sqlite3_column_text(stmt, 0));
    sqlite3_finalize(stmt);

    if (!is_valid_transition(current_status, new_status)) {
        return -1;
    }

    if (strcmp(new_status, "delivered") == 0) {
        get_current_time(current_time, sizeof(current_time));

        rc = sqlite3_prepare_v2(db, update_delivered_sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            print_error(db, "Failed to prepare delivered status update");
            return 0;
        }

        sqlite3_bind_text(stmt, 1, new_status, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, current_time, -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 3, parcel_id);
    } else {
        rc = sqlite3_prepare_v2(db, update_status_sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            print_error(db, "Failed to prepare status update");
            return 0;
        }

        sqlite3_bind_text(stmt, 1, new_status, -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, parcel_id);
    }

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        print_error(db, "Failed to update parcel status");
        return 0;
    }

    return 1;
}
