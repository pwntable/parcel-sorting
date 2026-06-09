# Output Module

- **Source File**: [output.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/output.c)
- **Header File**: [output.h](file:///Users/mac/Documents/GitHub/parcel-sorting/include/output.h)
- **Module Owner**: Khai
- **Core Responsibility**: Format and display relational data, analytical reports, commission summaries, detailed cards, and barcode visualizations on the console.

---

## Overview
The Output module handles formatting text outputs, reports, and tables. It processes raw relational IDs (address IDs, rider IDs) from the list and maps them to descriptive text (street names, cities, states, usernames) before displaying them to the user. It also contains the commission calculation engine and a pseudo-graphical barcode drawing system.

---

## Functions

### `print_divider`
```c
void print_divider(void);
```
- **Purpose**: Prints a horizontal dashed divider line to align tables.

---

### `print_table_header`
```c
void print_table_header(void);
```
- **Purpose**: Prints the standard headers for parcel listing tables.
- **Header Fields**: ID, Sender, Receiver, Type, Status, House#, Street, City, State, Rider.

---

### `print_table_row`
```c
void print_table_row(Parcel *parcel, Address addresses[], int addr_count, User users[], int user_count);
```
- **Purpose**: Prints a formatted row for a single parcel.
- **Parameters**: Refer to [output.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/output.c#L33-L71).
- **Key Logic**:
  - Resolves address road details from `addresses` by checking `parcel->address_id`.
  - Resolves rider username from `users` by checking `parcel->rider_id`. If `rider_id` is empty, searches for a rider assigned to the parcel's road.

---

### `display_all_parcels`
```c
void display_all_parcels(ParcelNode *head, Address addresses[], int addr_count, User users[], int user_count);
```
- **Purpose**: Displays all parcels in the linked list as a table.
- **Parameters**: Refer to [output.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/output.c#L82-L99).

---

### `display_sorted_parcels`
```c
void display_sorted_parcels(ParcelNode *sorted_head, Address addresses[], int addr_count, User users[], int user_count);
```
- **Purpose**: Displays the sorted active delivery queue in tabular format.

---

### `display_parcels_by_status`
```c
void display_parcels_by_status(ParcelNode *head, const char *status, Address addresses[], int addr_count, User users[], int user_count);
```
- **Purpose**: Displays parcels matching a specific status string.

---

### `display_summary`
```c
void display_summary(ParcelNode *head);
```
- **Purpose**: Displays a summary box with total counts by status (Pending, Out for Delivery, Delivered) and delivery type (Fast, Standard).

---

### `display_admin_report`
```c
void display_admin_report(ParcelNode *head, Address addresses[], int addr_count, User users[], int user_count);
```
- **Purpose**: Prints the system overview report for administrators (statistics summary and full parcel list).

---

### `display_rider_parcels`
```c
void display_rider_parcels(ParcelNode *head, int assigned_address_id, Address addresses[], int addr_count, User users[], int user_count);
```
- **Purpose**: Lists all parcels assigned to a rider's designated road ID.

---

### `display_rider_sorted_parcels`
```c
void display_rider_sorted_parcels(ParcelNode *sorted_head, int assigned_address_id, Address addresses[], int addr_count, User users[], int user_count);
```
- **Purpose**: Displays a sorted queue of active parcels on a rider's road.

---

### `display_active_parcels`
```c
void display_active_parcels(ParcelNode *head, Address addresses[], int addr_count, User users[], int user_count);
```
- **Purpose**: Displays all active (non-delivered) parcels in the system.

---

### `display_rider_active_parcels`
```c
void display_rider_active_parcels(ParcelNode *head, int assigned_address_id, Address addresses[], int addr_count, User users[], int user_count);
```
- **Purpose**: Displays active (non-delivered) parcels assigned to a rider's designated road.

---

### `print_barcode`
```c
void print_barcode(int parcel_id);
```
- **Purpose**: Generates and prints a pseudo-graphical barcode label for a parcel.
- **Parameters**:
  | Parameter | Type | Description |
  |-----------|------|-------------|
  | `parcel_id` | `int` | The parcel ID to serialize into the barcode. |
- **Returns**: `void`
- **Key Logic**:
  - Formats the ID as a 4-digit code (e.g. `P-0001`).
  - Draws guard lines (`█║│`) on the left and right.
  - Draws distinct thick/thin vertical line patterns for each digit of the ID.

---

### `display_rider_commission`
```c
void display_rider_commission(ParcelNode *head, User users[], int user_count, Address addresses[], int addr_count, int rider_idx);
```
- **Purpose**: Displays the commission report for a specific rider.
- **Rates**: Standard Delivery = **RM 3.00**, Fast Delivery = **RM 6.00**.
- **Key Logic**:
  - Filters for "Delivered" parcels belonging to the rider's road or assigned to their user ID.
  - Displays standard and fast delivery counts, sub-payments, and the total earned commission.

---

### `display_all_riders_commission_report`
```c
void display_all_riders_commission_report(ParcelNode *head, User users[], int user_count, Address addresses[], int addr_count);
```
- **Purpose**: Prints a system-wide overview of standard/fast delivery counts and total commissions for all riders.

---

### `display_parcel_detail`
```c
void display_parcel_detail(Parcel *parcel, Address addresses[], int addr_count, User users[], int user_count);
```
- **Purpose**: Displays a detailed card layout of a single parcel, including ID, sender, receiver, house number, type, status, street name, and its printed barcode.
