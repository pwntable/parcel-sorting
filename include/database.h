#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

// ==================== DATA STRUCTURES ====================
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

// ==================== FUNCTION DECLARATIONS ====================
void print_error(sqlite3 *db, const char *message);
int create_tables(sqlite3 *db);
int init_database(sqlite3 **db);

#endif