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
    printf("Enter Choice: ");

    safe_read_string(buffer, sizeof(buffer));
    choice = atoi(buffer);

    switch (choice) {
    case 1:
      clear_screen();
      printf("--- All Users ---\n");
      display_user_list(users, *user_count, addresses, addr_count);
      break;
    case 2: {
      char uname[30], pwd[30];
      int username_ok = 0;
      while (1) {
        printf("Enter New Rider Username (or press Enter to cancel): ");
        safe_read_string(uname, 30);
        trim_whitespace(uname);
        if (strlen(uname) == 0) {
          username_ok = -1; // Cancelled
          break;
        }

        // Check if username already exists
        int exists = 0;
        for (int i = 0; i < *user_count; i++) {
          if (strcmp(users[i].username, uname) == 0) {
            exists = 1;
            break;
          }
        }

        if (!exists) {
          username_ok = 1;
          break; // Username is unique!
        }
        printf("Error: Username already exists. Please try a different username.\n\n");
      }
      if (username_ok == -1) break;

      printf("Enter Password: ");
      safe_read_string(pwd, 30);

      // Force a valid road assignment (mandatory for Riders)
      int assigned_id = 0;
      while (1) {
        clear_screen();
        printf("=== ASSIGN ROAD FOR RIDER ===\n");
        display_address_list(addresses, addr_count);
        printf(
            "Enter Address ID to assign (Must be a valid ID from the list): ");
        safe_read_string(buffer, sizeof(buffer));
        assigned_id = atoi(buffer);

        if (find_address(addresses, addr_count, assigned_id) != NULL) {
          break; // Valid road selected
        }
        printf("Invalid Address ID! Riders must have a valid road assigned. "
               "Press Enter to try again...\n");
        safe_read_string(buffer, sizeof(buffer));
      }

      int res = register_rider(users, user_count, uname, pwd, assigned_id);
      if (res == 1) {
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
        printf("\nEnter User ID to Update (or press Enter/0 to cancel): ");
        safe_read_string(buffer, sizeof(buffer));
        if (strlen(buffer) == 0 || atoi(buffer) == 0) {
          target_idx = -2;
          break;
        }
        target_id = atoi(buffer);
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
        safe_read_string(buffer, sizeof(buffer));
      }
      if (target_idx == -2 || target_idx == -1) break;

      char new_uname[30];
      printf("Enter New Username (Current: %s, leave empty to keep): ",
             users[target_idx].username);
      safe_read_string(new_uname, 30);
      trim_whitespace(new_uname);

      int new_role = -1;
      while (1) {
        printf("Enter New Role (0: Admin, 1: Rider, or press Enter to keep current %s): ",
               (users[target_idx].role == ROLE_ADMIN) ? "Admin" : "Rider");
        safe_read_string(buffer, sizeof(buffer));
        trim_whitespace(buffer);
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

      if (final_role == ROLE_RIDER) {
        while (1) {
          clear_screen();
          printf("=== UPDATE ROAD ASSIGNMENT FOR RIDER ===\n");
          display_address_list(addresses, addr_count);
          printf("Enter Address ID to assign (Must be a valid ID from the list, or press Enter to keep current): ");
          safe_read_string(buffer, sizeof(buffer));
          
          if (strlen(buffer) == 0) {
            if (users[target_idx].assigned_address_id > 0) {
              new_assigned_id = users[target_idx].assigned_address_id;
              break;
            } else {
              printf("This user is transitioning to a Rider role and has no current road assignment. You must assign a valid road ID!\n");
              printf("Press Enter to try again...\n");
              safe_read_string(buffer, sizeof(buffer));
              continue;
            }
          }

          new_assigned_id = atoi(buffer);

          if (find_address(addresses, addr_count, new_assigned_id) != NULL) {
            break; // Valid road
          }
          printf("Invalid Address ID! Riders must have a valid road assigned. "
                 "Press Enter to try again...\n");
          safe_read_string(buffer, sizeof(buffer));
        }
      }

      int update_res = update_user(users, *user_count, target_id, new_uname,
                                   new_role, new_assigned_id);
      if (update_res == 1) {
        printf("User updated successfully!\n");
      } else if (update_res == -1) {
        printf("Error: Username conflict (new username already taken).\n");
      } else if (update_res == -2) {
        printf("Error: Invalid road assignment for Rider.\n");
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
        printf("\nEnter User ID to Delete (or press Enter/0 to cancel): ");
        safe_read_string(buffer, sizeof(buffer));
        if (strlen(buffer) == 0 || atoi(buffer) == 0) {
          res = -999;
          break;
        }
        target_id = atoi(buffer);
        res = delete_user(users, user_count, target_id);
        if (res == 1) {
          printf("User deleted successfully.\n");
          break;
        } else if (res == -2) {
          printf("Error: Cannot delete Admin. Press Enter to try again...\n");
          safe_read_string(buffer, sizeof(buffer));
        } else {
          printf("Error: User ID not found. Press Enter to try again...\n");
          safe_read_string(buffer, sizeof(buffer));
        }
      }
      break;
    }
    case 5:
      return;
    }
    printf("\nPress Enter to continue...");
    safe_read_string(buffer, sizeof(buffer));
  } while (choice != 5);
}

// --- SUB-MENU: PARCEL MANAGEMENT ---
void show_parcel_menu(ParcelNode **head, Address addresses[], int addr_count) {
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
    printf("Enter Choice: ");

    safe_read_string(buffer, sizeof(buffer));
    choice = atoi(buffer);

    switch (choice) {
    case 1:
      clear_screen();
      printf("--- All Parcels (Prioritized) ---\n");
      display_all_parcels(*head, addresses, addr_count);
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
      printf("Sender: ");
      safe_read_string(p.sender_name, 50);
      trim_whitespace(p.sender_name);
      printf("Receiver: ");
      safe_read_string(p.receiver_name, 50);
      trim_whitespace(p.receiver_name);
      while (1) {
        display_address_list(addresses, addr_count);
        printf("Enter Address ID (Must be a valid ID from the list): ");
        safe_read_string(buffer, 10);
        p.address_id = atoi(buffer);
        if (find_address(addresses, addr_count, p.address_id) != NULL) {
          break;
        }
        printf("Invalid Address ID! Press Enter to try again...\n");
        safe_read_string(buffer, sizeof(buffer));
      }
      printf("House #: ");
      safe_read_string(buffer, 10);
      p.house_number = atoi(buffer);
      printf("Type (1:Fast, 2:Standard): ");
      safe_read_string(buffer, 10);
      if (atoi(buffer) == 1)
        strcpy(p.delivery_type, "Fast");
      else
        strcpy(p.delivery_type, "Standard");
      strcpy(p.status, "Pending");
      get_current_time(p.time_in, 20);
      p.rider_id = 0;
      insert_parcel(head, p);
      printf("\nParcel created and auto-sorted!\n");
    } break;
    case 3: {
      clear_screen();
      printf("=== SEARCH ===\n1. By ID\n2. By Receiver\nEnter: ");
      safe_read_string(buffer, 10);
      int s = atoi(buffer);
      ParcelNode *f = NULL;
      if (s == 1) {
        printf("ID: ");
        safe_read_string(buffer, 10);
        f = search_by_id(*head, atoi(buffer));
      } else {
        printf("Name: ");
        safe_read_string(buffer, 30);
        f = search_by_receiver(*head, buffer);
      }
      if (f)
        display_all_parcels(f, addresses, addr_count);
      else
        printf("Not found.\n");
    } break;
    case 4: {
      ParcelNode *n = NULL;
      int id = 0;
      while (1) {
        clear_screen();
        printf("--- Current Active Parcels (Sorted) ---\n");
        display_active_parcels(*head, addresses, addr_count);
        printf("\n");

        printf("Enter Parcel ID to Update (or press Enter/0 to cancel): ");
        safe_read_string(buffer, 10);
        if (strlen(buffer) == 0 || atoi(buffer) == 0) {
          n = NULL;
          break;
        }
        id = atoi(buffer);
        n = find_parcel(*head, id);
        if (n != NULL) {
          if (strcmp(n->data.status, "Delivered") == 0) {
            printf("Error: This parcel has already been delivered and cannot be updated!\n");
            n = NULL;
            printf("Press Enter to try again...\n");
            safe_read_string(buffer, sizeof(buffer));
            continue;
          }
          break;
        }
        printf("Error: Parcel ID not found. Press Enter to try again...\n");
        safe_read_string(buffer, sizeof(buffer));
      }
      if (n == NULL) break;

      display_status_options();
      char st[20];
      while (1) {
        printf("New Status (1-3): ");
        safe_read_string(buffer, 10);
        int c = atoi(buffer);
        if (c == 1) strcpy(st, "Pending");
        else if (c == 2) strcpy(st, "Out for Delivery");
        else if (c == 3) strcpy(st, "Delivered");
        else {
          printf("Invalid choice! Choose 1, 2, or 3.\n");
          continue;
        }
        
        int update_res = update_parcel_status(head, id, st);
        if (update_res == 1) {
          printf("Updated and re-sorted!\n");
          break;
        } else {
          printf("Invalid transition (e.g. cannot go from Out for Delivery back to Pending). Press Enter to try again...\n");
          safe_read_string(buffer, sizeof(buffer));
        }
      }
    } break;
    case 5: {
      while (1) {
        printf("Enter Parcel ID to Delete (or press Enter/0 to cancel): ");
        safe_read_string(buffer, 10);
        if (strlen(buffer) == 0 || atoi(buffer) == 0) {
          break;
        }
        int id = atoi(buffer);
        if (delete_parcel(head, id)) {
          printf("Deleted successfully.\n");
          break;
        } else {
          printf("Error: Parcel not found. Press Enter to try again...\n");
          safe_read_string(buffer, sizeof(buffer));
        }
      }
      break;
    }
    case 6:
      return;
    }
    printf("\nPress Enter to continue...");
    safe_read_string(buffer, sizeof(buffer));
  } while (choice != 6);
}

// --- SUB-MENU: ADDRESS MANAGEMENT ---
void show_address_menu(Address addresses[], int *addr_count) {
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

    safe_read_string(buffer, sizeof(buffer));
    choice = atoi(buffer);

    switch (choice) {
    case 1:
      clear_screen();
      display_address_list(addresses, *addr_count);
      break;
    case 2: {
      Address a = {0};
      printf("Street: ");
      safe_read_string(a.street, 100);
      trim_whitespace(a.street);
      printf("City: ");
      safe_read_string(a.city, 50);
      trim_whitespace(a.city);
      printf("State: ");
      safe_read_string(a.state, 50);
      trim_whitespace(a.state);
      if (add_address(addresses, addr_count, a))
        printf("Added.\n");
      break;
    }
    case 3: {
      while (1) {
        printf("ID to Update (or press Enter/0 to cancel): ");
        safe_read_string(buffer, 10);
        if (strlen(buffer) == 0 || atoi(buffer) == 0) {
          break;
        }
        int id = atoi(buffer);
        if (update_address(addresses, *addr_count, id)) {
          printf("Updated successfully.\n");
          break;
        } else {
          printf("Error: Address ID not found. Press Enter to try again...\n");
          safe_read_string(buffer, sizeof(buffer));
        }
      }
      break;
    }
    case 4:
      return;
    }
    printf("\nPress Enter to continue...");
    safe_read_string(buffer, sizeof(buffer));
  } while (choice != 4);
}

// --- SUB-MENU: REPORTS ---
void show_report_menu(ParcelNode *head, Address addresses[], int addr_count) {
  int choice;
  char buffer[10];
  do {
    clear_screen();
    printf("=== REPORTS & ANALYTICS ===\n");
    printf("1. Admin Summary Report\n");
    printf("2. View Delivery Queue (Non-Delivered)\n");
    printf("3. Return to Main Menu\n");
    printf("Enter Choice: ");

    safe_read_string(buffer, sizeof(buffer));
    choice = atoi(buffer);

    switch (choice) {
    case 1:
      clear_screen();
      display_admin_report(head, addresses, addr_count);
      break;
    case 2: {
      clear_screen();
      display_sorted_parcels(head, addresses, addr_count);
    } break;
    case 3:
      return;
    }
    printf("\nPress Enter to continue...");
    safe_read_string(buffer, sizeof(buffer));
  } while (choice != 3);
}

// --- MAIN ADMIN MENU ---
void show_admin_main_menu(ParcelNode **head, User users[], int *user_count,
                          Address addresses[], int *addr_count) {
  int choice;
  char buffer[10];
  do {
    clear_screen();
    printf("=== ADMIN MAIN MENU ===\n");
    printf("1. Users Management\n");
    printf("2. Parcel Management\n");
    printf("3. Address Management\n");
    printf("4. Reports & Analytics\n");
    printf("5. Logout\n");
    printf("Enter Choice: ");

    safe_read_string(buffer, sizeof(buffer));
    choice = atoi(buffer);

    switch (choice) {
    case 1:
      show_user_menu(users, user_count, addresses, *addr_count);
      break;
    case 2:
      show_parcel_menu(head, addresses, *addr_count);
      break;
    case 3:
      show_address_menu(addresses, addr_count);
      break;
    case 4:
      show_report_menu(*head, addresses, *addr_count);
      break;
    case 5:
      printf("Logging out...\n");
      return;
    }
  } while (choice != 5);
}

void show_rider_menu(ParcelNode **head, int user_idx, User users[],
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
    printf("4. Logout\n");
    printf("Enter Choice: ");

    safe_read_string(buffer, sizeof(buffer));
    choice = atoi(buffer);

    switch (choice) {
    case 1:
      clear_screen();
      display_rider_parcels(*head, assigned_road_id, addresses, addr_count);
      break;
    case 2:
      clear_screen();
      display_rider_sorted_parcels(*head, assigned_road_id, addresses, addr_count);
      break;
    case 3: {
      ParcelNode *n = NULL;
      int id = 0;
      while (1) {
        clear_screen();
        printf("--- Your Assigned Active Road Parcels ---\n");
        display_rider_active_parcels(*head, assigned_road_id, addresses, addr_count);
        printf("\n");

        printf("Parcel ID to Update (or press Enter/0 to cancel): ");
        safe_read_string(buffer, 10);
        if (strlen(buffer) == 0 || atoi(buffer) == 0) {
          n = NULL;
          break;
        }
        id = atoi(buffer);
        n = find_parcel(*head, id);
        if (n != NULL) {
          if (n->data.address_id != assigned_road_id) {
            printf("Error: Access Denied. You can only update parcels assigned to your road!\n");
            n = NULL;
            printf("Press Enter to try again...\n");
            safe_read_string(buffer, sizeof(buffer));
            continue;
          }
          if (strcmp(n->data.status, "Delivered") == 0) {
            printf("Error: This parcel has already been delivered and cannot be updated!\n");
            n = NULL;
            printf("Press Enter to try again...\n");
            safe_read_string(buffer, sizeof(buffer));
            continue;
          }
          break; // Valid road match and not delivered!
        }
        printf("Error: Parcel ID not found. Press Enter to try again...\n");
        safe_read_string(buffer, sizeof(buffer));
      }
      if (n == NULL) break;

      display_status_options();
      char st[20];
      while (1) {
        printf("Choice (2-3): ");
        safe_read_string(buffer, 10);
        int c = atoi(buffer);
        if (c == 2) strcpy(st, "Out for Delivery");
        else if (c == 3) strcpy(st, "Delivered");
        else {
          printf("Invalid choice! Choose 2 or 3.\n");
          continue;
        }
        
        int update_res = update_parcel_status(head, id, st);
        if (update_res == 1) {
          printf("Updated and re-sorted!\n");
          break;
        } else {
          printf("Invalid status transition (e.g. cannot go backwards or set Pending). Press Enter to try again...\n");
          safe_read_string(buffer, sizeof(buffer));
        }
      }
    } break;
    case 4:
      return;
    }
    printf("\nPress Enter to continue...");
    safe_read_string(buffer, sizeof(buffer));
  } while (choice != 4);
}

int main() {
  ParcelNode *head = NULL;
  User users[MAX_USERS];
  Address addresses[MAX_ADDRESSES];
  int user_count = 0;
  int addr_count = 0;

  init_mock_database(&head, users, &user_count, addresses, &addr_count);

  while (1) {
    int user_idx = login(users, user_count);
    if (user_idx == ROLE_INVALID)
      break;

    int role = users[user_idx].role;
    if (role == ROLE_ADMIN) {
      show_admin_main_menu(&head, users, &user_count, addresses, &addr_count);
    } else if (role == ROLE_RIDER) {
      show_rider_menu(&head, user_idx, users, addresses, addr_count);
    }
  }

  free_all_parcels(&head);
  printf("Goodbye!\n");
  return 0;
}