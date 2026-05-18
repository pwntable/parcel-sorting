# V2 CLI Expected Outcome (Happy Path & Validation Flow)

This document outlines the successful execution flows and validation pathways for all menus of the **V2 Modular Parcel Sorting & Rider Assignment System**.

---

## 1. Login Module

### Admin Login
```text
=== PARCEL SORTING SYSTEM ===
Login
Enter Username: admin
Enter Password: admin123
```

### Rider Login
```text
=== PARCEL SORTING SYSTEM ===
Login
Enter Username: rider1
Enter Password: rider123
```

---

## 2. Admin Main Menu

After administrative login, the master navigation panel is presented:
```text
=== ADMIN MAIN MENU ===
1. Users Management
2. Parcel Management
3. Address Management
4. Reports & Analytics
5. Logout
Enter Choice: 
```

---

## 3. Users Management Sub-Menu

```text
=== USERS MANAGEMENT ===
1. View All Users
2. Register New Rider
3. Update User
4. Delete User
5. Return to Main Menu
Enter Choice: 2
```

---

## 4. View All Users Table (Relational Integration)

```text
--- All Users ---
--------------------------------------------------------------------------------
ID     | Username             | Role       | Assigned Road                      
--------------------------------------------------------------------------------
1      | admin                | Admin      | N/A (NULL)                         
2      | rider1               | Rider      | Jalan Tun Razak (Kuala Lumpur)     
--------------------------------------------------------------------------------
```

---

## 5. Register New Rider (Duplicate Road Validation & Loop)

If registering a new rider, the system outputs the assigned rider for each road in the system and loops on duplicate road selection until a vacant street ID is provided or the admin cancels.

```text
Enter New Rider Username (or press Enter to cancel): rider2
Enter Password: rider2password

=== ASSIGN ROAD FOR RIDER ===
----------------------------------------------------------------------------------------------------
ID    | Street                    | City            | State           | Assigned Rider      
----------------------------------------------------------------------------------------------------
101   | Jalan Tun Razak           | Kuala Lumpur    | WP Kuala Lumpur | rider1              
102   | Jalan Ampang              | Kuala Lumpur    | WP Kuala Lumpur | NULL                
----------------------------------------------------------------------------------------------------
Enter Address ID to assign (Must be a valid ID from the list, or press Enter/0 to cancel): 101

Error: Duplicate Road Assignment! This road is already assigned to another rider.
Press Enter to try again...

=== ASSIGN ROAD FOR RIDER ===
----------------------------------------------------------------------------------------------------
ID    | Street                    | City            | State           | Assigned Rider      
----------------------------------------------------------------------------------------------------
101   | Jalan Tun Razak           | Kuala Lumpur    | WP Kuala Lumpur | rider1              
102   | Jalan Ampang              | Kuala Lumpur    | WP Kuala Lumpur | NULL                
----------------------------------------------------------------------------------------------------
Enter Address ID to assign (Must be a valid ID from the list, or press Enter/0 to cancel): 102

===========================================
        RIDER REGISTERED SUCCESSFULLY!     
===========================================
New Rider Details:
  User ID       : 3
  Username      : rider2
  Role          : Rider
  Assigned Road : Jalan Ampang, Kuala Lumpur, WP Kuala Lumpur
===========================================

Press Enter to continue...
```

---

## 6. Update User (Road Assignment Loop)

Converting a user to a Rider triggers the loopable Unique Road selector.

```text
Enter User ID to Update (or press Enter/0 to cancel): 3
Enter New Username (Current: rider2, leave empty to keep): rider2_new
Enter New Role (0: Admin, 1: Rider, or press Enter to keep current Rider): 1

=== UPDATE ROAD ASSIGNMENT FOR RIDER ===
----------------------------------------------------------------------------------------------------
ID    | Street                    | City            | State           | Assigned Rider      
----------------------------------------------------------------------------------------------------
101   | Jalan Tun Razak           | Kuala Lumpur    | WP Kuala Lumpur | rider1              
102   | Jalan Ampang              | Kuala Lumpur    | WP Kuala Lumpur | rider2              
----------------------------------------------------------------------------------------------------
Enter Address ID to assign (Must be a valid ID from the list, press Enter to keep current, or 0 to cancel): 101

Error: Duplicate Road Assignment! This road is already assigned to another rider.
Press Enter to try again...

=== UPDATE ROAD ASSIGNMENT FOR RIDER ===
----------------------------------------------------------------------------------------------------
ID    | Street                    | City            | State           | Assigned Rider      
----------------------------------------------------------------------------------------------------
101   | Jalan Tun Razak           | Kuala Lumpur    | WP Kuala Lumpur | rider1              
102   | Jalan Ampang              | Kuala Lumpur    | WP Kuala Lumpur | rider2              
----------------------------------------------------------------------------------------------------
Enter Address ID to assign (Must be a valid ID from the list, press Enter to keep current, or 0 to cancel): 0

Update cancelled.
```

---

## 7. Dynamic Status Transitions & Double Escape Hatch

For a `Pending` parcel, the available status transitions automatically exclude invalid jumps. The Admin/Rider can hit **Enter** or enter **`0`** to abort the transition without editing.

```text
=== UPDATE PARCEL STATUS (ID: 1) ===
Current Status: Pending
Available Transitions:
1. Out for Delivery
2. Delivered
0. Cancel Update

Enter Choice (or press Enter/0 to cancel): 1

Updated and re-sorted!
```

---

## 8. View All Addresses (Address Management Sub-Menu)

Viewing all addresses displays the live assigned rider for each road in the system:

```text
=== ADDRESS MANAGEMENT ===
1. View All Addresses
2. Add New Address
3. Update Address
4. Return to Main Menu
Enter Choice: 1

----------------------------------------------------------------------------------------------------
ID    | Street                    | City            | State           | Assigned Rider      
----------------------------------------------------------------------------------------------------
101   | Jalan Tun Razak           | Kuala Lumpur    | WP Kuala Lumpur | rider1              
102   | Jalan Ampang              | Kuala Lumpur    | WP Kuala Lumpur | rider2              
----------------------------------------------------------------------------------------------------
```

---

## 9. View All Parcels (Sorted)

The parcel table now dynamically displays the **Sender** name alongside the receiver.

```text
=== PARCEL MANAGEMENT ===
1. View All Parcels (Sorted)
2. Create New Parcel
3. Search Parcel
4. Update Parcel Status
5. Delete Parcel
6. Return to Main Menu
Enter Choice: 1

--- All Parcels (Prioritized) ---
--------------------------------------------------------------------------------------------------------------------------------------
ID   | Sender          | Receiver        | Type     | Status       | House# | Street               | City            | State          
--------------------------------------------------------------------------------------------------------------------------------------
1    | Ali             | Abu             | Fast     | Pending      | 12     | Jalan Tun Razak      | Kuala Lumpur    | WP Kuala Lumpur
2    | Siti            | Ahmad           | Standard | Pending      | 5      | Jalan Ampang         | Kuala Lumpur    | WP Kuala Lumpur
--------------------------------------------------------------------------------------------------------------------------------------
Total: 2 parcels

Press Enter to continue...
```
