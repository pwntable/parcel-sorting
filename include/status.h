#ifndef STATUS_H
#define STATUS_H

#include <sqlite3.h>
#include "parcel_list.h"

int update_parcel_status(sqlite3 *db, int parcel_id, const char *new_status);
int is_valid_transition(const char *current_status, const char *new_status);
void display_status_options(void);
void get_current_time(char *buffer, int size);

#endif
