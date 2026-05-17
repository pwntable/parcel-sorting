#ifndef STATUS_H
#define STATUS_H

#include "parcel_list.h"

// Update status of a parcel by ID
int update_parcel_status(ParcelNode **head, int parcel_id, const char *new_status);

// Check if a status transition is valid
int is_valid_transition(const char *current_status, const char *new_status);

// Display status options
void display_status_options(void);

// Get current timestamp string
void get_current_time(char *buffer, int size);

#endif
