#include "../include/database.h"
#include "../include/parcel_list.h"
#include <stdio.h>
#include <string.h>

// Initialize with mocked data so we don't rely on text files or SQLite for now
void init_mock_database(ParcelNode **head, User users[], int *user_count,
                        Address addresses[], int *address_count) {
  // 1. Mock Users
  users[0].user_id = 1;
  strcpy(users[0].username, "admin");
  strcpy(users[0].password, "admin123");
  users[0].role = ROLE_ADMIN;
  users[0].assigned_address_id = 0; // N/A (NULL) for Admin

  users[1].user_id = 2;
  strcpy(users[1].username, "rider1");
  strcpy(users[1].password, "rider123");
  users[1].role = ROLE_RIDER;
  users[1].assigned_address_id = 101; // Mandatory assigned to Jalan Tun Razak

  *user_count = 2;

  // 2. Mock Addresses
  addresses[0].address_id = 101;
  strcpy(addresses[0].street, "Jalan Tun Razak");
  strcpy(addresses[0].city, "Kuala Lumpur");
  strcpy(addresses[0].state, "WP Kuala Lumpur");
  addresses[0].house_number = 0; // Reference if needed

  addresses[1].address_id = 102;
  strcpy(addresses[1].street, "Jalan Ampang");
  strcpy(addresses[1].city, "Kuala Lumpur");
  strcpy(addresses[1].state, "WP Kuala Lumpur");
  addresses[1].house_number = 0;

  *address_count = 2;

  // 3. Mock Parcels
  Parcel p1;
  p1.parcel_id = 1;
  strcpy(p1.sender_name, "Ali");
  strcpy(p1.receiver_name, "Abu");
  p1.address_id = 101;
  strcpy(p1.delivery_type, "Fast");
  strcpy(p1.status, "Pending");
  p1.house_number = 12;
  strcpy(p1.time_in, "2026-05-13 10:00");
  strcpy(p1.time_out, "");
  p1.rider_id = 0; // Unassigned

  Parcel p2;
  p2.parcel_id = 2;
  strcpy(p2.sender_name, "Siti");
  strcpy(p2.receiver_name, "Ahmad");
  p2.address_id = 102;
  strcpy(p2.delivery_type, "Standard");
  strcpy(p2.status, "Pending");
  p2.house_number = 5;
  strcpy(p2.time_in, "2026-05-13 11:30");
  strcpy(p2.time_out, "");
  p2.rider_id = 0;

  insert_parcel(head, p2);
  insert_parcel(head, p1);
}

int load_parcels_from_file(ParcelNode **head, const char *filename) {
  // Stub: not used when mocking
  return 0;
}

int save_parcels_to_file(ParcelNode *head, const char *filename) {
  // Stub: not used when mocking
  return 0;
}

int load_users(User users[], int max, const char *filename) {
  // Stub: not used when mocking
  return 0;
}

int load_addresses(Address addresses[], int max, const char *filename) {
  // Stub: not used when mocking
  return 0;
}

int get_next_parcel_id(ParcelNode *head) {
  int max_id = 0;
  ParcelNode *current = head;
  while (current != NULL) {
    if (current->data.parcel_id > max_id) {
      max_id = current->data.parcel_id;
    }
    current = current->next;
  }
  return max_id + 1;
}