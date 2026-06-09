# Status Parcel Module

- **Source File**: [status.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/status.c)
- **Header File**: [status.h](file:///Users/mac/Documents/GitHub/parcel-sorting/include/status.h)
- **Module Owner**: Kimi
- **Core Responsibility**: Implement a state machine for parcel delivery status transitions and manage delivery completion timestamps.

---

## Overview
The Status Parcel module acts as the state machine that governs delivery progress. It defines the allowed status transitions for parcels (Pending, Out for Delivery, Delivered) and enforces business rules to prevent invalid transitions. When a parcel is successfully delivered, it automatically timestamps the delivery.

---

## Allowed State Transitions

The system implements a directed state workflow for parcels:
```mermaid
graph LR
    Pending --> Out_for_Delivery["Out for Delivery"]
    Pending --> Delivered
    Out_for_Delivery --> Delivered
```

- **Pending**: Initial state when a parcel is registered. Can transition to "Out for Delivery" or "Delivered".
- **Out for Delivery**: Intermediate delivery state. Can only transition to "Delivered".
- **Delivered**: Terminal state. No outgoing transitions are permitted.

---

## Functions

### `get_current_time`
```c
void get_current_time(char *buffer, int size);
```
- **Purpose**: Formats the current calendar date and time.
- **Parameters**: Refer to [status.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/status.c#L14-L20).
- **Format**: `"YYYY-MM-DD HH:MM"` (e.g., `"2026-06-09 15:30"`).
- **Usage**: Used to record creation (`time_in`) and delivery completion (`time_out`) times.

---

### `display_status_options`
```c
void display_status_options(void);
```
- **Purpose**: Prints all possible parcel delivery statuses to the console.

---

### `display_dynamic_status_options`
```c
void display_dynamic_status_options(const char *current_status);
```
- **Purpose**: Prints the available status transitions for a parcel based on its current status.
- **Parameters**: Refer to [status.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/status.c#L42-L52).

---

### `is_valid_transition`
```c
int is_valid_transition(const char *current_status, const char *new_status);
```
- **Purpose**: Validates whether a proposed status transition is permitted.
- **Parameters**: Refer to [status.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/status.c#L66-L74).
- **Returns**: `int` — `1` if the transition is allowed, `0` if it is blocked.

---

### `update_parcel_status`
```c
int update_parcel_status(ParcelNode **head, int parcel_id, const char *new_status);
```
- **Purpose**: Updates a parcel's status and re-sorts the list.
- **Parameters**: Refer to [status.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/status.c#L87-L105).
- **Returns**: `int`:
  - `1` on success.
  - `0` if the parcel was not found.
  - `-1` if the transition is invalid.
- **Key Logic**:
  - Finds the parcel in the linked list using `find_parcel`.
  - Validates the transition using `is_valid_transition`.
  - Updates the status field.
  - If the status transitions to `"Delivered"`, calls `get_current_time` to set the `time_out` timestamp.
  - Calls `sort_parcel_list` to move delivered parcels to the bottom of the list.
