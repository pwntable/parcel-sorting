# 📦 Parcel Sorting System — Project Overview

## 1. Project Description

The **Parcel Sorting System** is a terminal-based (CLI) application written in C that manages the full lifecycle of parcel delivery — from creation, sorting, assignment, to delivery tracking. The system supports two user roles (**Admin** and **Rider**) and automates the sorting of parcels by delivery type and house number for optimized delivery routes.

The entire system is built using **modular programming**, where each team member owns a specific module that integrates into a unified application through a shared **linked list** data structure.

---

## 2. Problem Statement

Manual parcel sorting is error-prone and time-consuming, especially when:

- The number of parcels is **unpredictable** (varies daily)
- Parcels must be **prioritized** by delivery type (Fast vs Standard)
- Riders need a **clear, optimized delivery order**
- Status tracking requires **real-time updates** throughout the delivery lifecycle

A structured, automated system is needed to handle dynamic parcel data efficiently while remaining simple enough for a university-level C project.

---

## 3. Objectives

| # | Objective |
|---|-----------|
| 1 | Implement a secure login system with role-based access (Admin/Rider) |
| 2 | Enable full CRUD operations on parcels, addresses, and riders |
| 3 | Automate parcel sorting by delivery type and house number |
| 4 | Track parcel status through a defined lifecycle |
| 5 | Display sorted parcel lists for both Admin and Rider views |
| 6 | Use **linked list** as the primary data structure for dynamic parcel management |

---

## 4. Key Features

### 🔐 Authentication
- Username/password login
- Role-based menu (Admin vs Rider)

### 📦 Parcel Management (Admin)
- Create, view, update, and delete parcels
- Link parcels to predefined addresses
- Assign riders to parcels

### 🗺️ Address Management (Admin)
- Add and update predefined delivery addresses
- Address ID linking for parcel creation

### 🚴 Rider Operations
- View assigned sorted parcel list
- Update parcel delivery status

### ⚡ Sorting Engine
- Filter out delivered parcels
- Group by delivery type (Fast → Standard)
- Sort each group by house number (ascending)
- Generate a final delivery queue

### 📋 Output Display
- Formatted tables for Admin and Rider views
- Sorted parcel lists with all relevant details

---

## 5. System Scope & CLI Limitations

| Aspect | Detail |
|--------|--------|
| **Interface** | Text-based terminal (no GUI) |
| **Data Persistence** | File-based (text/CSV files) — no external database |
| **Concurrency** | Single-user at a time |
| **Platform** | Compiled and run on any system with a standard C compiler |
| **Libraries** | Standard C library only (`stdio.h`, `stdlib.h`, `string.h`) |
| **Security** | Basic credential matching (no encryption) |

---

## 6. High-Level Architecture

The system follows a **modular architecture** where each module is developed independently and integrated through the `main.c` entry point. All modules share access to a central **linked list** that holds parcel data in memory.

```
┌─────────────────────────────────────────────────┐
│                    main.c                       │
│              (Entry Point & Menu)               │
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

**Flow**: Login → Role Menu → Module Operations → Linked List Read/Write → Output Display

> 📌 A detailed flowchart is available in the project root: `mermaid flowchart` and `flowchart.svg`

---

## 7. Why Linked List?

The linked list was chosen as the primary data structure for parcel management for the following practical reasons:

### ✅ Dynamic Size
Parcel count changes daily — parcels are added and delivered constantly. Unlike arrays, a linked list **grows and shrinks at runtime** without needing to define a fixed maximum size.

### ✅ Efficient Insertion & Deletion
- **Adding a parcel**: Simply allocate a new node and link it — O(1) at head, no shifting required.
- **Removing a delivered parcel**: Unlink the node and free memory — O(1) removal once located, no shifting of remaining elements.

### ✅ Memory Efficiency
Memory is allocated **only when needed** (`malloc`) and **freed when done** (`free`). No wasted memory from pre-allocated empty slots.

### ✅ Suitable for Sorting
The sorting engine can traverse the list, reorder nodes by pointer manipulation, or build separate sub-lists (Fast/Standard groups) — a natural fit for the sorting requirements.

### ✅ University-Level Appropriateness
Linked lists are a core data structure in any C programming curriculum. Using them here reinforces:
- Pointer manipulation
- Dynamic memory management (`malloc`/`free`)
- Struct usage
- Modular design patterns

### Comparison

| Feature | Array | Linked List ✅ |
|---------|-------|---------------|
| Fixed size at compile time | Yes | No |
| Easy insert/delete | No (shifting) | Yes (pointer update) |
| Memory efficiency | Wastes unused slots | Allocates on demand |
| Random access | Yes (index) | No (traversal) |
| Suitable for dynamic data | Poor | Excellent |

---

## 8. Team Responsibilities

| Module | Owner | Description |
|--------|-------|-------------|
| Login | **Khai** | Authentication and role-based access control |
| Validation | Aidil | Input validation utilities |
| Database | Piki | File I/O for data persistence |
| Parcel Sorting | Aiman | Sorting algorithm and delivery queue generation |
| Status Parcel | Kimi | Parcel status lifecycle management |
| Address | Kimi | Address CRUD and management |
| Output | **Khai** | Formatted display of parcel data and reports |
| Search Parcel | Aidil | Parcel search and filter functionality |
