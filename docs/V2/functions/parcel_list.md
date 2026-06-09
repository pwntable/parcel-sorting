# Parcel Linked List Module

- **Source File**: [parcel_list.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/parcel_list.c)
- **Header File**: [parcel_list.h](file:///Users/mac/Documents/GitHub/parcel-sorting/include/parcel_list.h)
- **Module Owner**: Khai
- **Core Responsibility**: Centralized linked list data structures and operations for managing parcel records.

---

## Overview
The Parcel Linked List module contains the primary data structures used to store parcel records in RAM. It implements a singly linked list and provides functions for insertion, deletion, lookup, and memory cleanup. It also defines the priority comparison rules used to keep parcels sorted.

---

## Data Structures

### `Parcel`
Represents a parcel record.
```c
typedef struct {
    int parcel_id;            // Primary Key (Unique Parcel ID)
    char sender_name[50];     // Sender name
    char receiver_name[50];   // Receiver name
    int address_id;           // Target road ID (Foreign Key mapping to Address)
    char delivery_type[10];   // "Fast" or "Standard"
    char status[20];          // "Pending", "Out for Delivery", or "Delivered"
    int house_number;         // Destination house number
    char time_in[20];         // Timestamp: Creation time ("YYYY-MM-DD HH:MM")
    char time_out[20];        // Timestamp: Delivery completion time ("YYYY-MM-DD HH:MM")
    int rider_id;             // Assigned Rider ID (Foreign Key mapping to User)
} Parcel;
```

### `ParcelNode`
A node structure in the singly linked list.
```c
typedef struct ParcelNode {
    Parcel data;             // The Parcel record details
    struct ParcelNode *next; // Pointer to the next node in the list
} ParcelNode;
```

---

## Functions

### `compare_parcels`
```c
int compare_parcels(Parcel p1, Parcel p2);
```
- **Purpose**: Compares two parcels to determine their priority order.
- **Parameters**: Refer to [parcel_list.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/parcel_list.c#L19-L38).
- **Returns**: `int` — A negative value if `p1` should come before `p2`, a positive value if `p1` should come after `p2`, or `0` if their priority is equal.
- **Priority Rules**:
  1. **Status**: Non-delivered parcels are always prioritized ahead of Delivered parcels.
  2. **Delivery Type (Active)**: For non-delivered parcels, "Fast" delivery type is prioritized over "Standard".
  3. **House Number (Active)**: If status and type match, sorted by house number ascending.
  4. **Parcel ID (Delivered)**: Delivered parcels are sorted by `parcel_id` ascending (delivery history).

---

### `insert_parcel`
```c
void insert_parcel(ParcelNode **head, Parcel new_parcel);
```
- **Purpose**: Inserts a new parcel into the linked list in the correct priority position.
- **Parameters**: Refer to [parcel_list.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/parcel_list.c#L48-L72).
- **Key Logic**:
  - Dynamically allocates a new node with `malloc`.
  - Compares priority using `compare_parcels`.
  - Inserts the node at the head or inserts it between nodes to maintain sorted order.

---

### `sort_parcel_list`
```c
void sort_parcel_list(ParcelNode **head);
```
- **Purpose**: Re-sorts the entire linked list.
- **Parameters**: Refer to [parcel_list.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/parcel_list.c#L82-L95).
- **Key Logic**:
  - Useful when a parcel's status transitions (e.g. from "Pending" to "Delivered"), which changes its priority order.
  - Re-inserts all nodes into a new linked list.

---

### `delete_parcel`
```c
int delete_parcel(ParcelNode **head, int parcel_id);
```
- **Purpose**: Deletes a parcel from the linked list by its ID.
- **Parameters**: Refer to [parcel_list.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/parcel_list.c#L106-L124).
- **Returns**: `int` — `1` on success, `0` if the parcel was not found.
- **Key Logic**:
  - Traverses the list tracking the current and previous nodes.
  - Updates pointers to bypass the target node.
  - Frees the target node's memory.

---

### `find_parcel`
```c
ParcelNode* find_parcel(ParcelNode *head, int parcel_id);
```
- **Purpose**: Searches the linked list for a parcel node with a matching ID.
- **Parameters**: Refer to [parcel_list.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/parcel_list.c#L133-L142).
- **Returns**: `ParcelNode*` — Pointer to the matching node, or `NULL` if not found.

---

### `free_all_parcels`
```c
void free_all_parcels(ParcelNode **head);
```
- **Purpose**: Frees all nodes in the linked list to prevent memory leaks and sets the head pointer to `NULL`.
- **Parameters**: Refer to [parcel_list.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/parcel_list.c#L149-L158).

---

### `count_parcels`
```c
int count_parcels(ParcelNode *head);
```
- **Purpose**: Counts and returns the total number of nodes in the linked list.
