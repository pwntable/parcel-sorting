# 📦 V2 Parcel Sorting System — Project Overview

## 1. Project Description

The **Parcel Sorting System (V2)** is a sophisticated, terminal-based (CLI) application written in C that manages the full lifecycle of parcel delivery. Building upon the V1 architecture, V2 introduces strict relational data integrity (1 Rider per 1 Unique Road), dynamic state-machine status transitions, and a highly resilient user interface featuring loopable validation and Double Escape Hatches for safe cancellations. 

The system supports two user roles (**Admin** and **Rider**) and automates the sorting of parcels by delivery type and house number for optimized delivery routes.

---

## 2. Key Enhancements in V2

### 🛡️ Strict Relational Integrity
- Merged Rider data into the core User structure.
- **Unique Road Assignments:** An admin can no longer assign two active riders to the exact same street/road. The system scans the database live and actively rejects duplicate road assignments, ensuring perfect delivery routing.

### 🔄 Resilient UI & Double Escape Hatches
- Previous linear menus now feature continuous validation loops. If a user inputs duplicate data (like an occupied road), the system prints a descriptive error and loops gracefully instead of crashing or terminating.
- **Double Escape Hatch:** At any prompt, hitting **Enter** (empty input) or typing **`0`** allows the user to safely abort the operation without corrupting data.

### 🚦 Dynamic Status State-Machine
- Parcel transitions are no longer arbitrary choices. The system evaluates the current status and dynamically filters out illegal jumps (e.g., jumping from `Pending` directly to `Delivered` without being `Out for Delivery`).

---

## 3. High-Level Architecture

The system follows a **modular architecture** where each module is integrated through the `main.c` entry point. All modules share access to a central **linked list** that holds parcel data in memory.

```
┌─────────────────────────────────────────────────┐
│                    main.c                       │
│        (Entry Point, Menu, Escape Hatches)      │
├──────────┬──────────┬──────────┬────────────────┤
│  Login   │ Database │ Address  │    Output      │
│  Module  │  Module  │  Module  │    Module      │
├──────────┼──────────┼──────────┼────────────────┤
│ Validate │  Parcel  │  Search  │    Status      │
│  Module  │ Sorting  │  Module  │    Module      │
├──────────┴──────────┴──────────┴────────────────┤
│          Linked List (Parcel Storage)           │
│        parcel_list.c / parcel_list.h            │
└─────────────────────────────────────────────────┘
```

**Flow**: Login → Role Menu → Module Operations → Linked List Read/Write → Dynamic Validations → Output Display

---

## 4. Why Linked List?

The linked list was retained as the primary data structure for parcel management for the following practical reasons:

### ✅ Dynamic Size
Parcel count changes daily. A linked list **grows and shrinks at runtime** without needing to define a fixed maximum size.

### ✅ Efficient Insertion & Deletion
- **Adding a parcel**: Simply allocate a new node and link it — O(1) at head, no shifting required.
- **Removing a delivered parcel**: Unlink the node and free memory — O(1) removal once located, no shifting.

### ✅ Suitable for Sorting
The sorting engine can traverse the list, reorder nodes by pointer manipulation, or build separate sub-lists (Fast/Standard groups) — a natural fit for priority-based delivery queues.

---

## 5. Team Responsibilities

| Module | Description |
|--------|-------------|
| **Login / User Management** | Authentication, role control, and unique road validation checks |
| **Validation** | Input validation utilities and Double Escape Hatch parsing |
| **Database** | File I/O for data persistence |
| **Parcel Sorting** | Sorting algorithm and delivery queue generation |
| **Status / Address** | Parcel status state-machine & Address CRUD |
| **Output / Search** | Relational tabular output (with Rider mappings) and parcel search |
