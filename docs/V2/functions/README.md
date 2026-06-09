# Function & Module Documentation (V2)

Welcome to the function-level documentation index for the Parcel Sorting System. This directory contains detailed descriptions, signatures, parameters, return values, and implementation logic for every function in the codebase.

## Module Assignment & Map

Below is the assignment table mapping modules to their files, owners, and core responsibilities. Click on the **Documentation Link** column to open the detailed function guide for that module.

| Module | File(s) | Owner | Core Responsibility | Documentation Link |
|--------|---------|-------|---------------------|--------------------|
| **Login** | [login.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/login.c) / [login.h](file:///Users/mac/Documents/GitHub/parcel-sorting/include/login.h) | Khai | Authenticate users, manage riders, enforce road uniqueness | [login.md](file:///Users/mac/Documents/GitHub/parcel-sorting/docs/V2/functions/login.md) |
| **Validation** | [validation.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/validation.c) / [validation.h](file:///Users/mac/Documents/GitHub/parcel-sorting/include/validation.h) | Aidil | Validate inputs and manage Double Escape Hatch | [validation.md](file:///Users/mac/Documents/GitHub/parcel-sorting/docs/V2/functions/validation.md) |
| **Database** | [database.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/database.c) / [database.h](file:///Users/mac/Documents/GitHub/parcel-sorting/include/database.h) | Piki | Load/save data from/to files | [database.md](file:///Users/mac/Documents/GitHub/parcel-sorting/docs/V2/functions/database.md) |
| **Parcel Sorting** | [sorting.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/sorting.c) / [sorting.h](file:///Users/mac/Documents/GitHub/parcel-sorting/include/sorting.h) | Aiman | Sort parcels by type and house number | [sorting.md](file:///Users/mac/Documents/GitHub/parcel-sorting/docs/V2/functions/sorting.md) |
| **Status Parcel** | [status.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/status.c) / [status.h](file:///Users/mac/Documents/GitHub/parcel-sorting/include/status.h) | Kimi | State machine for parcel status transitions | [status.md](file:///Users/mac/Documents/GitHub/parcel-sorting/docs/V2/functions/status.md) |
| **Address** | [address.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/address.c) / [address.h](file:///Users/mac/Documents/GitHub/parcel-sorting/include/address.h) | Kimi | CRUD for delivery addresses | [address.md](file:///Users/mac/Documents/GitHub/parcel-sorting/docs/V2/functions/address.md) |
| **Output** | [output.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/output.c) / [output.h](file:///Users/mac/Documents/GitHub/parcel-sorting/include/output.h) | Khai | Display formatted relational data | [output.md](file:///Users/mac/Documents/GitHub/parcel-sorting/docs/V2/functions/output.md) |
| **Search Parcel** | [search.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/search.c) / [search.h](file:///Users/mac/Documents/GitHub/parcel-sorting/include/search.h) | Aidil | Search/filter parcels | [search.md](file:///Users/mac/Documents/GitHub/parcel-sorting/docs/V2/functions/search.md) |
| **Parcel List** | [parcel_list.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/parcel_list.c) / [parcel_list.h](file:///Users/mac/Documents/GitHub/parcel-sorting/include/parcel_list.h) | Khai | Centralized shared linked list operations | [parcel_list.md](file:///Users/mac/Documents/GitHub/parcel-sorting/docs/V2/functions/parcel_list.md) |
| **Main Menu** | [main.c](file:///Users/mac/Documents/GitHub/parcel-sorting/src/main.c) | System | Orchestrate sub-menus and user session flows | [main.md](file:///Users/mac/Documents/GitHub/parcel-sorting/docs/V2/functions/main.md) |

---

## Design Standard Guidelines
- **Double Escape Hatch Pattern**: Enforced in all user-facing functions in `validation.c` and routed throughout `main.c`. Allows users to cancel out of workflows cleanly.
- **Strict Data Uniqueness**: Enforced inside `login.c` (rider-to-road mappings) and `address.c` (address registration limits).
- **Self-Sorting Linked List**: The linked list in `parcel_list.c` automatically places nodes in prioritized sorting orders based on delivery status, delivery type, and house numbers.
