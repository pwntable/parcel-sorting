#include "../include/database.h"
#include "../include/parcel_list.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

// Helper function to extract fields from a CSV line.
// Replaces the next comma with a null terminator and advances the pointer.
// Handles empty fields correctly (e.g. consecutive commas return empty string "").
static char *parse_csv_field(char **line) {
  if (*line == NULL || **line == '\0') {
    return "";
  }
  char *start = *line;
  char *comma = strchr(start, ',');
  if (comma) {
    *comma = '\0';
    *line = comma + 1;
  } else {
    // Strip trailing newlines
    char *nl = strchr(start, '\n');
    if (nl) *nl = '\0';
    nl = strchr(start, '\r');
    if (nl) *nl = '\0';
    *line = NULL;
  }
  return start;
}

int load_parcels_from_file(ParcelNode **head, const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    return 0;
  }

  char line[512];
  // Skip header line
  if (!fgets(line, sizeof(line), fp)) {
    fclose(fp);
    return 0;
  }

  int count = 0;
  while (fgets(line, sizeof(line), fp)) {
    line[strcspn(line, "\r\n")] = '\0';
    if (strlen(line) == 0) continue;

    char *ptr = line;
    char *parcel_id_str = parse_csv_field(&ptr);
    char *sender_name = parse_csv_field(&ptr);
    char *receiver_name = parse_csv_field(&ptr);
    char *address_id_str = parse_csv_field(&ptr);
    char *delivery_type = parse_csv_field(&ptr);
    char *status = parse_csv_field(&ptr);
    char *house_number_str = parse_csv_field(&ptr);
    char *time_in = parse_csv_field(&ptr);
    char *time_out = parse_csv_field(&ptr);
    char *rider_id_str = parse_csv_field(&ptr);

    // Skip corrupted or incomplete lines
    if (*parcel_id_str == '\0') continue;

    Parcel p = {0};
    p.parcel_id = atoi(parcel_id_str);
    
    strncpy(p.sender_name, sender_name, sizeof(p.sender_name) - 1);
    p.sender_name[sizeof(p.sender_name) - 1] = '\0';
    
    strncpy(p.receiver_name, receiver_name, sizeof(p.receiver_name) - 1);
    p.receiver_name[sizeof(p.receiver_name) - 1] = '\0';
    
    p.address_id = atoi(address_id_str);
    
    strncpy(p.delivery_type, delivery_type, sizeof(p.delivery_type) - 1);
    p.delivery_type[sizeof(p.delivery_type) - 1] = '\0';
    
    strncpy(p.status, status, sizeof(p.status) - 1);
    p.status[sizeof(p.status) - 1] = '\0';
    
    p.house_number = atoi(house_number_str);
    
    strncpy(p.time_in, time_in, sizeof(p.time_in) - 1);
    p.time_in[sizeof(p.time_in) - 1] = '\0';
    
    strncpy(p.time_out, time_out, sizeof(p.time_out) - 1);
    p.time_out[sizeof(p.time_out) - 1] = '\0';
    
    p.rider_id = atoi(rider_id_str);

    insert_parcel(head, p);
    count++;
  }

  fclose(fp);
  return count;
}

int save_parcels_to_file(ParcelNode *head, const char *filename) {
  FILE *fp = fopen(filename, "w");
  if (!fp) {
    return 0;
  }

  // Write CSV Header
  fprintf(fp, "parcel_id,sender_name,receiver_name,address_id,delivery_type,status,house_number,time_in,time_out,rider_id\n");

  ParcelNode *current = head;
  while (current != NULL) {
    Parcel p = current->data;
    fprintf(fp, "%d,%s,%s,%d,%s,%s,%d,%s,%s,%d\n",
            p.parcel_id,
            p.sender_name,
            p.receiver_name,
            p.address_id,
            p.delivery_type,
            p.status,
            p.house_number,
            p.time_in,
            p.time_out,
            p.rider_id);
    current = current->next;
  }

  fclose(fp);
  return 1;
}

int load_users(User users[], int max, const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    return 0;
  }

  char line[256];
  // Skip header line
  if (!fgets(line, sizeof(line), fp)) {
    fclose(fp);
    return 0;
  }

  int count = 0;
  while (fgets(line, sizeof(line), fp) && count < max) {
    line[strcspn(line, "\r\n")] = '\0';
    if (strlen(line) == 0) continue;

    char *ptr = line;
    char *user_id_str = parse_csv_field(&ptr);
    char *username = parse_csv_field(&ptr);
    char *password = parse_csv_field(&ptr);
    char *role_str = parse_csv_field(&ptr);
    char *assigned_address_id_str = parse_csv_field(&ptr);

    if (*user_id_str == '\0') continue;

    users[count].user_id = atoi(user_id_str);
    
    strncpy(users[count].username, username, sizeof(users[count].username) - 1);
    users[count].username[sizeof(users[count].username) - 1] = '\0';
    
    strncpy(users[count].password, password, sizeof(users[count].password) - 1);
    users[count].password[sizeof(users[count].password) - 1] = '\0';
    
    users[count].role = atoi(role_str);
    users[count].assigned_address_id = atoi(assigned_address_id_str);

    count++;
  }

  fclose(fp);
  return count;
}

int save_users_to_file(User users[], int count, const char *filename) {
  FILE *fp = fopen(filename, "w");
  if (!fp) {
    return 0;
  }

  // Write CSV Header
  fprintf(fp, "user_id,username,password,role,assigned_address_id\n");

  for (int i = 0; i < count; i++) {
    fprintf(fp, "%d,%s,%s,%d,%d\n",
            users[i].user_id,
            users[i].username,
            users[i].password,
            users[i].role,
            users[i].assigned_address_id);
  }

  fclose(fp);
  return 1;
}

int load_addresses(Address addresses[], int max, const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    return 0;
  }

  char line[256];
  // Skip header line
  if (!fgets(line, sizeof(line), fp)) {
    fclose(fp);
    return 0;
  }

  int count = 0;
  while (fgets(line, sizeof(line), fp) && count < max) {
    line[strcspn(line, "\r\n")] = '\0';
    if (strlen(line) == 0) continue;

    char *ptr = line;
    char *address_id_str = parse_csv_field(&ptr);
    char *street = parse_csv_field(&ptr);
    char *city = parse_csv_field(&ptr);
    char *state = parse_csv_field(&ptr);
    char *house_number_str = parse_csv_field(&ptr);

    if (*address_id_str == '\0') continue;

    addresses[count].address_id = atoi(address_id_str);
    
    strncpy(addresses[count].street, street, sizeof(addresses[count].street) - 1);
    addresses[count].street[sizeof(addresses[count].street) - 1] = '\0';
    
    strncpy(addresses[count].city, city, sizeof(addresses[count].city) - 1);
    addresses[count].city[sizeof(addresses[count].city) - 1] = '\0';
    
    strncpy(addresses[count].state, state, sizeof(addresses[count].state) - 1);
    addresses[count].state[sizeof(addresses[count].state) - 1] = '\0';
    
    addresses[count].house_number = atoi(house_number_str);

    count++;
  }

  fclose(fp);
  return count;
}

int save_addresses_to_file(Address addresses[], int count, const char *filename) {
  FILE *fp = fopen(filename, "w");
  if (!fp) {
    return 0;
  }

  // Write CSV Header
  fprintf(fp, "address_id,street,city,state,house_number\n");

  for (int i = 0; i < count; i++) {
    fprintf(fp, "%d,%s,%s,%s,%d\n",
            addresses[i].address_id,
            addresses[i].street,
            addresses[i].city,
            addresses[i].state,
            addresses[i].house_number);
  }

  fclose(fp);
  return 1;
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