#ifndef ADDRESS_H
#define ADDRESS_H

#include <sqlite3.h>
#include "database.h"

typedef struct {
    int address_id;
    char street[100];
    char city[50];
    char state[50];
    int house_number;
} Address;

int add_address(sqlite3 *db, const char *lot, const char *street);
int update_address(sqlite3 *db, int address_id, const char *lot, const char *street);
int display_address_list(sqlite3 *db);
int find_address(sqlite3 *db, int address_id, Address *address);

#endif
