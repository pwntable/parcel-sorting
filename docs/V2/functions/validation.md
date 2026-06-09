# Validation Module

- **Source File**: [validation.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/validation.c)
- **Header File**: [validation.h](file:///Users/mac/Documents/GitHub/parcel-sorting/include/validation.h)
- **Module Owner**: Aidil
- **Core Responsibility**: Validate console inputs and manage the "Double Escape Hatch" design pattern.

---

## Overview
The Validation module manages user inputs. It contains helper functions that prevent buffer overflows, strip leading and trailing whitespace, validate data types, and check character lengths. It also implements the **Double Escape Hatch** pattern, allowing users to press Enter (or enter `0`) to cancel operations.

---

## Double Escape Hatch Pattern
To prevent users from getting stuck in input loops, input prompts support two escape options:
1. **Enter Key Escape**: Pressing Enter with an empty input cancels the action or retains the current value (e.g. during address updates).
2. **Numeric Zero Escape**: Entering `0` during ID selection prompts (e.g. user updates or parcel status updates) cancels the action and returns to the previous menu.

---

## Functions

### `clear_screen`
```c
void clear_screen(void);
```
- **Purpose**: Clears the console terminal screen.
- **Key Logic**: Executes `"cls"` on Windows platforms or `"clear"` on Unix/macOS/Linux.

---

### `trim_whitespace`
```c
void trim_whitespace(char *str);
```
- **Purpose**: Removes leading and trailing whitespace characters (spaces, tabs, newlines) from a string in-place.
- **Parameters**: Refer to [validation.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/validation.c#L25-L33).

---

### `safe_read_string`
```c
int safe_read_string(char *buffer, int size);
```
- **Purpose**: Reads input from `stdin` safely, preventing buffer overflow.
- **Parameters**: Refer to [validation.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/validation.c#L44-L57).
- **Returns**: `int` — `1` on success, `0` on error.
- **Key Logic**:
  - Uses `fgets` to prevent overflows.
  - Strips the trailing newline character (`\n`) if present.
  - If the input exceeds the buffer size (no newline found), flushes `stdin` to clear remaining characters.

---

### `validate_integer`
```c
int validate_integer(const char *input);
```
- **Purpose**: Checks if a string contains only numeric digits.
- **Parameters**: Refer to [validation.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/validation.c#L65-L72).
- **Returns**: `int` — `1` if all characters are digits, `0` otherwise.

---

### `validate_string_length`
```c
int validate_string_length(const char *input, int min, int max);
```
- **Purpose**: Validates whether a string's length falls within a specified range.
- **Returns**: `int` — `1` if within range, `0` otherwise.

---

### `validate_alphanumeric`
```c
int validate_alphanumeric(const char *input);
```
- **Purpose**: Validates whether a string contains only letters and numbers.
- **Returns**: `int` — `1` if the string is alphanumeric, `0` otherwise.

---

### `parse_parcel_id_input`
```c
int parse_parcel_id_input(const char *input);
```
- **Purpose**: Extracts the first sequence of numbers encountered in a string.
- **Parameters**: Refer to [validation.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/validation.c#L110-L122).
- **Returns**: `int` — The parsed ID, or `0` if no digits are found.
- **Usage**: Used to parse scanned input codes or barcode labels containing non-numeric formatting (e.g., extracting `12` from `"*P-0012*"`).

---

### `get_validated_choice`
```c
int get_validated_choice(const char *prompt, int min, int max);
```
- **Purpose**: Prompts the user to select an option from a numeric menu within a range.
- **Parameters**: Refer to [validation.c](file:///Users/mac/Documents/GitHub/validation.c#L134-L150).
- **Returns**: `int` — The validated menu option chosen by the user.

---

### `get_validated_string`
```c
int get_validated_string(const char *prompt, char *dest, int dest_size, int min_len, int max_len, int is_alphanumeric, int allow_empty);
```
- **Purpose**: Prompts the user for a string input and enforces formatting and validation rules.
- **Parameters**: Refer to [validation.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/validation.c#L164-L194).
- **Returns**: `int` — Returns `1` once a valid input is received.
- **Key Logic**:
  - Enforces minimum/maximum string length.
  - Enforces alphanumeric checks if requested.
  - If `allow_empty` is `1` and the user presses Enter, returns an empty string to support the **Double Escape Hatch** pattern.

---

### `get_validated_int_id`
```c
int get_validated_int_id(const char *prompt, int allow_empty, int *out_id);
```
- **Purpose**: Prompts the user to enter a positive integer ID.
- **Parameters**: Refer to [validation.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/validation.c#L204-L226).
- **Returns**: `int` — Returns `1` once a valid ID is received.
- **Key Logic**:
  - Validates that the input is a positive integer.
  - Supports the **Double Escape Hatch**: if `allow_empty` is `1` and the user presses Enter, sets `out_id` to `0`.
