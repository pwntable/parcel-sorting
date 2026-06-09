# Main Application Module

- **Source File**: [main.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/main.c)
- **Header File**: None (Executable entry point)
- **Module Owner**: System / Shared
- **Core Responsibility**: Orchestrate and coordinate the main console interface menus, route actions based on authenticated roles, and manage application startup and shutdown.

---

## Overview
The Main Application module is the central controller of the parcel sorting system. It handles startup initialization (loading CSV tables from datasets), coordinates user login security prompts, and routes authenticated users into either the **Admin Main Menu** or the **Rider Menu**. It manages interactive sub-menus for users, parcels, addresses, and reports.

---

## Functions

### `main`
```c
int main();
```
- **Purpose**: System execution entry point.
- **Returns**: `int` — `0` on successful exit, `1` on error.
- **Key Logic**:
  - Initializes database records by calling `load_users`, `load_addresses`, and `load_parcels_from_file`.
  - Terminates with an error code if the CSV databases are empty or missing.
  - Enters a loop prompting for user credentials via `login`.
  - Resolves the authenticated user's role:
    - If **Admin** (`ROLE_ADMIN`): routes to `show_admin_main_menu`.
    - If **Rider** (`ROLE_RIDER`): routes to `show_rider_menu`.
  - On exit (user logs out or terminates), saves all records back to `users.csv`, `addresses.csv`, and `parcels.csv`.
  - Frees all dynamic memory in the parcel list using `free_all_parcels`.

---

### `show_admin_main_menu`
```c
void show_admin_main_menu(ParcelNode **head, User users[], int *user_count, Address addresses[], int *addr_count);
```
- **Purpose**: Displays the main console dashboard for system administrators.
- **Parameters**: Refer to [main.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/main.c#L700-L731).
- **Sub-Menu Options**:
  1. **Users Management** (calls `show_user_menu`)
  2. **Parcel Management** (calls `show_parcel_menu`)
  3. **Address Management** (calls `show_address_menu`)
  4. **Reports & Analytics** (calls `show_report_menu`)
  5. **Logout** (returns to login screen)

---

### `show_user_menu`
```c
void show_user_menu(User users[], int *user_count, Address addresses[], int addr_count);
```
- **Purpose**: Coordinates administrative operations for user administration.
- **Key Administrative Rules**:
  - **Registration**: Allows creating new riders. Requires selecting a valid, unregistered address from the active addresses list. Enforces unique road assignments.
  - **Updating**: Updates passwords, usernames, and roles. Transitioning a user's role to Admin resets their road ID to `0`. Transitioning a user's role to Rider forces a new road assignment.
  - **Deletion**: Prevents deleting administrator accounts. Moves subsequent elements leftwards.

---

### `show_parcel_menu`
```c
void show_parcel_menu(ParcelNode **head, Address addresses[], int addr_count, User users[], int user_count);
```
- **Purpose**: Coordinates parcel sorting and shipping.
- **Key Operations**:
  - **View**: Displays a priority-sorted table of all parcels.
  - **Creation**: Prompts for sender, receiver, house number, and delivery type ("Fast" or "Standard"). ID is generated automatically. Status defaults to "Pending". Prints a graphical barcode.
  - **Search**: Routes to search by ID or receiver.
  - **Update Status**: Changes parcel status through permitted state transitions. If status transitions to "Delivered", auto-resolves and stamps the `rider_id` matching the rider assigned to the parcel's road.
  - **Delete**: Permanently removes a parcel node from the linked list.

---

### `show_address_menu`
```c
void show_address_menu(Address addresses[], int *addr_count, User users[], int user_count);
```
- **Purpose**: Coordinates address directories.
- **Key Operations**:
  - **View**: Lists all streets/roads with their assigned riders.
  - **Add**: Creates a new street record (validated street, city, state).
  - **Update**: Interactively modifies details of an address.

---

### `show_report_menu`
```c
void show_report_menu(ParcelNode *head, Address addresses[], int addr_count, User users[], int user_count);
```
- **Purpose**: Displays system reports.
- **Sub-Menu Options**:
  1. **Admin Summary Report**: Prints counts (Pending, Delivered, Out for Delivery) and lists all parcels.
  2. **View Delivery Queue**: Sorts and lists active parcels by priority.
  3. **Rider Commission Report**: Prints standard/fast delivery totals and commissions for all riders.

---

### `show_rider_menu`
```c
void show_rider_menu(ParcelNode **head, int user_idx, User users[], int user_count, Address addresses[], int addr_count);
```
- **Purpose**: Coordinates rider workflows.
- **Key Features**:
  - **View Assigned Parcels**: Displays active parcels assigned to their designated road.
  - **View Sorted Queue**: Displays sorted active parcels assigned to their road.
  - **Update Status**: Allows the rider to mark their road's parcels as "Out for Delivery" or "Delivered". Blocks riders from modifying parcels on roads assigned to other riders. Stamps the rider's ID upon successful delivery.
  - **Commission Report**: Displays a detailed commission report for the rider.
