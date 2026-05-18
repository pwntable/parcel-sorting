# 📂 V2 Folder Structure

## Recommended Project Layout

```
parcel-sorting/
│
├── main.c                      # Entry point — menu loop, integration
│
├── src/                        # Source files (.c)
│   ├── login.c                 # Authentication logic & User Management
│   ├── validation.c            # Input validation utilities
│   ├── database.c              # File I/O — load/save data
│   ├── sorting.c               # Parcel sorting algorithm
│   ├── status.c                # Parcel status management & dynamic transitions
│   ├── address.c               # Address CRUD operations
│   ├── output.c                # Formatted display/output (tables, summaries)
│   ├── search.c                # Parcel search/filter
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
├── tests/                      # Automated test suites
│   └── test_suite.c            # Comprehensive tests for V2 constraints
│
├── data/                       # Data files (runtime)
│   ├── users.txt               # Username, password, role, assigned_address_id
│   ├── parcels.txt             # Parcel records
│   └── addresses.txt           # Predefined addresses
│   (Note: riders.txt was merged into users.txt in V2)
│
├── docs/                       # Documentation
│   ├── V1 Legacy Docs...
│   └── V2/                     # V2 Documentation (this folder)
│       ├── database_schema.md
│       ├── documentation_comparison.md
│       ├── expected_outcome.md
│       ├── folder_structure.md
│       ├── module_specifications.md
│       ├── project_overview.md
│       └── system_architecture.md
│
└── Makefile                    # Build automation & test runner
```

---

## File Descriptions

### Root

| File | Purpose |
|------|---------|
| `main.c` | Program entry point. Initializes data, manages user sessions, and implements Double Escape Hatch UI logic. |
| `Makefile` | Automates compilation with `make`, `make test`, and `make clean`. |

### `/src` — Source Files

| File | Purpose |
|------|---------|
| `parcel_list.c` | Core linked list operations: insert, delete, find, count, free. |
| `login.c` | Login, User CRUD, and Rider uniqueness logic (`display_address_list_with_riders`). |
| `output.c` | Dynamic table printing. |
| `validation.c` | Escape hatch checks and format parsers. |
| `search.c` | Search functions — by ID, receiver name, status, type |
| `database.c` | File read/write logic. |
| `sorting.c` | Sorting pipeline — filter, split, sort, merge |
| `status.c` | Status transitions and state-machine validation (`is_valid_transition`). |
| `address.c` | Address CRUD — add, update, find. |

### `/data` — Data Files (V2 Schema)

| File | Format | Example Content |
|------|--------|----------------|
| `users.txt` | `username,password,role,assigned_address_id` | `rider1,rider123,1,101` |
| `parcels.txt` | `id,sender,receiver,addr_id,type,status,house#,time_in,time_out,rider_id` | `1,Ali,Abu,101,Fast,Pending,12,2026-05-06 10:00,,0` |
| `addresses.txt` | `id,street,city,state,house_number` | `101,Jalan Merdeka,Kuala Lumpur,Selangor,12` |

---

## Linked List File Separation

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
```

---

## Compilation

### Makefile

```makefile
all: parcel_system

parcel_system: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

test: 
	make clean && make test_suite && ./test_suite

clean:
	rm -f $(OBJ) parcel_system test_suite
```

Usage:
```bash
make          # compile main application
make test     # compile and run full V2 test suite
make clean    # remove executables and objects
```
