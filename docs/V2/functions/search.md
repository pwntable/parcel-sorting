# Search Module

- **Source File**: [search.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/search.c)
- **Header File**: [search.h](file:///Users/mac/Documents/GitHub/parcel-sorting/include/search.h)
- **Module Owner**: Aidil
- **Core Responsibility**: Provide search operations for filtering and locating parcels.

---

## Overview
The Search module provides operations to search for and filter parcels by ID, receiver name, status, and delivery type. It includes case-insensitive matching logic, allowing matching regardless of text capitalization.

---

## Functions

### `custom_strcasestr`
```c
static char* custom_strcasestr(const char *haystack, const char *needle);
```
- **Purpose**: Helper function that performs a case-insensitive substring search.
- **Parameters**:
  | Parameter | Type | Description |
  |-----------|------|-------------|
  | `haystack` | `const char*` | The text string to search within. |
  | `needle` | `const char*` | The substring to look for. |
- **Returns**: `char*` — Pointer to the beginning of the located substring in the haystack, or `NULL` if not found.
- **Key Logic**:
  - Useful for platforms that do not support the non-standard `strcasestr` function.
  - Converts characters to lowercase using `tolower` while matching strings.

---

### `search_by_id`
```c
ParcelNode* search_by_id(ParcelNode *head, int parcel_id);
```
- **Purpose**: Locates a parcel by its ID.
- **Parameters**: Refer to [search.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/search.c#L33-L35).
- **Returns**: `ParcelNode*` — Pointer to the matching `ParcelNode`, or `NULL` if not found.
- **Key Logic**: Direct wrapper around `find_parcel`.

---

### `search_by_receiver`
```c
ParcelNode* search_by_receiver(ParcelNode *head, const char *name);
```
- **Purpose**: Locates a parcel by receiver name using a case-insensitive substring match.
- **Parameters**: Refer to [search.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/search.c#L44-L53).
- **Returns**: `ParcelNode*` — Pointer to the first matching node, or `NULL` if not found.
- **Key Logic**:
  - Traverses the linked list.
  - Uses `custom_strcasestr` to compare the `receiver_name` field.
  - Returns the first node where the substring match succeeds.

---

### `search_by_status`
```c
ParcelNode* search_by_status(ParcelNode *head, const char *status);
```
- **Purpose**: Locates the first parcel matching a specific status string.
- **Parameters**: Refer to [search.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/search.c#L62-L71).
- **Returns**: `ParcelNode*` — Pointer to the first matching node, or `NULL` if not found.
- **Key Logic**: Uses `strcasecmp` to compare status strings.

---

### `search_by_delivery_type`
```c
ParcelNode* search_by_delivery_type(ParcelNode *head, const char *type);
```
- **Purpose**: Locates the first parcel matching a specific delivery type.
- **Parameters**: Refer to [search.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/search.c#L80-L89).
- **Returns**: `ParcelNode*` — Pointer to the first matching node, or `NULL` if not found.
- **Key Logic**: Uses `strcasecmp` to compare delivery type strings ("Fast" or "Standard").
