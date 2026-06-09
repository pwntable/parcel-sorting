# Parcel Sorting Module

- **Source File**: [sorting.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/sorting.c)
- **Header File**: [sorting.h](file:///Users/mac/Documents/GitHub/parcel-sorting/include/sorting.h)
- **Module Owner**: Aiman
- **Core Responsibility**: Sort parcels by delivery status, delivery type, and house number.

---

## Overview
The Sorting module contains the implementation of the system's sorting logic. To generate the sorted delivery queue, it filters out delivered parcels, splits the active parcels into categories by delivery type ("Fast" and "Standard"), sorts each sub-list in ascending order of house numbers using a bubble sort algorithm, and merges the lists back together.

---

## Functions

### `filter_active_parcels`
```c
ParcelNode* filter_active_parcels(ParcelNode *head);
```
- **Purpose**: Creates a copy of the list containing only active (non-delivered) parcels.
- **Parameters**: Refer to [sorting.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/sorting.c#L10-L22).
- **Returns**: `ParcelNode*` — Head pointer to the copy list.
- **Key Logic**:
  - Traverses the original list.
  - Checks if the parcel status is not equal to `"Delivered"`.
  - Inserts matching records into the new active list using `insert_parcel`.

---

### `split_by_delivery_type`
```c
void split_by_delivery_type(ParcelNode *active, ParcelNode **fast_head, ParcelNode **standard_head);
```
- **Purpose**: Splits active parcels into two sub-lists based on their delivery type.
- **Parameters**: Refer to [sorting.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/sorting.c#L31-L41).
- **Key Logic**:
  - Traverses the active list.
  - Places "Fast" delivery type parcels into the `fast_head` list.
  - Places "Standard" delivery type parcels into the `standard_head` list.

---

### `sort_by_house_number`
```c
void sort_by_house_number(ParcelNode **head);
```
- **Purpose**: Sorts a linked list in-place in ascending order of house numbers.
- **Parameters**: Refer to [sorting.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/sorting.c#L50-L73).
- **Key Logic**:
  - Uses a bubble sort algorithm.
  - Compares the `house_number` of adjacent nodes.
  - Swaps the `Parcel` data structs within the nodes when out of order.

---

### `merge_sorted_lists`
```c
ParcelNode* merge_sorted_lists(ParcelNode *fast, ParcelNode *standard);
```
- **Purpose**: Appends standard parcels behind fast parcels to form a single queue.
- **Parameters**: Refer to [sorting.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/sorting.c#L82-L93).
- **Returns**: `ParcelNode*` — Head pointer to the merged list.
- **Key Logic**:
  - Finds the last node of the `fast` list.
  - Links its `next` pointer to the head of the `standard` list.

---

### `generate_sorted_queue`
```c
ParcelNode* generate_sorted_queue(ParcelNode *head);
```
- **Purpose**: Generates the complete sorted delivery queue.
- **Parameters**: Refer to [sorting.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/sorting.c#L107-L119).
- **Returns**: `ParcelNode*` — Head pointer to the sorted queue.
- **Pipeline Workflow**:
  ```
  [Input List] -> filter_active_parcels() -> [Active List]
                       |
                       v
             split_by_delivery_type() -> [Fast Sub-list] & [Standard Sub-list]
                       |
                       v
             sort_by_house_number() -> (Sorts both sub-lists in ascending order of house numbers)
                       |
                       v
             merge_sorted_lists() -> [Final Sorted Queue]
  ```

---

### `free_sorted_queue`
```c
void free_sorted_queue(ParcelNode **sorted_head);
```
- **Purpose**: Safely frees the copy list structures created during sorting.
