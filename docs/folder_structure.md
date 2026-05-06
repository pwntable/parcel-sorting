# 📂 Folder Structure

## Recommended Project Layout

```
parcel-sorting/
│
├── main.c                      # Entry point — menu loop, integration
│
├── src/                        # Source files (.c)
│   ├── login.c                 # Authentication logic (Khai)
│   ├── validation.c            # Input validation utilities (Aidil)
│   ├── database.c              # File I/O — load/save data (Piki)
│   ├── sorting.c               # Parcel sorting algorithm (Aiman)
│   ├── status.c                # Parcel status management (Kimi)
│   ├── address.c               # Address CRUD operations (Kimi)
│   ├── output.c                # Formatted display/output (Khai)
│   ├── search.c                # Parcel search/filter (Aidil)
│   └── parcel_list.c           # ⭐ Linked list operations (shared)
│
├── include/                    # Header files (.h)
│   ├── login.h
│   ├── validation.h
│   ├── database.h
│   ├── sorting.h
│   ├── status.h
│   ├── address.h
│   ├── output.h
│   ├── search.h
│   └── parcel_list.h           # ⭐ Struct definitions + LL prototypes
│
├── data/                       # Data files (runtime)
│   ├── users.txt               # Username, password, role
│   ├── parcels.txt             # Parcel records
│   ├── addresses.txt           # Predefined addresses
│   └── riders.txt              # Rider information
│
├── docs/                       # Documentation (this folder)
│   ├── project_overview.md
│   ├── system_architecture.md
│   ├── implementation_plan.md
│   ├── module_specifications.md
│   └── folder_structure.md
│
├── mermaid flowchart            # Existing flowchart (Mermaid syntax)
├── flowchart.svg                # Existing flowchart (rendered image)
│
└── Makefile                    # (Optional) Build automation
```

---

## File Descriptions

### Root

| File | Purpose |
|------|---------|
| `main.c` | Program entry point. Includes all headers, initializes data structures, runs login, displays role-based menus, handles cleanup on exit. |
| `Makefile` | Optional. Automates compilation with `make` command. |

### `/src` — Source Files

| File | Owner | Purpose |
|------|-------|---------|
| `parcel_list.c` | Shared | Core linked list operations: insert, delete, find, count, free. **Every module that touches parcels depends on this.** |
| `login.c` | Khai | Login prompt, credential verification, attempt limiting |
| `output.c` | Khai | All `printf`-based display functions — tables, details, summaries |
| `validation.c` | Aidil | Input validation helpers — integer check, string length, trimming |
| `search.c` | Aidil | Search functions — by ID, receiver name, status, type |
| `database.c` | Piki | File read/write — load data into memory, save back to files |
| `sorting.c` | Aiman | Sorting pipeline — filter, split, sort, merge |
| `status.c` | Kimi | Status transitions — validate and update parcel status |
| `address.c` | Kimi | Address CRUD — add, update, display, find |

### `/include` — Header Files

Each `.h` file contains:
- `#ifndef` / `#define` include guards
- Struct definitions (where applicable)
- Function prototypes
- Constants / macros

**Key header**: `parcel_list.h` defines the `Parcel` struct and `ParcelNode` struct used across the entire project.

### `/data` — Data Files

| File | Format | Example Content |
|------|--------|----------------|
| `users.txt` | `username,password,role` | `admin,admin123,0` |
| `parcels.txt` | `id,sender,receiver,addr_id,type,status,house#,time_in,time_out,rider_id` | `1,Ali,Abu,101,Fast,Pending,12,2026-05-06 10:00,,0` |
| `addresses.txt` | `id,street,city,state,house_number` | `101,Jalan Merdeka,Kuala Lumpur,Selangor,12` |
| `riders.txt` | `id,name,phone` | `1,Ahmad,012-3456789` |

> Use comma-separated values (CSV) for simplicity. Each line = one record.

---

## Linked List File Separation

The linked list is isolated into its own file pair to ensure:

| Benefit | Detail |
|---------|--------|
| **Single responsibility** | `parcel_list.c` only handles list operations |
| **Shared access** | All modules include `parcel_list.h` to access the `ParcelNode` type |
| **No duplication** | Struct definitions exist in one place only |
| **Easy testing** | Can test linked list operations independently |

```
parcel_list.h          parcel_list.c
┌──────────────┐       ┌──────────────────────┐
│ Parcel struct │       │ insert_parcel()      │
│ ParcelNode   │       │ delete_parcel()      │
│ struct       │       │ find_parcel()        │
│              │       │ count_parcels()      │
│ Function     │       │ free_all_parcels()   │
│ prototypes   │       │                      │
└──────────────┘       └──────────────────────┘
       ↑                        ↑
       │                        │
  #include by:            compiled with:
  - main.c                - main.c
  - database.c            - all .c files
  - sorting.c             
  - output.c              
  - status.c              
  - search.c              
```

---

## Compilation

### Manual (gcc)

```bash
gcc -Wall -Wextra -o parcel_system \
    main.c \
    src/parcel_list.c \
    src/login.c \
    src/validation.c \
    src/database.c \
    src/sorting.c \
    src/status.c \
    src/address.c \
    src/output.c \
    src/search.c \
    -Iinclude
```

- `-Wall -Wextra` — enables all warnings (catch bugs early)
- `-Iinclude` — tells compiler to look for headers in `/include`
- `-o parcel_system` — output executable name

### Makefile (Optional)

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = main.c $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = parcel_system

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
```

Usage:
```bash
make          # compile
make run      # compile and run
make clean    # remove executable
```

---

## File Ownership Summary

| Team Member | Files Owned |
|-------------|-------------|
| **Khai** | `login.c`, `login.h`, `output.c`, `output.h` |
| **Aidil** | `validation.c`, `validation.h`, `search.c`, `search.h` |
| **Piki** | `database.c`, `database.h` |
| **Aiman** | `sorting.c`, `sorting.h` |
| **Kimi** | `status.c`, `status.h`, `address.c`, `address.h` |
| **Shared** | `main.c`, `parcel_list.c`, `parcel_list.h`, `/data/*` |
