#include "../include/address.h"
#include "../include/database.h"
#include "../include/login.h"
#include "../include/output.h"
#include "../include/parcel_list.h"
#include "../include/search.h"
#include "../include/sorting.h"
#include "../include/status.h"
#include "../include/validation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 10
#define MAX_ADDRESSES 50

// --- SUB-MENU: USER MANAGEMENT ---
void show_user_menu(User users[], int *user_count, Address addresses[],
                    int addr_count) {
  int choice;
  char buffer[50];
  do {
    clear_screen();
    printf("=== USERS MANAGEMENT ===\n");
    printf("1. View All Users\n");
    printf("2. Register New Rider\n");
    printf("3. Update User\n");
    printf("4. Delete User\n");
    printf("5. Return to Main Menu\n");
    choice = get_validated_choice("Enter Choice: ", 1, 5);

    switch (choice) {
    case 1:
      clear_screen();
      printf("--- All Users ---\n");
      display_user_list(users, *user_count, addresses, addr_count);
      break;
    case 2: {
      char uname[30], pwd[30];
      int exists = 0;
      while (1) {
        get_validated_string("Enter New Rider Username (or press Enter to cancel): ", uname, 30, 0, 29, 1, 1);
        if (strlen(uname) == 0) {
          exists = -1; // Cancelled
          break;
        }

        // Check if username already exists
        exists = 0;
        for (int i = 0; i < *user_count; i++) {
          if (strcmp(users[i].username, uname) == 0) {
            exists = 1;
            break;
          }
        }

        if (!exists) {
          break; // Username is unique!
        }
        printf("Error: Username already exists. Please try a different username.\n\n");
      }
      if (exists == -1) break;

      get_validated_string("Enter Password: ", pwd, 30, 1, 29, 0, 0);

      // Force a valid road assignment (mandatory for Riders)
      int assigned_id = 0;
      int reg_cancelled = 0;
      while (1) {
        clear_screen();
        printf("=== ASSIGN ROAD FOR RIDER ===\n");
        display_address_list_with_riders(addresses, addr_count, users, *user_count);
        
        if (!get_validated_int_id("Enter Address ID to assign (Must be a valid ID from the list, or press Enter/0 to cancel): ", 1, &assigned_id) || assigned_id == 0) {
          reg_cancelled = 1;
          break;
        }
        
        // 1. Verify it exists in the address list
        if (find_address(addresses, addr_count, assigned_id) == NULL) {
          printf("\nError: Invalid Address ID! Please select a valid ID from the list.\n");
          printf("Press Enter to try again...\n");
          get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
          continue;
        }
        
        // 2. Verify it is not already assigned to another rider
        int road_taken = 0;
        for (int i = 0; i < *user_count; i++) {
          if (users[i].role == ROLE_RIDER && users[i].assigned_address_id == assigned_id) {
            road_taken = 1;
            break;
          }
        }
        
        if (road_taken) {
          printf("\nError: Duplicate Road Assignment! This road is already assigned to another rider.\n");
          printf("Press Enter to try again...\n");
          get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
          continue;
        }
        
        break; // Valid and unique road selected!
      }
      
      if (reg_cancelled) {
        printf("\nRegistration cancelled.\n");
        break;
      }

      int res = register_rider(users, user_count, uname, pwd, assigned_id);
      if (res == 1) {
        save_users_to_file(users, *user_count, "dataset/users.csv");
        // Look up assigned road details
        Address *addr = find_address(addresses, addr_count, assigned_id);
        char road_str[150] = "Unknown Address";
        if (addr) {
          snprintf(road_str, sizeof(road_str), "%s, %s, %s", addr->street, addr->city, addr->state);
        }
        
        clear_screen();
        printf("===========================================\n");
        printf("        RIDER REGISTERED SUCCESSFULLY!     \n");
        printf("===========================================\n");
        printf("New Rider Details:\n");
        printf("  User ID       : %d\n", users[*user_count - 1].user_id);
        printf("  Username      : %s\n", users[*user_count - 1].username);
        printf("  Role          : Rider\n");
        printf("  Assigned Road : %s\n", road_str);
        printf("===========================================\n");
      } else if (res == -1) {
        printf("Error: Username already exists.\n");
      } else if (res == -3) {
        printf("Error: Duplicate Road Assignment! This road is already assigned to another rider.\n");
      } else {
        printf("Error: User limit reached.\n");
      }
      break;
    }
    case 3: {
      int target_id = 0;
      int target_idx = -1;
      while (1) {
        clear_screen();
        printf("--- Current Users ---\n");
        display_user_list(users, *user_count, addresses, addr_count);
        
        if (!get_validated_int_id("\nEnter User ID to Update (or press Enter/0 to cancel): ", 1, &target_id) || target_id == 0) {
          target_idx = -2;
          break;
        }
        for (int i = 0; i < *user_count; i++) {
          if (users[i].user_id == target_id) {
            target_idx = i;
            break;
          }
        }
        if (target_idx != -1) {
          break;
        }
        printf("Error: User ID not found. Press Enter to try again...\n");
        get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
      }
      if (target_idx == -2 || target_idx == -1) break;

      char new_uname[30];
      char prompt_buf[120];
      snprintf(prompt_buf, sizeof(prompt_buf), "Enter New Username (Current: %s, leave empty to keep): ", users[target_idx].username);
      get_validated_string(prompt_buf, new_uname, 30, 0, 29, 1, 1);

      int new_role = -1;
      snprintf(prompt_buf, sizeof(prompt_buf), "Enter New Role (0: Admin, 1: Rider, or press Enter to keep current %s): ",
               (users[target_idx].role == ROLE_ADMIN) ? "Admin" : "Rider");
      while (1) {
        get_validated_string(prompt_buf, buffer, sizeof(buffer), 0, 1, 0, 1);
        if (strlen(buffer) == 0) {
          new_role = -1;
          break;
        }
        if (strcmp(buffer, "0") == 0) {
          new_role = ROLE_ADMIN;
          break;
        } else if (strcmp(buffer, "1") == 0) {
          new_role = ROLE_RIDER;
          break;
        }
        printf("Invalid input! Please enter 0, 1, or press Enter.\n");
      }

      int new_assigned_id = -1;
      int final_role = (new_role == -1) ? users[target_idx].role : new_role;

      int update_cancelled = 0;
      if (final_role == ROLE_RIDER) {
        while (1) {
          clear_screen();
          printf("=== UPDATE ROAD ASSIGNMENT FOR RIDER ===\n");
          display_address_list_with_riders(addresses, addr_count, users, *user_count);
          
          get_validated_string("Enter Address ID to assign (Must be a valid ID from the list, press Enter to keep current, or 0 to cancel): ",
                               buffer, sizeof(buffer), 0, 10, 0, 1);
          
          if (strcmp(buffer, "0") == 0) {
            update_cancelled = 1;
            break;
          }
          
          if (strlen(buffer) == 0) {
            if (users[target_idx].assigned_address_id > 0) {
              new_assigned_id = users[target_idx].assigned_address_id;
              break;
            } else {
              printf("\nError: This user is transitioning to a Rider role and has no current road assignment. You must assign a valid road ID!\n");
              printf("Press Enter to try again...\n");
              get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
              continue;
            }
          }

          if (!validate_integer(buffer)) {
            printf("\nError: Invalid Address ID! Please select a valid ID from the list.\n");
            printf("Press Enter to try again...\n");
            get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
            continue;
          }

          new_assigned_id = atoi(buffer);

          // 1. Verify it exists in the address list
          if (find_address(addresses, addr_count, new_assigned_id) == NULL) {
            printf("\nError: Invalid Address ID! Please select a valid ID from the list.\n");
            printf("Press Enter to try again...\n");
            get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
            continue;
          }

          // 2. Verify it is not already assigned to another rider
          int road_taken = 0;
          for (int i = 0; i < *user_count; i++) {
            if (i != target_idx && users[i].role == ROLE_RIDER && users[i].assigned_address_id == new_assigned_id) {
              road_taken = 1;
              break;
            }
          }

          if (road_taken) {
            printf("\nError: Duplicate Road Assignment! This road is already assigned to another rider.\n");
            printf("Press Enter to try again...\n");
            get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
            continue;
          }

          break; // Valid and unique road selected!
        }
      }

      if (update_cancelled) {
        printf("\nUpdate cancelled.\n");
        break;
      }

      int update_res = update_user(users, *user_count, target_id, new_uname,
                                   new_role, new_assigned_id);
      if (update_res == 1) {
        save_users_to_file(users, *user_count, "dataset/users.csv");
        printf("User updated successfully!\n");
      } else if (update_res == -1) {
        printf("Error: Username conflict (new username already taken).\n");
      } else if (update_res == -2) {
        printf("Error: Invalid road assignment for Rider.\n");
      } else if (update_res == -3) {
        printf("Error: Duplicate Road Assignment! This road is already assigned to another rider.\n");
      } else {
        printf("Error: Update failed.\n");
      }
      break;
    }
    case 4: {
      int target_id = 0;
      int res = 0;
      while (1) {
        clear_screen();
        printf("--- Current Users ---\n");
        display_user_list(users, *user_count, addresses, addr_count);
        
        if (!get_validated_int_id("\nEnter User ID to Delete (or press Enter/0 to cancel): ", 1, &target_id) || target_id == 0) {
          res = -999;
          break;
        }
        res = delete_user(users, user_count, target_id);
        if (res == 1) {
          save_users_to_file(users, *user_count, "dataset/users.csv");
          printf("User deleted successfully.\n");
          break;
        } else if (res == -2) {
          printf("Error: Cannot delete Admin. Press Enter to try again...\n");
          get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
        } else {
          printf("Error: User ID not found. Press Enter to try again...\n");
          get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
        }
      }
      break;
    }
    case 5:
      return;
    }
    printf("\nPress Enter to continue...");
    get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
  } while (choice != 5);
}

// --- SUB-MENU: PARCEL MANAGEMENT ---
void show_parcel_menu(ParcelNode **head, Address addresses[], int addr_count, User users[], int user_count) {
  int choice;
  char buffer[50];
  do {
    clear_screen();
    printf("=== PARCEL MANAGEMENT ===\n");
    printf("1. View All Parcels (Sorted)\n");
    printf("2. Create New Parcel\n");
    printf("3. Search Parcel\n");
    printf("4. Update Parcel Status\n");
    printf("5. Delete Parcel\n");
    printf("6. Return to Main Menu\n");
    choice = get_validated_choice("Enter Choice: ", 1, 6);

    switch (choice) {
    case 1:
      clear_screen();
      printf("--- All Parcels (Prioritized) ---\n");
      display_all_parcels(*head, addresses, addr_count, users, user_count);
      break;
    case 2: {
      clear_screen();
      printf("=== CREATE NEW PARCEL ===\n");
      if (addr_count == 0) {
        printf("Error: No addresses found.\n");
        break;
      }
      Parcel p = {0};
      p.parcel_id = get_next_parcel_id(*head);
      get_validated_string("Sender: ", p.sender_name, 50, 1, 49, 0, 0);
      get_validated_string("Receiver: ", p.receiver_name, 50, 1, 49, 0, 0);
      while (1) {
        display_address_list_with_riders(addresses, addr_count, users, user_count);
        if (!get_validated_int_id("Enter Address ID (Must be a valid ID from the list): ", 0, &p.address_id)) {
          continue;
        }
        if (find_address(addresses, addr_count, p.address_id) != NULL) {
          break;
        }
        printf("Invalid Address ID! Press Enter to try again...\n");
        get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
      }
      get_validated_int_id("House #: ", 0, &p.house_number);
      int type_choice = get_validated_choice("Type (1:Fast, 2:Standard): ", 1, 2);
      if (type_choice == 1)
        strcpy(p.delivery_type, "Fast");
      else
        strcpy(p.delivery_type, "Standard");
      strcpy(p.status, "Pending");
      get_current_time(p.time_in, 20);
      p.rider_id = 0;
      insert_parcel(head, p);
      save_parcels_to_file(*head, "dataset/parcels.csv");

      Address *addr = find_address(addresses, addr_count, p.address_id);
      char road_str[150] = "Unknown Address";
      if (addr) {
        snprintf(road_str, sizeof(road_str), "%s, %s, %s", addr->street, addr->city, addr->state);
      }

      clear_screen();
      printf("===========================================\n");
      printf("        PARCEL CREATED SUCCESSFULLY!       \n");
      printf("===========================================\n");
      printf("New Parcel Details:\n");
      printf("  Parcel ID     : %d\n", p.parcel_id);
      printf("  Sender Name   : %s\n", p.sender_name);
      printf("  Receiver Name : %s\n", p.receiver_name);
      printf("  House Number  : %d\n", p.house_number);
      printf("  Delivery Type : %s\n", p.delivery_type);
      printf("  Status        : %s\n", p.status);
      printf("  Address Road  : %s\n", road_str);
      printf("===========================================\n");
      print_barcode(p.parcel_id);
      printf("===========================================\n");
    } break;
    case 3: {
      clear_screen();
      int s = get_validated_choice("=== SEARCH ===\n1. By ID\n2. By Receiver\nEnter Choice: ", 1, 2);
      ParcelNode *f = NULL;
      if (s == 1) {
        int search_id = 0;
        get_validated_int_id("ID: ", 0, &search_id);
        f = search_by_id(*head, search_id);
        if (f) {
          clear_screen();
          printf("===========================================\n");
          printf("             PARCEL SEARCH RESULT          \n");
          printf("===========================================\n");
          printf("Details:\n");
          printf("  Parcel ID     : %d\n", f->data.parcel_id);
          printf("  Sender Name   : %s\n", f->data.sender_name);
          printf("  Receiver Name : %s\n", f->data.receiver_name);
          printf("  House Number  : %d\n", f->data.house_number);
          printf("  Delivery Type : %s\n", f->data.delivery_type);
          printf("  Status        : %s\n", f->data.status);
          
          Address *addr = find_address(addresses, addr_count, f->data.address_id);
          char road_str[150] = "Unknown Address";
          if (addr) {
            snprintf(road_str, sizeof(road_str), "%s, %s, %s", addr->street, addr->city, addr->state);
          }
          printf("  Address Road  : %s\n", road_str);
          printf("===========================================\n");
          print_barcode(f->data.parcel_id);
          printf("===========================================\n");
        } else {
          printf("Not found.\n");
        }
      } else {
        char search_name[30];
        get_validated_string("Name: ", search_name, 30, 1, 29, 0, 0);
        f = search_by_receiver(*head, search_name);
        if (f) {
          display_all_parcels(f, addresses, addr_count, users, user_count);
        } else {
          printf("Not found.\n");
        }
      }
    } break;
    case 4: {
      ParcelNode *n = NULL;
      int id = 0;
      while (1) {
        clear_screen();
        printf("--- Current Active Parcels (Sorted) ---\n");
        display_active_parcels(*head, addresses, addr_count, users, user_count);
        printf("\n");

        if (!get_validated_int_id("Enter Parcel ID to Update (or press Enter/0 to cancel): ", 1, &id) || id == 0) {
          n = NULL;
          break;
        }
        n = find_parcel(*head, id);
        if (n != NULL) {
          if (strcmp(n->data.status, "Delivered") == 0) {
            printf("Error: This parcel has already been delivered and cannot be updated!\n");
            n = NULL;
            printf("Press Enter to try again...\n");
            get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
            continue;
          }
          break;
        }
        printf("Error: Parcel ID not found. Press Enter to try again...\n");
        get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
      }
      if (n == NULL) break;

      char st[20] = "";
      while (1) {
        clear_screen();
        printf("=== UPDATE PARCEL STATUS (ID: %d) ===\n", id);
        display_dynamic_status_options(n->data.status);
        
        int choice_limit = strcmp(n->data.status, "Pending") == 0 ? 2 : 1;
        
        get_validated_string("\nEnter Choice (or press Enter/0 to cancel): ", buffer, sizeof(buffer), 0, 10, 0, 1);
        if (strlen(buffer) == 0 || strcmp(buffer, "0") == 0) {
          printf("Update cancelled.\n");
          break;
        }
        
        if (!validate_integer(buffer)) {
          printf("Invalid choice! Choose a valid number or 0 to cancel. Press Enter to try again...\n");
          get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
          continue;
        }
        
        int c = atoi(buffer);
        if (c < 1 || c > choice_limit) {
          printf("Invalid choice! Choose a number between 1 and %d or 0 to cancel. Press Enter to try again...\n", choice_limit);
          get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
          continue;
        }

        if (strcmp(n->data.status, "Pending") == 0) {
          if (c == 1) strcpy(st, "Out for Delivery");
          else if (c == 2) strcpy(st, "Delivered");
        } else if (strcmp(n->data.status, "Out for Delivery") == 0) {
          if (c == 1) strcpy(st, "Delivered");
        }

        if (strcmp(st, "Delivered") == 0) {
          int assigned_rider_id = 0;
          for (int i = 0; i < user_count; i++) {
            if (users[i].role == ROLE_RIDER && users[i].assigned_address_id == n->data.address_id) {
              assigned_rider_id = users[i].user_id;
              break;
            }
          }
          n->data.rider_id = assigned_rider_id;
        }

        int update_res = update_parcel_status(head, id, st);
        if (update_res == 1) {
          save_parcels_to_file(*head, "dataset/parcels.csv");
          printf("Updated and re-sorted!\n");
          break;
        } else {
          printf("Invalid transition. Press Enter to try again...\n");
          get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
        }
      }
    } break;
    case 5: {
      while (1) {
        clear_screen();
        printf("--- All Parcels in System ---\n");
        display_all_parcels(*head, addresses, addr_count, users, user_count);
        printf("\n");
        
        int id = 0;
        if (!get_validated_int_id("Enter Parcel ID to Delete (or press Enter/0 to cancel): ", 1, &id) || id == 0) {
          break;
        }
        if (delete_parcel(head, id)) {
          save_parcels_to_file(*head, "dataset/parcels.csv");
          printf("Deleted successfully.\n");
          break;
        } else {
          printf("Error: Parcel not found. Press Enter to try again...\n");
          get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
        }
      }
      break;
    }
    case 6:
      return;
    }
    printf("\nPress Enter to continue...");
    get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
  } while (choice != 6);
}

// --- SUB-MENU: ADDRESS MANAGEMENT ---
void show_address_menu(Address addresses[], int *addr_count, User users[], int user_count) {
  int choice;
  char buffer[50];
  do {
    clear_screen();
    printf("=== ADDRESS MANAGEMENT ===\n");
    printf("1. View All Addresses\n");
    printf("2. Add New Address\n");
    printf("3. Update Address\n");
    printf("4. Return to Main Menu\n");
    printf("Enter Choice: ");

    choice = get_validated_choice("Enter Choice: ", 1, 4);

    switch (choice) {
    case 1:
      clear_screen();
      display_address_list_with_riders(addresses, *addr_count, users, user_count);
      break;
    case 2: {
      Address a = {0};
      get_validated_string("Street (e.g. Jalan Ampang / Taman Putri Kulai): ", a.street, 100, 1, 99, 0, 0);
      get_validated_string("City (e.g. Kuala Lumpur / Kulai): ", a.city, 50, 1, 49, 0, 0);
      get_validated_string("State (e.g. WP Kuala Lumpur / Johor): ", a.state, 50, 1, 49, 0, 0);
      if (add_address(addresses, addr_count, a)) {
        save_addresses_to_file(addresses, *addr_count, "dataset/addresses.csv");
        clear_screen();
        printf("===========================================\n");
        printf("        ADDRESS CREATED SUCCESSFULLY!      \n");
        printf("===========================================\n");
        printf("New Address Details:\n");
        printf("  Address ID : %d\n", addresses[*addr_count - 1].address_id);
        printf("  Street     : %s\n", addresses[*addr_count - 1].street);
        printf("  City       : %s\n", addresses[*addr_count - 1].city);
        printf("  State      : %s\n", addresses[*addr_count - 1].state);
        printf("===========================================\n");
      }
      break;
    }
    case 3: {
      while (1) {
        clear_screen();
        printf("--- Current Addresses ---\n");
        display_address_list_with_riders(addresses, *addr_count, users, user_count);
        printf("\n");
        int id = 0;
        if (!get_validated_int_id("ID to Update (or press Enter/0 to cancel): ", 1, &id) || id == 0) {
          break;
        }
        if (update_address(addresses, *addr_count, id)) {
          save_addresses_to_file(addresses, *addr_count, "dataset/addresses.csv");
          printf("Updated successfully.\n");
          break;
        } else {
          printf("Error: Address ID not found. Press Enter to try again...\n");
          get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
        }
      }
      break;
    }
    case 4:
      return;
    }
    printf("\nPress Enter to continue...");
    get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
  } while (choice != 4);
}

// --- SUB-MENU: REPORTS ---
void show_report_menu(ParcelNode *head, Address addresses[], int addr_count, User users[], int user_count) {
  int choice;
  char buffer[10];
  do {
    clear_screen();
    printf("=== REPORTS & ANALYTICS ===\n");
    printf("1. Admin Summary Report\n");
    printf("2. View Delivery Queue (Non-Delivered)\n");
    printf("3. Rider Commission Report\n");
    printf("4. Return to Main Menu\n");
    choice = get_validated_choice("Enter Choice: ", 1, 4);

    switch (choice) {
    case 1:
      clear_screen();
      display_admin_report(head, addresses, addr_count, users, user_count);
      break;
    case 2: {
      clear_screen();
      ParcelNode *sorted = generate_sorted_queue(head);
      display_sorted_parcels(sorted, addresses, addr_count, users, user_count);
      free_sorted_queue(&sorted);
    } break;
    case 3:
      clear_screen();
      display_all_riders_commission_report(head, users, user_count, addresses, addr_count);
      break;
    case 4:
      return;
    }
    printf("\nPress Enter to continue...");
    get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
  } while (choice != 4);
}

// --- MAIN ADMIN MENU ---
void show_admin_main_menu(ParcelNode **head, User users[], int *user_count,
                          Address addresses[], int *addr_count) {
  int choice;
  do {
    clear_screen();
    printf("=== ADMIN MAIN MENU ===\n");
    printf("1. Users Management\n");
    printf("2. Parcel Management\n");
    printf("3. Address Management\n");
    printf("4. Reports & Analytics\n");
    printf("5. Logout\n");
    choice = get_validated_choice("Enter Choice: ", 1, 5);

    switch (choice) {
    case 1:
      show_user_menu(users, user_count, addresses, *addr_count);
      break;
    case 2:
      show_parcel_menu(head, addresses, *addr_count, users, *user_count);
      break;
    case 3:
      show_address_menu(addresses, addr_count, users, *user_count);
      break;
    case 4:
      show_report_menu(*head, addresses, *addr_count, users, *user_count);
      break;
    case 5:
      printf("Logging out...\n");
      return;
    }
  } while (choice != 5);
}

void show_rider_menu(ParcelNode **head, int user_idx, User users[], int user_count,
                     Address addresses[], int addr_count) {
  int choice;
  char buffer[10];

  int assigned_road_id = users[user_idx].assigned_address_id;
  Address *addr = find_address(addresses, addr_count, assigned_road_id);
  char road_name[100];
  if (addr) {
    snprintf(road_name, sizeof(road_name), "%s (%s)", addr->street, addr->city);
  } else {
    snprintf(road_name, sizeof(road_name), "Unknown Road ID: %d",
             assigned_road_id);
  }

  do {
    clear_screen();
    printf("=== RIDER MENU (User: %s) ===\n", users[user_idx].username);
    printf("Assigned Road: %s\n", road_name);
    printf("----------------------------------------\n");
    printf("1. View My Assigned Road Parcels\n");
    printf("2. View All Sorted Parcels\n");
    printf("3. Update Parcel Status\n");
    printf("4. View My Commission Report\n");
    printf("5. Logout\n");
    choice = get_validated_choice("Enter Choice: ", 1, 5);

    switch (choice) {
    case 1:
      clear_screen();
      display_rider_parcels(*head, assigned_road_id, addresses, addr_count, users, user_count);
      break;
    case 2: {
      clear_screen();
      ParcelNode *sorted = generate_sorted_queue(*head);
      display_rider_sorted_parcels(sorted, assigned_road_id, addresses, addr_count, users, user_count);
      free_sorted_queue(&sorted);
    } break;
    case 3: {
      ParcelNode *n = NULL;
      int id = 0;
      while (1) {
        clear_screen();
        printf("--- Your Assigned Active Road Parcels ---\n");
        display_rider_active_parcels(*head, assigned_road_id, addresses, addr_count, users, user_count);
        printf("\n");

        if (!get_validated_int_id("Parcel ID to Update (or press Enter/0 to cancel): ", 1, &id) || id == 0) {
          n = NULL;
          break;
        }
        n = find_parcel(*head, id);
        if (n != NULL) {
          if (n->data.address_id != assigned_road_id) {
            printf("Error: Access Denied. You can only update parcels assigned to your road!\n");
            n = NULL;
            printf("Press Enter to try again...\n");
            get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
            continue;
          }
          if (strcmp(n->data.status, "Delivered") == 0) {
            printf("Error: This parcel has already been delivered and cannot be updated!\n");
            n = NULL;
            printf("Press Enter to try again...\n");
            get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
            continue;
          }
          break; // Valid road match and not delivered!
        }
        printf("Error: Parcel ID not found. Press Enter to try again...\n");
        get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
      }
      if (n == NULL) break;

      char st[20] = "";
      while (1) {
        clear_screen();
        printf("=== UPDATE PARCEL STATUS (ID: %d) ===\n", id);
        display_dynamic_status_options(n->data.status);
        
        int choice_limit = strcmp(n->data.status, "Pending") == 0 ? 2 : 1;
        
        get_validated_string("\nEnter Choice (or press Enter/0 to cancel): ", buffer, sizeof(buffer), 0, 10, 0, 1);
        if (strlen(buffer) == 0 || strcmp(buffer, "0") == 0) {
          printf("Update cancelled.\n");
          break;
        }
        
        if (!validate_integer(buffer)) {
          printf("Invalid choice! Choose a valid number or 0 to cancel. Press Enter to try again...\n");
          get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
          continue;
        }
        
        int c = atoi(buffer);
        if (c < 1 || c > choice_limit) {
          printf("Invalid choice! Choose a number between 1 and %d or 0 to cancel. Press Enter to try again...\n", choice_limit);
          get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
          continue;
        }

        if (strcmp(n->data.status, "Pending") == 0) {
          if (c == 1) strcpy(st, "Out for Delivery");
          else if (c == 2) strcpy(st, "Delivered");
        } else if (strcmp(n->data.status, "Out for Delivery") == 0) {
          if (c == 1) strcpy(st, "Delivered");
        }

        if (strcmp(st, "Delivered") == 0) {
          n->data.rider_id = users[user_idx].user_id;
        }

        int update_res = update_parcel_status(head, id, st);
        if (update_res == 1) {
          save_parcels_to_file(*head, "dataset/parcels.csv");
          printf("Updated and re-sorted!\n");
          break;
        } else {
          printf("Invalid transition. Press Enter to try again...\n");
          get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
        }
      }
    } break;
    case 4:
      clear_screen();
      display_rider_commission(*head, users, user_count, addresses, addr_count, user_idx);
      break;
    case 5:
      return;
    }
    printf("\nPress Enter to continue...");
    get_validated_string("", buffer, sizeof(buffer), 0, sizeof(buffer)-1, 0, 1);
  } while (choice != 5);
}

int main() {
  ParcelNode *head = NULL;
  User users[MAX_USERS];
  Address addresses[MAX_ADDRESSES];
  int user_count = 0;
  int addr_count = 0;

  // Try to load from CSV dataset
  user_count = load_users(users, MAX_USERS, "dataset/users.csv");
  addr_count = load_addresses(addresses, MAX_ADDRESSES, "dataset/addresses.csv");
  int parcel_count = load_parcels_from_file(&head, "dataset/parcels.csv");

  if (user_count == 0 || addr_count == 0) {
    printf("CSV database not found or empty. Initializing with mock database...\n");
    free_all_parcels(&head);
    head = NULL;
    init_mock_database(&head, users, &user_count, addresses, &addr_count);
    
    // Save to establish baseline files
    save_users_to_file(users, user_count, "dataset/users.csv");
    save_addresses_to_file(addresses, addr_count, "dataset/addresses.csv");
    save_parcels_to_file(head, "dataset/parcels.csv");

    printf("\nPress Enter to start...");
    char temp[10];
    get_validated_string("", temp, sizeof(temp), 0, sizeof(temp)-1, 0, 1);
  } else {
    printf("Successfully loaded database from CSV files:\n");
    printf("  - Users: %d\n", user_count);
    printf("  - Addresses: %d\n", addr_count);
    printf("  - Parcels: %d\n", parcel_count);
    printf("\nPress Enter to start...");
    char temp[10];
    get_validated_string("", temp, sizeof(temp), 0, sizeof(temp)-1, 0, 1);
  }

  while (1) {
    int user_idx = login(users, user_count);
    if (user_idx == ROLE_INVALID)
      break;

    int role = users[user_idx].role;
    if (role == ROLE_ADMIN) {
      show_admin_main_menu(&head, users, &user_count, addresses, &addr_count);
    } else if (role == ROLE_RIDER) {
      show_rider_menu(&head, user_idx, users, user_count, addresses, addr_count);
    }
  }

  // Final backup save on clean exit
  save_users_to_file(users, user_count, "dataset/users.csv");
  save_addresses_to_file(addresses, addr_count, "dataset/addresses.csv");
  save_parcels_to_file(head, "dataset/parcels.csv");

  free_all_parcels(&head);
  printf("Goodbye!\n");
  return 0;
}