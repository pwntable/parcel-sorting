# Project Expected Outcome (Happy Path)

This document outlines the straightforward, successful execution path for all modules of the **Modular Parcel Sorting & Rider Assignment System**. It lists the clean, happy-path CLI console inputs and structured tabular outputs.

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

When logging in as `admin`, the following primary control menu appears:

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

## 3. Rider Menu

When logging in as a Rider, the system displays the Rider's name and their assigned road:

```text
=== RIDER MENU (User: rider1) ===
Assigned Road: Jalan Tun Razak (Kuala Lumpur)
----------------------------------------
1. View My Assigned Road Parcels
2. View All Sorted Parcels
3. Update Parcel Status
4. Logout
Enter Choice: 
```

---

## 4. Users Management Sub-Menu

```text
=== USERS MANAGEMENT ===
1. View All Users
2. Register New Rider
3. Update User
4. Delete User
5. Return to Main Menu
Enter Choice: 
```

---

## 5. View All Users

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

## 6. Register New Rider

```text
Enter New Rider Username (or press Enter to cancel): rider2
Enter Password: rider2password

=== ASSIGN ROAD FOR RIDER ===
------------------------------------------------------------------
ID    | Street                    | City            | State          
------------------------------------------------------------------
101   | Jalan Tun Razak           | Kuala Lumpur    | WP Kuala Lumpur
102   | Jalan Ampang              | Kuala Lumpur    | WP Kuala Lumpur
------------------------------------------------------------------
Enter Address ID to assign (Must be a valid ID from the list): 102

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

## 7. Update User

```text
--- Current Users ---
--------------------------------------------------------------------------------
ID     | Username             | Role       | Assigned Road                      
--------------------------------------------------------------------------------
1      | admin                | Admin      | N/A (NULL)                         
2      | rider1               | Rider      | Jalan Tun Razak (Kuala Lumpur)     
3      | rider2               | Rider      | Jalan Ampang (Kuala Lumpur)        
--------------------------------------------------------------------------------

Enter User ID to Update (or press Enter/0 to cancel): 3
Enter New Username (Current: rider2, leave empty to keep): rider2_updated
Enter New Role (0: Admin, 1: Rider, or press Enter to keep current Rider): 1

=== UPDATE ROAD ASSIGNMENT FOR RIDER ===
------------------------------------------------------------------
ID    | Street                    | City            | State          
------------------------------------------------------------------
101   | Jalan Tun Razak           | Kuala Lumpur    | WP Kuala Lumpur
102   | Jalan Ampang              | Kuala Lumpur    | WP Kuala Lumpur
------------------------------------------------------------------
Enter Address ID to assign (Must be a valid ID from the list, or press Enter to keep current): 101

User updated successfully!

Press Enter to continue...
```

---

## 8. Delete User

```text
--- Current Users ---
--------------------------------------------------------------------------------
ID     | Username             | Role       | Assigned Road                      
--------------------------------------------------------------------------------
1      | admin                | Admin      | N/A (NULL)                         
2      | rider1               | Rider      | Jalan Tun Razak (Kuala Lumpur)     
--------------------------------------------------------------------------------

Enter User ID to Delete (or press Enter/0 to cancel): 2
User deleted successfully.

Press Enter to continue...
```

---

## 9. Parcel Management Sub-Menu

```text
=== PARCEL MANAGEMENT ===
1. View All Parcels (Sorted)
2. Create New Parcel
3. Search Parcel
4. Update Parcel Status
5. Delete Parcel
6. Return to Main Menu
Enter Choice: 
```

---

## 10. View All Parcels (Sorted & Priority Zones)

All dynamic addresses are auto-resolved relationally:

```text
--- All Parcels (Prioritized) ---
------------------------------------------------------------------------------------------------------------------------
ID | Sender          | Receiver        | Type       | Status       | House# | Street          | City         | State
------------------------------------------------------------------------------------------------------------------------
1  | Ali             | Abu             | Fast       | Pending      | 12     | Jalan Tun Razak | Kuala Lumpur | WP KL
2  | Siti            | Ahmad           | Standard   | Pending      | 5      | Jalan Ampang    | Kuala Lumpur | WP KL
------------------------------------------------------------------------------------------------------------------------
Total: 2 parcels
```

---

## 11. Create New Parcel

```text
=== CREATE NEW PARCEL ===
Sender: Aiman
Receiver: Ibad

=== SELECT ADDRESS ===
------------------------------------------------------------------
ID    | Street                    | City            | State          
------------------------------------------------------------------
101   | Jalan Tun Razak           | Kuala Lumpur    | WP Kuala Lumpur
102   | Jalan Ampang              | Kuala Lumpur    | WP Kuala Lumpur
------------------------------------------------------------------
Enter Address ID (Must be a valid ID from the list): 101
House #: 45
Type (1:Fast, 2:Standard): 1

Parcel created and auto-sorted!
```

---

## 12. Search Parcel

```text
=== SEARCH ===
1. By ID
2. By Receiver
Enter: 1
ID: 1

--- All Parcels (Prioritized) ---
------------------------------------------------------------------------------------------------------------------------
ID | Sender          | Receiver        | Type       | Status       | House# | Street          | City         | State
------------------------------------------------------------------------------------------------------------------------
1  | Ali             | Abu             | Fast       | Pending      | 12     | Jalan Tun Razak | Kuala Lumpur | WP KL
------------------------------------------------------------------------------------------------------------------------
```

---

## 13. Update Parcel Status

```text
--- Current Active Parcels (Sorted) ---
------------------------------------------------------------------------------------------------------------------------
ID | Sender          | Receiver        | Type       | Status       | House# | Street          | City         | State
------------------------------------------------------------------------------------------------------------------------
1  | Ali             | Abu             | Fast       | Pending      | 12     | Jalan Tun Razak | Kuala Lumpur | WP KL
2  | Siti            | Ahmad           | Standard   | Pending      | 5      | Jalan Ampang    | Kuala Lumpur | WP KL
------------------------------------------------------------------------------------------------------------------------

Enter Parcel ID to Update (or press Enter/0 to cancel): 1

=== STATUS OPTIONS ===
1. Pending
2. Out for Delivery
3. Delivered
New Status (1-3): 3

Updated and re-sorted!
```

---

## 14. Delete Parcel

```text
Enter Parcel ID to Delete (or press Enter/0 to cancel): 1
Deleted successfully.
```

---

## 15. Address Management Sub-Menu

```text
=== ADDRESS MANAGEMENT ===
1. View All Addresses
2. Add New Address
3. Update Address
4. Return to Main Menu
Enter Choice: 
```

---

## 16. View All Addresses

```text
--------------------------------------------------------------------------------
ID     | Street                         | City                 | State          
--------------------------------------------------------------------------------
101    | Jalan Tun Razak                | Kuala Lumpur         | WP Kuala Lumpur
102    | Jalan Ampang                   | Kuala Lumpur         | WP Kuala Lumpur
--------------------------------------------------------------------------------
```

---

## 17. Add New Address

```text
Street: Jalan Bukit Bintang
City: Kuala Lumpur
State: WP Kuala Lumpur
Added.
```

---

## 18. Update Address

```text
ID to Update (or press Enter/0 to cancel): 101
Enter New Street (Current: Jalan Tun Razak): Jalan Tun Razak Baru
Enter New City (Current: Kuala Lumpur): Kuala Lumpur
Enter New State (Current: WP Kuala Lumpur): WP Kuala Lumpur
Updated successfully.
```

---

## 19. Reports & Analytics Sub-Menu

```text
=== REPORTS & ANALYTICS ===
1. Admin Summary Report
2. View Delivery Queue (Non-Delivered)
3. Return to Main Menu
Enter Choice: 
```

---

## 20. Admin Summary Report

```text
===========================================
          ADMIN SYSTEM SUMMARY
===========================================
Total Registered Parcels: 3
  - Pending             : 1
  - Out for Delivery    : 1
  - Delivered (History) : 1

Total Registered Riders : 2
Total Active Addresses  : 2
===========================================
```

---

## 21. View Delivery Queue (Non-Delivered)

Displays active packages sorted priority-wise (Delivered parcels are filtered out):

```text
--- Priority Delivery Queue ---
------------------------------------------------------------------------------------------------------------------------
ID | Sender          | Receiver        | Type       | Status       | House# | Street          | City         | State
------------------------------------------------------------------------------------------------------------------------
2  | Siti            | Ahmad           | Fast       | Pending      | 5      | Jalan Ampang    | Kuala Lumpur | WP KL
1  | Ali             | Abu             | Standard   | Out for Del  | 12     | Jalan Tun Razak | Kuala Lumpur | WP KL
------------------------------------------------------------------------------------------------------------------------
```
