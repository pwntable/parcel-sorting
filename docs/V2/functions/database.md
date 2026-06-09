# Database Module

- **Source File**: [database.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/database.c)
- **Header File**: [database.h](file:///Users/mac/Documents/GitHub/parcel-sorting/include/database.h)
- **Module Owner**: Piki
- **Core Responsibility**: Load and save users, addresses, and parcel data from and to CSV database files.

---

## Overview
The Database module acts as the persistence engine for the application. It handles flat-file serialization and deserialization using standard comma-separated values (CSV). When the application starts, this module reads the tables for users, addresses, and parcels into RAM. On exit (or after crucial write actions), it writes the updated states back to disk.

---

## Functions

### `parse_csv_field`
```c
static char* parse_csv_field(char **line);
```
- **Purpose**: Parses a single field out of a comma-separated text string.
- **Parameters**:
  | Parameter | Type | Description |
  |-----------|------|-------------|
  | `line` | `char**` | Pointer to the pointer of the current position in the CSV line buffer. |
- **Returns**: `char*` — Pointer to the beginning of the parsed string field.
- **Key Logic**:
  - Finds the next comma character using `strchr`.
  - Replaces it with a null terminator (`\0`) to separate the field.
  - Updates the `line` pointer to point immediately after the comma.
  - If no comma is found, strips trailing newlines/carriage returns (`\n`, `\r`) and null-terminates the end.

---

### `load_parcels_from_file`
```c
int load_parcels_from_file(ParcelNode **head, const char *filename);
```
- **Purpose**: Reads parcel records from a CSV file and populates the linked list.
- **Parameters**:
  | Parameter | Type | Description |
  |-----------|------|-------------|
  | `head` | `ParcelNode**` | Double pointer to the head of the parcel linked list. |
  | `filename` | `const char*` | File path of the parcels CSV database (e.g. `"dataset/parcels.csv"`). |
- **Returns**: `int` — Total count of parcels successfully loaded, or `0` if the file could not be opened.
- **Key Logic**:
  - Skips the first line of the CSV (column header row).
  - Reads line-by-line using `fgets`.
  - Calls `parse_csv_field` to extract: ID, sender, receiver, road ID, delivery type, status, house number, time in, time out, and rider ID.
  - Skips incomplete/corrupt lines (lines with empty parcel IDs).
  - Automatically inserts each parcel using `insert_parcel`, ensuring the list is sorted in priority order immediately upon loading.

---

### `save_parcels_to_file`
```c
int save_parcels_to_file(ParcelNode *head, const char *filename);
```
- **Purpose**: Writes the current state of the parcel linked list back to a CSV file.
- **Parameters**:
  | Parameter | Type | Description |
  |-----------|------|-------------|
  | `head` | `ParcelNode*` | Pointer to the head of the parcel list. |
  | `filename` | `const char*` | Destination file path. |
- **Returns**: `int` — `1` on success, `0` on error.
- **Key Logic**:
  - Opens/creates the target file for writing (`"w"` mode).
  - Prints the CSV header columns: `parcel_id,sender_name,receiver_name,address_id,delivery_type,status,house_number,time_in,time_out,rider_id`.
  - Traverses the linked list node-by-node and records all attributes in a standard CSV row.

---

### `load_users`
```c
int load_users(User users[], int max, const char *filename);
```
- **Purpose**: Loads user accounts from a users CSV database file into the `User` struct array.
- **Parameters**:
  | Parameter | Type | Description |
  |-----------|------|-------------|
  | `users[]` | `User` | Array of User structures to populate. |
  | `max` | `int` | Maximum capacity of the user array. |
  | `filename` | `const char*` | File path. |
- **Returns**: `int` — The count of users successfully loaded, or `0` if loading failed.
- **Key Logic**:
  - Skips the header row.
  - Extracts fields: user ID, username, password, role index, and assigned road/address ID.
  - Stops when file EOF is reached or when array capacity limit (`max`) is hit.

---

### `save_users_to_file`
```c
int save_users_to_file(User users[], int count, const char *filename);
```
- **Purpose**: Serializes and writes user array records to the users CSV file.
- **Parameters**:
  | Parameter | Type | Description |
  |-----------|------|-------------|
  | `users[]` | `User` | Array of users to serialize. |
  | `count` | `int` | Total users to save. |
  | `filename` | `const char*` | Destination file path. |
- **Returns**: `int` — `1` on success, `0` on error.
- **Key Logic**:
  - Overwrites the target file and records headers: `user_id,username,password,role,assigned_address_id`.
  - Writes details of each user row.

---

### `load_addresses`
```c
int load_addresses(Address addresses[], int max, const char *filename);
```
- **Purpose**: Deserializes addresses CSV database records into the address struct array.
- **Parameters**:
  | Parameter | Type | Description |
  |-----------|------|-------------|
  | `addresses[]` | `Address` | Array of addresses to populate. |
  | `max` | `int` | Maximum size of the address array. |
  | `filename` | `const char*` | File path. |
- **Returns**: `int` — The count of loaded addresses, or `0` on error.
- **Key Logic**:
  - Skips header row.
  - Extracts: ID, street name, city, state, and house number.
  - Populates the address database array.

---

### `save_addresses_to_file`
```c
int save_addresses_to_file(Address addresses[], int count, const char *filename);
```
- **Purpose**: Serializes and saves the address database array back to a CSV file.
- **Parameters**:
  | Parameter | Type | Description |
  |-----------|------|-------------|
  | `addresses[]` | `Address` | Array of addresses to serialize. |
  | `count` | `int` | Total addresses. |
  | `filename` | `const char*` | Destination file path. |
- **Returns**: `int` — `1` on success, `0` on error.

---

### `get_next_parcel_id`
```c
int get_next_parcel_id(ParcelNode *head);
```
- **Purpose**: Calculates the next unique parcel ID to assign.
- **Parameters**:
  | Parameter | Type | Description |
  |-----------|------|-------------|
  | `head` | `ParcelNode*` | Pointer to the head of the current parcel linked list. |
- **Returns**: `int` — The next unique integer ID (`max_id + 1`).
- **Key Logic**:
  - Traverses the linked list to find the maximum `parcel_id` value currently in use.
  - Returns that maximum value incremented by 1.
  - If the list is empty, returns `1`.
