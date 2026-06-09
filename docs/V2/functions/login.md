# Login Module

- **Source File**: [login.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/login.c)
- **Header File**: [login.h](file:///Users/mac/Documents/GitHub/parcel-sorting/include/login.h)
- **Module Owner**: Khai
- **Core Responsibility**: Authenticate users, manage riders, and enforce unique road assignments.

---

## Overview
The Login module handles secure access to the system. It defines user roles (Admin and Rider) and manages credentials. In addition to user authentication, it includes administrative functions for CRUD operations on user accounts, ensuring that every rider is assigned a unique road and that duplicates are strictly prevented.

---

## Data Structures

### `User`
A structure representing a system user (either an administrator or delivery rider).
```c
typedef struct {
    int user_id;             // Primary Key (Unique User ID)
    char username[30];       // Unique username
    char password[30];       // Password for authentication
    int role;                // Role: ROLE_ADMIN (0) or ROLE_RIDER (1)
    int assigned_address_id; // Mandatory for Riders (valid ID), 0 (N/A) for Admins
} User;
```

---

## Functions

### `display_login_prompt`
```c
void display_login_prompt(char *username, char *password);
```
- **Purpose**: Prompts the user to enter their credentials on the terminal.
- **Parameters**:
  | Parameter | Type | Description |
  |-----------|------|-------------|
  | `username` | `char*` | Output buffer to store the input username (min 1, max 29 chars). |
  | `password` | `char*` | Output buffer to store the input password (min 1, max 29 chars). |
- **Returns**: `void`
- **Internal Logic**: Uses `get_validated_string` to securely capture inputs without overflow risk.

---

### `verify_credentials`
```c
int verify_credentials(User users[], int user_count, const char *username, const char *password);
```
- **Purpose**: Checks if a given username and password combination exists in the registered users database.
- **Parameters**:
  | Parameter | Type | Description |
  |-----------|------|-------------|
  | `users[]` | `User` | Array of all registered users. |
  | `user_count` | `int` | Total number of registered users. |
  | `username` | `const char*` | Username to verify. |
  | `password` | `const char*` | Password to verify. |
- **Returns**: `int` — The index of the matching user in the `users` array, or `ROLE_INVALID` (`-1`) if no match is found.
- **Usage**: Used internally by the login coordinator loop.

---

### `login`
```c
int login(User users[], int user_count);
```
- **Purpose**: Coordinates the authentication process, allowing the user a limited number of login attempts.
- **Parameters**:
  | Parameter | Type | Description |
  |-----------|------|-------------|
  | `users[]` | `User` | Array of all registered users. |
  | `user_count` | `int` | Total number of registered users. |
- **Returns**: `int` — The index of the successfully logged-in user in the array, or `ROLE_INVALID` (`-1`) if authentication fails.
- **Key Logic**:
  - Limits user to `MAX_ATTEMPTS` (`3`).
  - Clears screen and prompts via `display_login_prompt`.
  - Verifies inputs via `verify_credentials`.
  - If attempts are exhausted, outputs an exit message.

---

### `register_rider`
```c
int register_rider(User users[], int *user_count, const char *username, const char *password, int assigned_address_id);
```
- **Purpose**: Registers a new rider user, auto-generating a unique ID and enforcing critical business rules.
- **Parameters**:
  | Parameter | Type | Description |
  |-----------|------|-------------|
  | `users[]` | `User` | Array of registered users. |
  | `user_count` | `int*` | Pointer to the integer tracking total users. |
  | `username` | `const char*` | Desired username for the new rider. |
  | `password` | `const char*` | Desired password. |
  | `assigned_address_id` | `int` | The ID of the road/street the rider is assigned to. |
- **Returns**: `int`:
  - `1` on success.
  - `0` if user capacity limit (`10`) is reached.
  - `-1` if the username is already taken.
  - `-2` if `assigned_address_id` is missing/invalid (`<= 0`).
  - `-3` if the target road is already assigned to another rider.
- **Key Logic**:
  - Automatically calculates the next unique user ID by checking the current maximum ID (`max_id + 1`).
  - Enforces mandatory road assignment for riders.
  - Enforces that no two riders share the same `assigned_address_id`.

---

### `update_user`
```c
int update_user(User users[], int user_count, int user_id, const char *new_username, int new_role, int new_assigned_address_id);
```
- **Purpose**: Modifies user account details and automatically updates related properties.
- **Parameters**:
  | Parameter | Type | Description |
  |-----------|------|-------------|
  | `users[]` | `User` | Array of registered users. |
  | `user_count` | `int` | Current number of users. |
  | `user_id` | `int` | Unique ID of the user to update. |
  | `new_username` | `const char*` | New username (or empty string to keep current). |
  | `new_role` | `int` | New role index (`0` for Admin, `1` for Rider, `-1` to keep current). |
  | `new_assigned_address_id` | `int` | New road assignment ID (or `-1` to keep current). |
- **Returns**: `int`:
  - `1` on success.
  - `0` if the user is not found.
  - `-1` if the new username conflicts with an existing user.
  - `-2` if a Rider is missing a valid road assignment.
  - `-3` if the new road assignment conflicts with another rider.
- **Key Logic**:
  - Automatically resets `assigned_address_id` to `0` if a user's role transitions to Admin.
  - Validates uniqueness of road assignment if updating/maintaining a Rider role.

---

### `delete_user`
```c
int delete_user(User users[], int *user_count, int user_id);
```
- **Purpose**: Deletes a Rider user from the system by shifting subsequent elements to maintain array continuity.
- **Parameters**:
  | Parameter | Type | Description |
  |-----------|------|-------------|
  | `users[]` | `User` | Array of registered users. |
  | `user_count` | `int*` | Pointer to current number of users. |
  | `user_id` | `int` | Unique ID of the user to delete. |
- **Returns**: `int`:
  - `1` on success.
  - `0` if the user is not found.
  - `-2` if the target user is an Admin (Admin deletion is blocked).
- **Key Logic**:
  - Prevents accidental deletion of administrators.
  - Compacts the user array by shifting subsequent records leftwards.

---

### `display_user_list`
```c
void display_user_list(User users[], int user_count, Address addresses[], int addr_count);
```
- **Purpose**: Prints a formatted table listing user IDs, usernames, roles, and assigned roads.
- **Parameters**:
  | Parameter | Type | Description |
  |-----------|------|-------------|
  | `users[]` | `User` | Array of users to print. |
  | `user_count` | `int` | Current number of users. |
  | `addresses[]` | `Address` | Array of registered addresses (used to resolve road names). |
  | `addr_count` | `int` | Current number of addresses. |
- **Returns**: `void`
- **Internal Logic**: Looks up the assigned address text using `find_address` to display it clearly, rather than just showing raw address IDs.

---

### `display_address_list_with_riders`
```c
void display_address_list_with_riders(Address addresses[], int addr_count, User users[], int user_count);
```
- **Purpose**: Displays a comprehensive table of all roads and shows the rider assigned to each.
- **Parameters**:
  | Parameter | Type | Description |
  |-----------|------|-------------|
  | `addresses[]` | `Address` | Array of registered addresses. |
  | `addr_count` | `int` | Current number of addresses. |
  | `users[]` | `User` | Array of registered users. |
  | `user_count` | `int` | Current number of users. |
- **Returns**: `void`
- **Internal Logic**: Iterates through each address and cross-references it with active rider users to identify who is assigned, printing `NULL` if no rider is assigned.
