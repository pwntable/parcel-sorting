# V2 Module Specifications & Symbol Registry

This document lists all structs, global variables/constants, and functions created or refactored for the **Modular Parcel Sorting & Rider Assignment System (V2)**.

---

## 1. Struct Definitions (Core Relational Schema)

### 1.1 `Address` Struct (Declared in `address.h`)
Represents physical streets/roads in the sorting system database.
```c
typedef struct {
    int address_id;          // Primary Key (Unique Address/Street ID)
    char street[100];        // Name of the street
    char city[50];           // City name
    char state[50];          // State name
    int house_number;        // House number (used for final ordering)
} Address;
```

### 1.2 `User` Struct (Declared in `login.h`)
Represents registered riders and administrative accounts.
```c
typedef struct {
    int user_id;             // Primary Key (Unique User ID)
    char username[30];       // Unique alphanumeric username
    char password[30];       // Password string
    int role;                // 0 for ROLE_ADMIN, 1 for ROLE_RIDER
    int assigned_address_id; // Foreign Key pointing to Address ID. 0 (NULL) for Admins.
} User;
```

### 1.3 `Parcel` & `ParcelNode` Struct (Declared in `parcel_list.h`)
Represents customer parcel entries and the linked list node structure.
```c
typedef struct {
    int parcel_id;            // Primary Key (Unique Parcel ID)
    char sender_name[50];     // Sender name
    char receiver_name[50];   // Receiver name
    int address_id;           // Foreign Key pointing to Address ID
    char delivery_type[10];   // "Fast" or "Standard"
    char status[20];          // "Pending", "Out for Delivery", or "Delivered"
    int house_number;         // Specific house number on the street
    char time_in[20];         // Timestamp of creation (YYYY-MM-DD HH:MM)
    char time_out[20];        // Timestamp of delivery
    int rider_id;             // Foreign Key pointing to User ID (Assigned Rider)
} Parcel;

typedef struct ParcelNode {
    Parcel data;              // Holds the Parcel payload
    struct ParcelNode *next;  // Pointer to the next node in the list
} ParcelNode;
```

---

## 2. Global Constants & Variable Settings
* `MAX_USERS`: `10` — Maximum allowable registered riders/users.
* `MAX_ADDRESSES`: `50` — Maximum registered streets/addresses.
* `MAX_ATTEMPTS`: `3` — Maximum consecutive failed login attempts before lockout.
* `ROLE_ADMIN`: `0`
* `ROLE_RIDER`: `1`
* `ROLE_INVALID`: `-1`

---

## 3. Function Registry & Module breakdown

### 3.1 `login` Module (`src/login.c`, `include/login.h`)
* `int login(User users[], int user_count)`
  Runs the main credentials entry loop with attempt counts. Returns the authenticated user's array index or `ROLE_INVALID`.
* `int verify_credentials(User users[], int user_count, const char *username, const char *password)`
  Scans user database. Returns index on success, otherwise `ROLE_INVALID`.
* `int register_rider(User users[], int *user_count, const char *username, const char *password, int assigned_address_id)`
  Registers a rider. **[V2 Feature]** returns `-3` if the target address is occupied.
* `int update_user(User users[], int user_count, int user_id, const char *new_username, int new_role, int new_assigned_address_id)`
  Updates user settings. **[V2 Feature]** returns `-3` if changing road assignment to an occupied address.
* `int delete_user(User users[], int *user_count, int user_id)`
  Deletes the specified user index. Returns `-2` if attempting to delete the system Admin.
* `void display_user_list(User users[], int user_count, Address addresses[], int addr_count)`
  Outputs all users in a tabular block.
* `void display_address_list_with_riders(Address addresses[], int addr_count, User users[], int user_count)`
  **[V2 Feature]** Renders addresses side-by-side with their assigned rider's username (or `NULL` if vacant).

### 3.2 `address` Module (`src/address.c`, `include/address.h`)
* `int add_address(Address addresses[], int *count, Address new_addr)`
  Inserts a new street into the database, generating sequential IDs.
* `int update_address(Address addresses[], int count, int address_id)`
  Updates street name, city, or state parameters.
* `Address* find_address(Address addresses[], int count, int address_id)`
  Finds address pointer matching ID, returning `NULL` if absent.

### 3.3 `status` Module (`src/status.c`, `include/status.h`)
* `int update_parcel_status(ParcelNode **head, int parcel_id, const char *new_status)`
  Sets status on matching parcel ID, writing timestamp if `Delivered`.
* `int is_valid_transition(const char *current_status, const char *new_status)`
  **[V2 Feature]** Implements strict state-machine constraints for transitions.
* `void display_dynamic_status_options(const char *current_status)`
  **[V2 Feature]** Renders state choices available for a parcel.
* `void get_current_time(char *buffer, int size)`
  Fills a string with local timestamp (`YYYY-MM-DD HH:MM`).

### 3.4 `sorting` Module (`src/sorting.c`, `include/sorting.h`)
* `ParcelNode* generate_sorted_queue(ParcelNode *head)`
  Generates and prioritizes a new sorted copy list, sorting standard parcels after fast ones, prioritized by street ID then ascending house numbers.
* `void split_by_delivery_type(ParcelNode *active, ParcelNode **fast_head, ParcelNode **standard_head)`
  Partitions parcels by Fast vs Standard type.
* `void sort_by_house_number(ParcelNode **head)`
  Sorts a sub-list by house number in ascending order.
* `ParcelNode* merge_sorted_lists(ParcelNode *fast, ParcelNode *standard)`
  Combines Fast and Standard lists, maintaining the sorting constraints.
