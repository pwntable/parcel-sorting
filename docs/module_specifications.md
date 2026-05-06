# 📘 Module Specifications

Complete reference for all module functions — parameters, return values, responsibilities, example usage, and linked list interaction.

---

## 1. Linked List Core (`parcel_list.c`)

### `insert_parcel`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `void insert_parcel(ParcelNode **head, Parcel new_parcel)` |
| **Parameters** | `head` — pointer to the head pointer; `new_parcel` — Parcel struct to insert |
| **Returns** | `void` |
| **Responsibility** | Allocates a new node, copies parcel data, inserts at head of list |
| **LL Interaction** | **Write** — allocates new node, updates head |

**Example Usage:**
```c
ParcelNode *head = NULL;
Parcel p;
p.parcel_id = 1;
strcpy(p.sender_name, "Ali");
strcpy(p.receiver_name, "Abu");
p.address_id = 101;
strcpy(p.delivery_type, "Fast");
strcpy(p.status, "Pending");
p.house_number = 12;
strcpy(p.time_in, "2026-05-06 10:00");
strcpy(p.time_out, "");
p.rider_id = 0;

insert_parcel(&head, p);
// head now points to the new node
```

---

### `delete_parcel`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `int delete_parcel(ParcelNode **head, int parcel_id)` |
| **Parameters** | `head` — pointer to head pointer; `parcel_id` — ID to remove |
| **Returns** | `1` on success, `0` if not found |
| **Responsibility** | Finds node by ID, unlinks it, frees memory |
| **LL Interaction** | **Write** — removes node, frees memory |

**Example Usage:**
```c
int result = delete_parcel(&head, 3);
if (result) {
    printf("Parcel 3 deleted.\n");
} else {
    printf("Parcel 3 not found.\n");
}
```

---

### `find_parcel`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `ParcelNode* find_parcel(ParcelNode *head, int parcel_id)` |
| **Parameters** | `head` — list head; `parcel_id` — ID to search |
| **Returns** | Pointer to matching node, or `NULL` if not found |
| **Responsibility** | Traverses list, returns first match |
| **LL Interaction** | **Read** — traversal only |

**Example Usage:**
```c
ParcelNode *result = find_parcel(head, 5);
if (result != NULL) {
    printf("Found: %s\n", result->data.receiver_name);
}
```

---

### `free_all_parcels`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `void free_all_parcels(ParcelNode **head)` |
| **Parameters** | `head` — pointer to head pointer |
| **Returns** | `void` |
| **Responsibility** | Frees every node in the list, sets head to NULL |
| **LL Interaction** | **Write** — destroys entire list |

**Example Usage:**
```c
free_all_parcels(&head);
// head is now NULL, all memory freed
```

---

### `count_parcels`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `int count_parcels(ParcelNode *head)` |
| **Parameters** | `head` — list head |
| **Returns** | Number of nodes in the list |
| **Responsibility** | Traverses and counts all nodes |
| **LL Interaction** | **Read** — traversal only |

**Example Usage:**
```c
int total = count_parcels(head);
printf("Total parcels: %d\n", total);
```

---

## 2. Login Module (`login.c`) — Khai ⭐

### `login`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `int login(User users[], int user_count)` |
| **Parameters** | `users[]` — array of User structs; `user_count` — number of users |
| **Returns** | `ROLE_ADMIN (0)`, `ROLE_RIDER (1)`, or `ROLE_INVALID (-1)` |
| **Responsibility** | Main login loop — prompts for credentials, validates, authenticates |
| **LL Interaction** | **None** |

**Example Usage:**
```c
User users[10];
int count = load_users(users, 10, "data/users.txt");
int role = login(users, count);

if (role == ROLE_ADMIN) {
    printf("Welcome, Admin!\n");
} else if (role == ROLE_RIDER) {
    printf("Welcome, Rider!\n");
} else {
    printf("Access denied.\n");
}
```

---

### `display_login_prompt`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `void display_login_prompt(char *username, char *password)` |
| **Parameters** | `username` — buffer to store input; `password` — buffer to store input |
| **Returns** | `void` (fills buffers via pointer) |
| **Responsibility** | Displays prompt text, reads username and password from stdin |
| **LL Interaction** | **None** |

**Example Usage:**
```c
char username[30], password[30];
display_login_prompt(username, password);
printf("You entered: %s\n", username);
```

---

### `verify_credentials`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `int verify_credentials(User users[], int user_count, const char *username, const char *password)` |
| **Parameters** | `users[]` — user array; `user_count`; `username`; `password` |
| **Returns** | User's role if match found, `ROLE_INVALID` if no match |
| **Responsibility** | Loops through users, compares credentials |
| **LL Interaction** | **None** |

**Example Usage:**
```c
int role = verify_credentials(users, count, "admin", "admin123");
// Returns ROLE_ADMIN if valid
```

---

## 3. Output Module (`output.c`) — Khai ⭐

### `display_all_parcels`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `void display_all_parcels(ParcelNode *head)` |
| **Parameters** | `head` — linked list head |
| **Returns** | `void` |
| **Responsibility** | Prints all parcels in a formatted table |
| **LL Interaction** | **Read** — traverses entire list |

**Example Usage:**
```c
display_all_parcels(head);
// Output:
// ID | Sender     | Receiver   | Type     | Status  | House# | Rider
// ---|------------|------------|----------|---------|--------|------
//  1 | Ali        | Abu        | Fast     | Pending |     12 |     1
//  2 | Siti       | Ahmad      | Standard | Pending |      5 |     0
```

---

### `display_sorted_parcels`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `void display_sorted_parcels(ParcelNode *sorted_head)` |
| **Parameters** | `sorted_head` — sorted linked list (from sorting module) |
| **Returns** | `void` |
| **Responsibility** | Displays sorted delivery queue with group headers |
| **LL Interaction** | **Read** — traverses sorted copy |

**Example Usage:**
```c
ParcelNode *sorted = generate_sorted_queue(head);
display_sorted_parcels(sorted);
free_sorted_queue(&sorted);

// Output:
// === SORTED DELIVERY QUEUE ===
// --- FAST DELIVERY ---
// 1. [ID:3] Ahmad → House #2 (Pending)
// 2. [ID:1] Abu   → House #12 (Pending)
// --- STANDARD DELIVERY ---
// 3. [ID:2] Siti  → House #5 (Pending)
```

---

### `display_parcels_by_status`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `void display_parcels_by_status(ParcelNode *head, const char *status)` |
| **Parameters** | `head` — list head; `status` — filter string (e.g., "Pending") |
| **Returns** | `void` |
| **Responsibility** | Displays only parcels matching the given status |
| **LL Interaction** | **Read** — traverses with filter condition |

**Example Usage:**
```c
display_parcels_by_status(head, "Pending");
// Shows only parcels with status == "Pending"
```

---

### `display_parcel_detail`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `void display_parcel_detail(Parcel *parcel)` |
| **Parameters** | `parcel` — pointer to a single Parcel struct |
| **Returns** | `void` |
| **Responsibility** | Prints all fields of a single parcel in a detailed view |
| **LL Interaction** | **Read** — reads single node data |

**Example Usage:**
```c
ParcelNode *found = find_parcel(head, 1);
if (found) {
    display_parcel_detail(&found->data);
}
// Output:
// ┌─────────────────────────┐
// │ Parcel ID:     1        │
// │ Sender:        Ali      │
// │ Receiver:      Abu      │
// │ Type:          Fast     │
// │ Status:        Pending  │
// │ House #:       12       │
// │ Rider:         1        │
// │ Time In:       10:00    │
// │ Time Out:      -        │
// └─────────────────────────┘
```

---

### `display_summary`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `void display_summary(ParcelNode *head)` |
| **Parameters** | `head` — list head |
| **Returns** | `void` |
| **Responsibility** | Counts parcels by status and type, displays summary box |
| **LL Interaction** | **Read** — traverses and aggregates |

**Example Usage:**
```c
display_summary(head);
// Output:
// ┌─────────────────────────┐
// │   PARCEL SUMMARY        │
// │ Total:         10       │
// │ Pending:        4       │
// │ Out:            3       │
// │ Delivered:      3       │
// │ Fast:           6       │
// │ Standard:       4       │
// └─────────────────────────┘
```

---

### `display_admin_report`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `void display_admin_report(ParcelNode *head)` |
| **Parameters** | `head` — list head |
| **Returns** | `void` |
| **Responsibility** | Shows full parcel table + summary (Admin view) |
| **LL Interaction** | **Read** — full traversal |

---

### `display_rider_parcels`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `void display_rider_parcels(ParcelNode *head, int rider_id)` |
| **Parameters** | `head` — list head; `rider_id` — rider to filter by |
| **Returns** | `void` |
| **Responsibility** | Shows only parcels assigned to a specific rider |
| **LL Interaction** | **Read** — traverses with rider_id filter |

**Example Usage:**
```c
display_rider_parcels(head, 2);
// Shows only parcels where rider_id == 2
```

---

### `print_table_header` / `print_table_row` / `print_divider`

| Function | Signature | Responsibility |
|----------|-----------|---------------|
| `print_table_header` | `void print_table_header(void)` | Prints column headers |
| `print_table_row` | `void print_table_row(Parcel *parcel)` | Prints one formatted row |
| `print_divider` | `void print_divider(void)` | Prints `---...---` line |

These are **internal helper functions** used by the display functions above.

---

## 4. Validation Module (`validation.c`) — Aidil

### `validate_integer`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `int validate_integer(const char *input)` |
| **Parameters** | `input` — string to check |
| **Returns** | `1` if valid integer, `0` otherwise |
| **LL Interaction** | **None** |

### `validate_string_length`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `int validate_string_length(const char *input, int min, int max)` |
| **Parameters** | `input`; `min` — minimum length; `max` — maximum length |
| **Returns** | `1` if length is within range, `0` otherwise |
| **LL Interaction** | **None** |

### `validate_alphanumeric`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `int validate_alphanumeric(const char *input)` |
| **Parameters** | `input` — string to check |
| **Returns** | `1` if all chars are alphanumeric, `0` otherwise |
| **LL Interaction** | **None** |

### `validate_status`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `int validate_status(const char *status)` |
| **Parameters** | `status` — status string |
| **Returns** | `1` if valid status ("Pending", "Out for Delivery", "Delivered"), `0` otherwise |
| **LL Interaction** | **None** |

### `validate_delivery_type`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `int validate_delivery_type(const char *type)` |
| **Parameters** | `type` — delivery type string |
| **Returns** | `1` if "Fast" or "Standard", `0` otherwise |
| **LL Interaction** | **None** |

### `trim_whitespace`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `void trim_whitespace(char *str)` |
| **Parameters** | `str` — string to trim (modified in-place) |
| **Returns** | `void` |
| **LL Interaction** | **None** |

### `safe_read_string`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `int safe_read_string(char *buffer, int size)` |
| **Parameters** | `buffer` — destination; `size` — max chars |
| **Returns** | `1` on success, `0` on failure |
| **LL Interaction** | **None** |

### `safe_read_int`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `int safe_read_int(int *result)` |
| **Parameters** | `result` — pointer to store the integer |
| **Returns** | `1` if valid integer read, `0` otherwise |
| **LL Interaction** | **None** |

---

## 5. Database Module (`database.c`) — Piki

### `load_parcels_from_file`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `int load_parcels_from_file(ParcelNode **head, const char *filename)` |
| **Parameters** | `head` — pointer to head pointer; `filename` — path to data file |
| **Returns** | Number of parcels loaded, `-1` on file error |
| **Responsibility** | Reads file line by line, parses into Parcel structs, inserts into linked list |
| **LL Interaction** | **Write** — calls `insert_parcel()` for each record |

### `save_parcels_to_file`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `int save_parcels_to_file(ParcelNode *head, const char *filename)` |
| **Parameters** | `head` — list head; `filename` — output path |
| **Returns** | Number of parcels saved, `-1` on file error |
| **Responsibility** | Traverses linked list, writes each node to file |
| **LL Interaction** | **Read** — traverses list |

### `load_users`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `int load_users(User users[], int max, const char *filename)` |
| **Parameters** | `users[]` — array to fill; `max` — array capacity; `filename` |
| **Returns** | Number of users loaded |
| **LL Interaction** | **None** — uses array |

### `load_addresses`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `int load_addresses(Address addresses[], int max, const char *filename)` |
| **Parameters** | `addresses[]` — array to fill; `max`; `filename` |
| **Returns** | Number of addresses loaded |
| **LL Interaction** | **None** — uses array |

### `get_next_parcel_id`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `int get_next_parcel_id(ParcelNode *head)` |
| **Parameters** | `head` — list head |
| **Returns** | Highest existing ID + 1 |
| **Responsibility** | Traverses list to find max ID |
| **LL Interaction** | **Read** — traversal |

---

## 6. Sorting Module (`sorting.c`) — Aiman

### `generate_sorted_queue`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `ParcelNode* generate_sorted_queue(ParcelNode *head)` |
| **Parameters** | `head` — original list head |
| **Returns** | Head of a **new** sorted linked list (copy) |
| **Responsibility** | Full sorting pipeline: filter → split → sort → merge |
| **LL Interaction** | **Read** original, **Write** new sorted copy |

### `filter_active_parcels`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `ParcelNode* filter_active_parcels(ParcelNode *head)` |
| **Parameters** | `head` — original list |
| **Returns** | New list with only non-delivered parcels |
| **LL Interaction** | **Read** original, **Write** filtered copy |

### `split_by_delivery_type`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `void split_by_delivery_type(ParcelNode *active, ParcelNode **fast_head, ParcelNode **standard_head)` |
| **Parameters** | `active` — filtered list; `fast_head`, `standard_head` — output sub-lists |
| **Returns** | `void` (fills fast_head and standard_head) |
| **LL Interaction** | **Read** active list, **Write** two sub-lists |

### `sort_by_house_number`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `void sort_by_house_number(ParcelNode **head)` |
| **Parameters** | `head` — pointer to sub-list head |
| **Returns** | `void` (list is sorted in-place) |
| **LL Interaction** | **Write** — reorders nodes |

### `merge_sorted_lists`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `ParcelNode* merge_sorted_lists(ParcelNode *fast, ParcelNode *standard)` |
| **Parameters** | `fast` — sorted fast list; `standard` — sorted standard list |
| **Returns** | Head of merged list (fast → standard) |
| **LL Interaction** | **Write** — links last node of fast to head of standard |

### `free_sorted_queue`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `void free_sorted_queue(ParcelNode **sorted_head)` |
| **Parameters** | `sorted_head` — pointer to sorted list head |
| **Returns** | `void` |
| **Responsibility** | Frees the sorted copy (NOT the original list) |
| **LL Interaction** | **Write** — frees all copied nodes |

---

## 7. Status Module (`status.c`) — Kimi

### `update_parcel_status`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `int update_parcel_status(ParcelNode *head, int parcel_id, const char *new_status)` |
| **Parameters** | `head`; `parcel_id`; `new_status` |
| **Returns** | `1` on success, `0` if not found, `-1` if invalid transition |
| **LL Interaction** | **Read + Update** — finds node, modifies status field |

### `is_valid_transition`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `int is_valid_transition(const char *current_status, const char *new_status)` |
| **Parameters** | Current and desired status strings |
| **Returns** | `1` if valid, `0` if not |
| **LL Interaction** | **None** — pure logic |

### `display_status_options`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `void display_status_options(void)` |
| **Returns** | `void` |
| **Responsibility** | Prints available status options to terminal |
| **LL Interaction** | **None** |

### `get_current_time`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `void get_current_time(char *buffer, int size)` |
| **Parameters** | `buffer` — output string; `size` — buffer capacity |
| **Returns** | `void` (fills buffer) |
| **LL Interaction** | **None** |

---

## 8. Address Module (`address.c`) — Kimi

### `add_address`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `int add_address(Address addresses[], int *count, Address new_addr)` |
| **Returns** | `1` on success, `0` if array full |
| **LL Interaction** | **None** — uses array |

### `update_address`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `int update_address(Address addresses[], int count, int address_id)` |
| **Returns** | `1` on success, `0` if not found |
| **LL Interaction** | **Indirect** — parcels reference address_id |

### `display_address_list`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `void display_address_list(Address addresses[], int count)` |
| **Returns** | `void` |
| **LL Interaction** | **None** |

### `find_address`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `Address* find_address(Address addresses[], int count, int address_id)` |
| **Returns** | Pointer to matching Address, or `NULL` |
| **LL Interaction** | **None** |

---

## 9. Search Module (`search.c`) — Aidil

### `search_by_id`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `ParcelNode* search_by_id(ParcelNode *head, int parcel_id)` |
| **Returns** | Pointer to matching node, or `NULL` |
| **LL Interaction** | **Read** — traversal |

### `search_by_receiver`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `ParcelNode* search_by_receiver(ParcelNode *head, const char *name)` |
| **Returns** | Pointer to first matching node, or `NULL` |
| **LL Interaction** | **Read** — traversal with string comparison |

### `search_by_status`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `ParcelNode* search_by_status(ParcelNode *head, const char *status)` |
| **Returns** | Pointer to first matching node, or `NULL` |
| **LL Interaction** | **Read** — traversal |

### `search_by_delivery_type`

| Attribute | Detail |
|-----------|--------|
| **Signature** | `ParcelNode* search_by_delivery_type(ParcelNode *head, const char *type)` |
| **Returns** | Pointer to first matching node, or `NULL` |
| **LL Interaction** | **Read** — traversal |
