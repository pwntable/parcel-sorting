# Comparative Documentation: V1 vs. V2 Architectures

This comparative document presents an executive-level analysis of the enhancements, structural refactoring, validation rules, and relational models introduced in **V2** of the **Modular Parcel Sorting & Rider Assignment System** relative to the initial **V1** release.

---

## 1. Feature-by-Feature Structural Comparison

| Feature Module | V1 Behavior (Legacy) | V2 Behavior (Enhanced) | Structural Impact |
| :--- | :--- | :--- | :--- |
| **Rider Road Uniqueness** | Multiple riders could be assigned to the exact same street ID, leading to package delivery conflicts. | Strict relational check ensures **1 rider can be assigned to exactly 1 unique road**. | Enforces transactional database integrity; prevents administrative errors dynamically. |
| **Address List Table Presentation** | Printed only basic properties (`ID`, `Street`, `City`, `State`). | Added **`Assigned Rider`** column showing which rider username is assigned to that street or **`NULL`** if vacant. | Improves transparency during parcel registration and user assignment. |
| **Status Transitions** | Admins/Riders typed numbers 1-3 to arbitrarily set status, permitting illegal jumps (e.g. `Delivered` directly from `Pending`). | **Dynamic Transitions** menu showing only permissible next steps. Automatically registers transition timestamp. | Re-sorts the prioritized linked list live on every valid transition. Enforces business logic. |
| **Cancel Escape Hatches** | Operations were rigid; prompts had no cancellation option, forcing users to input dummy values or terminate the app (`^C`). | Added **Double Escape Hatch** (`Enter` key or `0` choice) universally to cancel updates safely. | Significantly enhances keyboard-centric CLI user experience. |
| **Rider Road Selection Loop** | Entering a duplicate or invalid address ID terminated rider registration or updates with a generic failure. | Refactored into a **loopable retry prompt** that continues until a vacant, valid road is given, or cancelled. | Eliminates workflow disruption by keeping the operator within the registration flow. |

---

## 2. API Signature & Code Evolution

### 2.1 Rider Registration
* **V1 Pattern:** Basic duplicate username check; no road availability validation.
* **V2 Pattern:** Dual validation:
  1. Scans existing riders for username conflicts.
  2. Scans for duplicate road assignments.
```c
// Returns 1 on success, -1 on duplicate username, -2 on missing road, -3 on duplicate road assignment
int register_rider(User users[], int *user_count, const char *username, const char *password, int assigned_address_id);
```

### 2.2 Address Table Listing
* **V1 Pattern:** Stateless print of `Address` structures.
* **V2 Pattern:** relational inner-join with active user database.
```c
// Relational join with User database to extract assigned riders live
void display_address_list_with_riders(Address addresses[], int addr_count, User users[], int user_count);
```

### 2.3 Status Transition Management
* **V1 Pattern:** Static list of options.
* **V2 Pattern:** Dynamic state machine mapping.
```c
// Dynamically filters allowed next states based on current state (Pending -> Out for Del / Delivered; Out for Del -> Delivered)
void display_dynamic_status_options(const char *current_status);
int is_valid_transition(const char *current_status, const char *new_status);
```

---

> [!IMPORTANT]
> **V2 System Integrity Guarantee**: The database files and memory structs maintain complete backwards compatibility. All V1 data files can be loaded into V2 with no schema conversion, but V2 will enforce the strict relational integrity rules moving forward.
