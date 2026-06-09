# Address Module

- **Source File**: [address.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/address.c)
- **Header File**: [address.h](file:///Users/mac/Documents/GitHub/parcel-sorting/include/address.h)
- **Module Owner**: Kimi
- **Core Responsibility**: CRUD operations for delivery addresses and roads.

---

## Overview
The Address module manages the delivery destination master lists. It registers streets (roads), cities, and states into the system. It enables administrators to create, read, and update addresses. It also provides search helper functions to retrieve address records by their unique IDs.

---

## Data Structures

### `Address`
A structure representing a physical street location or road node within the sorting database.
```c
typedef struct {
    int address_id;       // Primary Key (Unique Address ID)
    char street[100];     // Name of the street / road (e.g. "Jalan Mawar")
    char city[50];        // City (e.g. "Batu Pahat")
    char state[50];       // State (e.g. "Johor")
    int house_number;     // House number associated with parcels (note: stored in address struct but mainly used inside parcel mappings)
} Address;
```

---

## Functions

### `add_address`
```c
int add_address(Address addresses[], int *count, Address new_addr);
```
- **Purpose**: Appends a new delivery address to the system database.
- **Parameters**:
  | Parameter | Type | Description |
  |-----------|------|-------------|
  | `addresses[]` | `Address` | Array of all registered addresses. |
  | `count` | `int*` | Pointer to the integer tracking total addresses. |
  | `new_addr` | `Address` | Struct populated with the street, city, and state details. |
- **Returns**: `int` — `1` if the address was added successfully, `0` if the maximum array limit (`50`) is reached.
- **Key Logic**:
  - Automatically verifies if an `address_id` is provided. If `new_addr.address_id == 0`, it computes the next available unique ID (`max_id + 1`) and assigns it.
  - Appends the struct to the end of the array and increments the address count.

---

### `find_address`
```c
Address* find_address(Address addresses[], int count, int address_id);
```
- **Purpose**: Searches the address database by a unique address ID.
- **Parameters**:
  | Parameter | Type | Description |
  |-----------|------|-------------|
  | `addresses[]` | `Address` | Array of registered addresses. |
  | `count` | `int` | Total number of addresses in the system. |
  | `address_id` | `int` | Unique address ID to find. |
- **Returns**: `Address*` — Pointer to the matching `Address` structure inside the array, or `NULL` if not found.
- **Usage**: Used by multiple modules (Login, Output, main menus) to translate address IDs into full street descriptions.

---

### `update_address`
```c
int update_address(Address addresses[], int count, int address_id);
```
- **Purpose**: Interactively updates the fields (street, city, state) of a registered address.
- **Parameters**:
  | Parameter | Type | Description |
  |-----------|------|-------------|
  | `addresses[]` | `Address` | Array of registered addresses. |
  | `count` | `int` | Total number of addresses. |
  | `address_id` | `int` | Unique address ID of the target to update. |
- **Returns**: `int` — `1` if successfully updated, `0` if target address ID not found.
- **Key Logic**:
  - Finds the address using `find_address`.
  - Prompts the user for a new street name, city, and state.
  - Enforces a **Double Escape Hatch / Preserve Field** rule: if the user enters nothing (empty string by pressing Enter), the function leaves the existing value unchanged.

---

### `display_address_list`
```c
void display_address_list(Address addresses[], int count);
```
- **Purpose**: Prints a simple tabular list of all registered addresses to the screen.
- **Parameters**:
  | Parameter | Type | Description |
  |-----------|------|-------------|
  | `addresses[]` | `Address` | Array of registered addresses. |
  | `count` | `int` | Current address count. |
- **Returns**: `void`
- **Output Columns**: Address ID, Street Name, City, and State.
