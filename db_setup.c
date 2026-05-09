#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "db_setup.h"

#define DB_FILE "parcel_system.db"

// Data Structures
typedef struct {
    int uid;
    char role[20];
    char username[50];
    char password[100];
} User;

typedef struct {
    int rider_id;
    int uid;
    char name[100];
    char transport[50];
} Rider;

typedef struct {
    int address_id;
    char lot[30];
    char street[100];
} Address;

typedef struct {
    int parcel_id;
    char customer_name[100];
    int address_id;
    char time_in[30];
    char time_out[30];
    char parcel_type[20];
    int rider_id;
    char status[30];
} Parcel;

// Helper to print SQLite error
void print_error(sqlite3 *db, const char *message) {
    fprintf(stderr, "%s: %s\n", message, sqlite3_errmsg(db));
}

// Create all tables
int create_tables(sqlite3 *db) {
    char *err_msg = NULL;
    int rc;

    const char *sql[] = {
        "CREATE TABLE IF NOT EXISTS User ("
        " UID INTEGER PRIMARY KEY AUTOINCREMENT,"
        " role TEXT NOT NULL CHECK(role IN ('admin', 'rider')),"
        " userName TEXT UNIQUE NOT NULL,"
        " password TEXT NOT NULL"
        ");",

        "CREATE TABLE IF NOT EXISTS Rider ("
        " rider_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " UID INTEGER UNIQUE NOT NULL,"
        " name TEXT NOT NULL,"
        " transport TEXT,"
        " FOREIGN KEY (UID) REFERENCES User(UID) ON DELETE CASCADE"
        ");",

        "CREATE TABLE IF NOT EXISTS Address ("
        " address_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " lot TEXT,"
        " street TEXT NOT NULL"
        ");",

        "CREATE TABLE IF NOT EXISTS Parcel ("
        " parcel_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " customer_name TEXT NOT NULL,"
        " address_id INTEGER NOT NULL,"
        " time_in TEXT NOT NULL,"
        " time_out TEXT,"
        " parcel_type TEXT NOT NULL CHECK(parcel_type IN ('normal', 'fast')),"
        " rider_id INTEGER,"
        " status TEXT NOT NULL DEFAULT 'pending' CHECK(status IN ('pending', 'out_to_delivery', 'delivered')),"
        " FOREIGN KEY (address_id) REFERENCES Address(address_id),"
        " FOREIGN KEY (rider_id) REFERENCES Rider(rider_id)"
        ");"
    };

    for (int i = 0; i < 4; i++) {
        rc = sqlite3_exec(db, sql[i], 0, 0, &err_msg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error creating table %d: %s\n", i+1, err_msg);
            sqlite3_free(err_msg);
            return rc;
        }
    }

    printf("✅ All tables created successfully!\n");
    return SQLITE_OK;
}

// Initialize Database (Open + Create Tables)
int init_database(sqlite3 **db) {
    int rc;

    rc = sqlite3_open(DB_FILE, db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(*db));
        return rc;
    }

    sqlite3_exec(*db, "PRAGMA foreign_keys = ON;", 0, 0, NULL);
    printf("✅ Database opened successfully!\n");

    return create_tables(*db);
}