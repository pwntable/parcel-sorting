# 📋 Implementation Plan

## Phase Overview

| Phase | Focus | Duration (Suggested) |
|-------|-------|---------------------|
| Phase 1 | Core structure setup | Week 1 |
| Phase 2 | Individual module development | Week 2–3 |
| Phase 3 | Integration & wiring | Week 4 |
| Phase 4 | Testing & bug fixing | Week 5 |

---

## Phase 1: Core Structure

### Goals
- Set up project folder structure
- Define all shared data structures (structs)
- Implement the linked list utility (`parcel_list.c` / `parcel_list.h`)
- Create stub header files for all modules
- Implement `clear_screen` utility in `validation.c`
- Set up `main.c` with basic menu skeleton

### Deliverables

| File | Purpose |
|------|---------|
| `main.c` | Entry point, menu loop |
| `parcel_list.h` | Linked list struct definitions and function prototypes |
| `parcel_list.c` | Linked list operations (insert, delete, search, traverse, free) |
| All `.h` files | Function prototypes for each module |

### Linked List Setup — Core Functions

```c
// parcel_list.h

#ifndef PARCEL_LIST_H
#define PARCEL_LIST_H

typedef struct {
    int parcel_id;
    char sender_name[50];
    char receiver_name[50];
    int address_id;
    char delivery_type[10];   // "Fast" or "Standard"
    char status[20];          // "Pending", "Out for Delivery", "Delivered"
    int house_number;
    char time_in[20];
    char time_out[20];
    int rider_id;
} Parcel;

typedef struct ParcelNode {
    Parcel data;
    struct ParcelNode *next;
} ParcelNode;

// Core linked list operations
void insert_parcel(ParcelNode **head, Parcel new_parcel);
int delete_parcel(ParcelNode **head, int parcel_id);
ParcelNode* find_parcel(ParcelNode *head, int parcel_id);
void free_all_parcels(ParcelNode **head);
int count_parcels(ParcelNode *head);

#endif
```

### Memory Management Rules (Team-Wide)

| Rule | Detail |
|------|--------|
| **Allocation** | Only `insert_parcel()` and `load_parcels_from_file()` call `malloc` |
| **Deallocation** | Only `delete_parcel()` and `free_all_parcels()` call `free` |
| **NULL check** | Always check `malloc` return value |
| **Cleanup** | `free_all_parcels()` must be called before program exits |
| **No dangling** | After `free`, set pointer to `NULL` |

---

## Phase 2: Module Development

Each team member develops their module independently using the shared header files.

---

### Module: Login (Khai) ⭐ DETAILED

#### Description
Authenticates users by comparing input credentials against stored user data. Returns the user's role to determine which menu to display.

#### Input / Output

| Direction | Data |
|-----------|------|
| **Input** | Username (string), Password (string) |
| **Output** | Role code: `0` = Admin, `1` = Rider, `-1` = Failed |

#### Dependencies
- `validation.c` — for input sanitization
- `database.c` — for loading user data from `users.txt`

#### Function Signatures

```c
// login.h
#ifndef LOGIN_H
#define LOGIN_H

#define MAX_ATTEMPTS 3
#define ROLE_ADMIN 0
#define ROLE_RIDER 1
#define ROLE_INVALID -1

typedef struct {
    char username[30];
    char password[30];
    int role;  // ROLE_ADMIN or ROLE_RIDER
} User;

// Main login function — returns role or ROLE_INVALID
int login(User users[], int user_count);

// Display login prompt and get credentials
void display_login_prompt(char *username, char *password);

// Verify credentials against user array
int verify_credentials(User users[], int user_count,
                       const char *username, const char *password);

#endif
```

#### Logic Flow (Detailed)

```
1. Display "=== PARCEL SORTING SYSTEM ==="
2. Display "Login"
3. Set attempt_count = 0

4. LOOP (while attempt_count < MAX_ATTEMPTS):
   a. Prompt "Username: " → read input
   b. Prompt "Password: " → read input
   c. Trim whitespace from both inputs
   d. Validate input (not empty, no special chars)
      - If invalid → print error, increment attempt_count, continue
   e. Loop through users[]:
      - Compare username (case-insensitive)
      - Compare password (case-sensitive)
      - If match found → return users[i].role
   f. No match → print "Invalid credentials"
   g. Increment attempt_count
   h. Print "Attempts remaining: (MAX_ATTEMPTS - attempt_count)"

5. If attempt_count >= MAX_ATTEMPTS:
   a. Print "Too many failed attempts. Exiting."
   b. Return ROLE_INVALID

6. main.c receives role:
   - ROLE_ADMIN → show_admin_menu()
   - ROLE_RIDER → show_rider_menu()
   - ROLE_INVALID → exit program
```

#### Edge Cases

| Edge Case | Handling |
|-----------|----------|
| Empty username/password | Print "Field cannot be empty", re-prompt |
| Username with spaces | Trim leading/trailing spaces |
| Case mismatch in username | Convert both to lowercase before comparing |
| Password with spaces | Keep as-is (spaces are valid in passwords) |
| Buffer overflow attempt | Use `fgets()` with size limit, never `scanf("%s")` or `gets()` |
| Max attempts exceeded | Print lockout message, return `ROLE_INVALID` |
| File not found (users.txt) | database.c handles this — login receives empty array, all logins fail |

#### Validation Handling
- Username: 3–29 characters, alphanumeric only
- Password: 3–29 characters, any printable character
- Use `validate_string_length()` and `validate_alphanumeric()` from `validation.c`

#### Linked List Interaction
- **None** — Login does not access the parcel linked list

---

### Module: Output (Khai) ⭐ DETAILED

#### Description
Formats and displays parcel data to the terminal in clean, aligned tables. Provides different views for Admin and Rider. Reads from the linked list but never modifies it.

#### Input / Output

| Direction | Data |
|-----------|------|
| **Input** | `ParcelNode *head` (linked list), display mode flag |
| **Output** | Formatted terminal output (tables, summaries) |

#### Dependencies
- `parcel_list.h` — for traversing the linked list
- `address.h` — for resolving address_id to full address string
- `sorting.h` — for receiving sorted list

#### Function Signatures

```c
// output.h
#ifndef OUTPUT_H
#define OUTPUT_H

#include "parcel_list.h"
#include "address.h"

// Display all parcels in a formatted table
void display_all_parcels(ParcelNode *head);

// Display sorted parcels (after sorting engine runs)
void display_sorted_parcels(ParcelNode *sorted_head);

// Display parcels filtered by status
void display_parcels_by_status(ParcelNode *head, const char *status);

// Display a single parcel's full details
void display_parcel_detail(Parcel *parcel);

// Display summary statistics
void display_summary(ParcelNode *head);

// Print table header
void print_table_header(void);

// Print table row for one parcel
void print_table_row(Parcel *parcel);

// Print horizontal divider line
void print_divider(void);

// Admin-specific view
void display_admin_report(ParcelNode *head);

// Rider-specific view
void display_rider_parcels(ParcelNode *head, int rider_id);

#endif
```

#### Logic Flow (Detailed)

##### `display_all_parcels(ParcelNode *head)`
```
1. If head == NULL:
   a. Print "No parcels in system."
   b. Return

2. Print table header:
   "ID | Sender | Receiver | Type | Status | House# | Rider"
   Print divider line

3. Set current = head
4. LOOP (while current != NULL):
   a. Print formatted row with current->data fields
   b. Use %-*s for left-aligned, %*d for right-aligned numbers
   c. current = current->next

5. Print divider line
6. Print total count: "Total: X parcels"
```

##### `display_sorted_parcels(ParcelNode *sorted_head)`
```
1. Print "=== SORTED DELIVERY QUEUE ==="
2. Print "Priority: Fast Delivery first, then Standard"
3. Print "Order: By house number (ascending)"
4. Print blank line

5. Set position = 1
6. Set current_type = "" (track group changes)

7. LOOP (while current != NULL):
   a. If current->data.delivery_type != current_type:
      - Print group header: "--- FAST DELIVERY ---" or "--- STANDARD DELIVERY ---"
      - Update current_type
   b. Print: "position. [ID] Receiver → House #X (Status)"
   c. position++
   d. current = current->next

8. Print blank line
9. Print "Queue complete."
```

##### `display_summary(ParcelNode *head)`
```
1. Initialize counters: total=0, pending=0, out=0, delivered=0, fast=0, standard=0
2. Traverse linked list, increment counters based on status and type
3. Print summary box:
   ┌─────────────────────────┐
   │   PARCEL SUMMARY        │
   ├─────────────────────────┤
   │ Total:      XX          │
   │ Pending:    XX          │
   │ Out:        XX          │
   │ Delivered:  XX          │
   │ Fast:       XX          │
   │ Standard:   XX          │
   └─────────────────────────┘
```

#### Edge Cases

| Edge Case | Handling |
|-----------|----------|
| Empty linked list (head == NULL) | Print "No parcels in system", return gracefully |
| Very long sender/receiver names | Truncate to column width with `%.20s` format |
| Missing address_id link | Print "N/A" instead of address |
| Rider ID = 0 (unassigned) | Print "Unassigned" |
| Empty time_out field | Print "-" |
| Terminal width too narrow | Use compact column widths, abbreviate headers |

#### Validation Handling
- Check `head != NULL` before any traversal
- Check `parcel != NULL` in `display_parcel_detail()`
- Verify string fields are not empty before printing

#### Linked List Interaction
- **Read-only** — traverses the list using `current = current->next`
- Never calls `malloc`, `free`, or modifies any node data
- Receives `ParcelNode *head` (not `**head`) since it doesn't modify the list

---

### Module: Validation (Aidil)

#### Description
Provides reusable input validation functions used by all other modules.

#### Function Signatures

```c
// validation.h
#ifndef VALIDATION_H
#define VALIDATION_H

int validate_integer(const char *input);
int validate_string_length(const char *input, int min, int max);
int validate_alphanumeric(const char *input);
int validate_status(const char *status);
int validate_delivery_type(const char *type);
void trim_whitespace(char *str);
int safe_read_string(char *buffer, int size);
int safe_read_int(int *result);
void clear_screen(void);

#endif
```

#### Input / Output
| Direction | Data |
|-----------|------|
| **Input** | Raw user input strings |
| **Output** | `1` = valid, `0` = invalid |

#### Dependencies
- None (standalone utility module)

#### Linked List Interaction
- **None** — pure utility functions

---

### Module: Database (Piki)

#### Description
Handles all file I/O — loading data from text files into memory structures and saving them back.

#### Function Signatures

```c
// database.h
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

#endif
```

#### Input / Output
| Direction | Data |
|-----------|------|
| **Input** | File paths, pointer to data structures |
| **Output** | Populated structures, return code (success/fail) |

#### Dependencies
- `parcel_list.h` — for building the linked list

#### Linked List Interaction
- **Write**: `load_parcels_from_file()` calls `insert_parcel()` for each line
- **Read**: `save_parcels_to_file()` traverses list and writes each node

#### How parcels are loaded into the linked list:
```
1. Open file
2. For each line in file:
   a. Parse fields (sscanf or strtok)
   b. Create Parcel struct with parsed data
   c. Call insert_parcel(&head, parcel)  ← malloc happens here
3. Close file
4. Return count of loaded parcels
```

---

### Module: Parcel Sorting (Aiman)

#### Description
Implements the sorting algorithm that generates an optimized delivery queue.

#### Function Signatures

```c
// sorting.h
#ifndef SORTING_H
#define SORTING_H

#include "parcel_list.h"

// Main sorting function — returns new sorted list
ParcelNode* generate_sorted_queue(ParcelNode *head);

// Filter out delivered parcels
ParcelNode* filter_active_parcels(ParcelNode *head);

// Split into Fast and Standard groups
void split_by_delivery_type(ParcelNode *active,
                            ParcelNode **fast_head,
                            ParcelNode **standard_head);

// Sort a sub-list by house number (ascending)
void sort_by_house_number(ParcelNode **head);

// Merge two sorted lists (Fast first, then Standard)
ParcelNode* merge_sorted_lists(ParcelNode *fast, ParcelNode *standard);

// Free the sorted copy list
void free_sorted_queue(ParcelNode **sorted_head);

#endif
```

#### Input / Output
| Direction | Data |
|-----------|------|
| **Input** | `ParcelNode *head` (original list) |
| **Output** | `ParcelNode *` (new sorted copy) |

#### Dependencies
- `parcel_list.h`

#### Linked List Interaction
- **Read** the original list (does NOT modify it)
- **Creates a new linked list** (sorted copy) for display
- The sorted copy must be freed after display (`free_sorted_queue`)

#### Sorting Algorithm
```
1. Traverse original list → copy active parcels (status != "Delivered")
2. Split copied list into two: Fast group, Standard group
3. Sort Fast group by house_number ASC (insertion sort or bubble sort on linked list)
4. Sort Standard group by house_number ASC
5. Concatenate: Fast list → Standard list
6. Return merged head
```

---

### Module: Status Parcel (Kimi)

#### Description
Manages the parcel status lifecycle with valid transition enforcement.

#### Function Signatures

```c
// status.h
#ifndef STATUS_H
#define STATUS_H

#include "parcel_list.h"

// Update status of a parcel by ID
int update_parcel_status(ParcelNode *head, int parcel_id, const char *new_status);

// Check if a status transition is valid
int is_valid_transition(const char *current_status, const char *new_status);

// Display status options
void display_status_options(void);

// Get current timestamp string
void get_current_time(char *buffer, int size);

#endif
```

#### Valid Status Transitions
```
Pending → Out for Delivery → Delivered
```
No skipping steps. No going backwards.

#### Linked List Interaction
- **Read + Update**: Find node by `parcel_id`, modify `status` and `time_out` fields

---

### Module: Address (Kimi)

#### Description
CRUD operations for predefined delivery addresses.

#### Function Signatures

```c
// address.h
#ifndef ADDRESS_H
#define ADDRESS_H

typedef struct {
    int address_id;
    char street[100];
    char city[50];
    char state[50];
    int house_number;
} Address;

int add_address(Address addresses[], int *count, Address new_addr);
int update_address(Address addresses[], int count, int address_id);
void display_address_list(Address addresses[], int count);
Address* find_address(Address addresses[], int count, int address_id);

#endif
```

#### Linked List Interaction
- **Indirect**: Address data is referenced by `address_id` in parcel nodes
- Does not directly modify the parcel linked list

---

### Module: Search Parcel (Aidil)

#### Description
Search and filter parcels by various criteria.

#### Function Signatures

```c
// search.h
#ifndef SEARCH_H
#define SEARCH_H

#include "parcel_list.h"

ParcelNode* search_by_id(ParcelNode *head, int parcel_id);
ParcelNode* search_by_receiver(ParcelNode *head, const char *name);
ParcelNode* search_by_status(ParcelNode *head, const char *status);
ParcelNode* search_by_delivery_type(ParcelNode *head, const char *type);

#endif
```

#### Linked List Interaction
- **Read-only**: Traverses the list, returns pointer to matching node(s)

---

## Phase 3: Integration

### Goals
- Wire all modules into `main.c`
- Test module-to-module data passing
- Ensure linked list is shared correctly

### Integration Checklist

| # | Task | Modules Involved |
|---|------|-----------------|
| 1 | Database loads → linked list populates on startup | database + parcel_list |
| 2 | Login returns role → correct menu displays | login + main |
| 3 | Admin creates parcel → node added to linked list | validation + address + parcel_list |
| 4 | Admin generates sort → sorted list displayed | sorting + output |
| 5 | Rider views parcels → filtered by rider_id | sorting + output |
| 6 | Status update → node modified → file saved | status + parcel_list + database |
| 7 | Search returns results → displayed by output | search + output |
| 8 | Program exit → save to file → free all memory | database + parcel_list |

### Integration in main.c (Skeleton)

```c
int main() {
    // Phase 1: Load data
    ParcelNode *head = NULL;
    User users[MAX_USERS];
    Address addresses[MAX_ADDRESSES];

    int user_count = load_users(users, MAX_USERS, "data/users.txt");
    load_parcels_from_file(&head, "data/parcels.txt");
    int addr_count = load_addresses(addresses, MAX_ADDRESSES, "data/addresses.txt");

    // Phase 2: Login
    int role = login(users, user_count);
    if (role == ROLE_INVALID) {
        free_all_parcels(&head);
        return 1;
    }

    // Phase 3: Menu loop
    if (role == ROLE_ADMIN) {
        show_admin_menu(&head, addresses, &addr_count);
    } else {
        show_rider_menu(&head);
    }

    // Phase 4: Cleanup
    save_parcels_to_file(head, "data/parcels.txt");
    free_all_parcels(&head);

    return 0;
}
```

---

## Phase 4: Testing

### Test Categories

| Category | What to Test |
|----------|-------------|
| **Unit** | Each function in isolation (e.g., `insert_parcel`, `verify_credentials`) |
| **Integration** | Module interactions (e.g., login → menu → create parcel → save) |
| **Edge** | Empty list, max attempts, invalid input, missing files |
| **Memory** | No leaks — every `malloc` has a matching `free` |

### Test Cases for Login (Khai)

| # | Test | Input | Expected Output |
|---|------|-------|----------------|
| 1 | Valid admin login | admin / admin123 | Return `ROLE_ADMIN` |
| 2 | Valid rider login | rider1 / rider123 | Return `ROLE_RIDER` |
| 3 | Wrong password | admin / wrong | "Invalid credentials" |
| 4 | Wrong username | nobody / admin123 | "Invalid credentials" |
| 5 | Empty username | (enter) | "Field cannot be empty" |
| 6 | 3 failed attempts | wrong × 3 | "Too many attempts", return `ROLE_INVALID` |
| 7 | Username with spaces | "  admin  " | Trimmed, login succeeds |
| 8 | Buffer overflow attempt | 100+ char string | Truncated by `fgets`, no crash |

### Test Cases for Output (Khai)

| # | Test | Input | Expected Output |
|---|------|-------|----------------|
| 1 | Empty list | head = NULL | "No parcels in system." |
| 2 | Single parcel | 1 node | Table with 1 row |
| 3 | Multiple parcels | 5 nodes | Table with 5 rows, correct alignment |
| 4 | Sorted display | sorted list | Fast parcels first, then Standard |
| 5 | Summary stats | mixed statuses | Correct count for each status |
| 6 | Rider view | rider_id = 2 | Only shows parcels assigned to rider 2 |
| 7 | Long names | 50-char names | Truncated cleanly in table |
| 8 | Unassigned rider | rider_id = 0 | Shows "Unassigned" |

### Memory Testing

```
Compile with: gcc -Wall -Wextra -g -fsanitize=address main.c ...
```

Check for:
- Memory leaks (allocated but never freed)
- Use-after-free errors
- Double-free errors
- NULL pointer dereferences

---

## Linked List Operations — Deep Dive

### Adding a Parcel (malloc)

```c
void insert_parcel(ParcelNode **head, Parcel new_parcel) {
    // 1. Allocate memory for new node
    ParcelNode *new_node = (ParcelNode *)malloc(sizeof(ParcelNode));

    // 2. Check allocation success
    if (new_node == NULL) {
        printf("Error: Memory allocation failed!\n");
        return;
    }

    // 3. Copy data into node
    new_node->data = new_parcel;

    // 4. Link to existing list (insert at head)
    new_node->next = *head;

    // 5. Update head pointer
    *head = new_node;
}
```

### Removing a Parcel (free)

```c
int delete_parcel(ParcelNode **head, int parcel_id) {
    ParcelNode *current = *head;
    ParcelNode *prev = NULL;

    // Traverse to find the node
    while (current != NULL) {
        if (current->data.parcel_id == parcel_id) {
            // Unlink node
            if (prev == NULL) {
                *head = current->next;  // removing head
            } else {
                prev->next = current->next;  // bypass node
            }
            free(current);  // release memory
            return 1;  // success
        }
        prev = current;
        current = current->next;
    }
    return 0;  // not found
}
```

### Searching a Parcel

```c
ParcelNode* find_parcel(ParcelNode *head, int parcel_id) {
    ParcelNode *current = head;
    while (current != NULL) {
        if (current->data.parcel_id == parcel_id) {
            return current;  // found
        }
        current = current->next;
    }
    return NULL;  // not found
}
```

### Freeing All Parcels (Cleanup)

```c
void free_all_parcels(ParcelNode **head) {
    ParcelNode *current = *head;
    ParcelNode *next_node;

    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
    *head = NULL;  // prevent dangling pointer
}
```

### Memory Management Summary

| Operation | Memory Action | Who Calls |
|-----------|--------------|-----------|
| Insert parcel | `malloc(sizeof(ParcelNode))` | `insert_parcel()`, `load_parcels_from_file()` |
| Delete parcel | `free(node)` | `delete_parcel()` |
| Generate sorted queue | `malloc` (creates copy list) | `generate_sorted_queue()` |
| Free sorted queue | `free` (releases copy list) | `free_sorted_queue()` |
| Program exit | `free` (all remaining nodes) | `free_all_parcels()` |
