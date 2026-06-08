#ifndef DATABASE_H
#define DATABASE_H

#include "parcel_list.h"
#include "login.h"
#include "address.h"

// Load/Save parcels (linked list)
int load_parcels_from_file(ParcelNode **head, const char *filename);
int save_parcels_to_file(ParcelNode *head, const char *filename);

// Load/Save users (array)
int load_users(User users[], int max, const char *filename);

// Load/Save addresses (array)
int load_addresses(Address addresses[], int max, const char *filename);

// Generate next unique parcel ID
int get_next_parcel_id(ParcelNode *head);

// Init mock data
void init_mock_database(ParcelNode **head, User users[], int *user_count, Address addresses[], int *address_count);

#endif