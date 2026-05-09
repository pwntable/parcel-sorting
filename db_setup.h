#ifndef DB_SETUP_H
#define DB_SETUP_H

#include <sqlite3.h>

// Function declarations
int create_tables(sqlite3 *db);
int init_database(sqlite3 **db);

#endif