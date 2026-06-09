#include <stdio.h>
#include <string.h>
#include "../include/output.h"

/**
 * @brief Prints a standard horizontal divider line for formatted tables.
 */
void print_divider(void) {
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");
}

/**
 * @brief Prints the column headers for the parcel listings table.
 */
void print_table_header(void) {
    print_divider();
    printf("%-4s | %-15s | %-15s | %-8s | %-12s | %-6s | %-20s | %-15s | %-15s | %-15s\n",
           "ID", "Sender", "Receiver", "Type", "Status", "House#", "Street", "City", "State", "Rider");
    print_divider();
}

/**
 * @brief Prints a single row of parcel details formatted to align with the table headers.
 * 
 * Resolves address road details and rider username from corresponding IDs.
 * 
 * @param parcel The Parcel structure to print.
 * @param addresses Array of Address structures.
 * @param addr_count Current number of addresses.
 * @param users Array of User structures.
 * @param user_count Current number of users.
 */
void print_table_row(Parcel *parcel, Address addresses[], int addr_count, User users[], int user_count) {
    Address *addr = find_address(addresses, addr_count, parcel->address_id);
    char street[21] = "N/A", city[16] = "N/A", state[16] = "N/A";
    
    if (addr) {
        strncpy(street, addr->street, 20); street[20] = '\0';
        strncpy(city, addr->city, 15); city[15] = '\0';
        strncpy(state, addr->state, 15); state[15] = '\0';
    }

    const char *rider_uname = "Unassigned";
    if (parcel->rider_id > 0) {
        for (int i = 0; i < user_count; i++) {
            if (users[i].user_id == parcel->rider_id) {
                rider_uname = users[i].username;
                break;
            }
        }
    } else {
        for (int i = 0; i < user_count; i++) {
            if (users[i].role == ROLE_RIDER && users[i].assigned_address_id == parcel->address_id) {
                rider_uname = users[i].username;
                break;
            }
        }
    }

    printf("%-4d | %-15.15s | %-15.15s | %-8.8s | %-12.12s | %-6d | %-20.20s | %-15.15s | %-15.15s | %-15.15s\n",
           parcel->parcel_id,
           parcel->sender_name,
           parcel->receiver_name,
           parcel->delivery_type,
           parcel->status,
           parcel->house_number,
           street,
           city,
           state,
           rider_uname);
}

/**
 * @brief Displays all parcels starting from the specified head node in a tabular format.
 * 
 * @param head Head node of the parcel linked list.
 * @param addresses Array of Address structures.
 * @param addr_count Current number of addresses.
 * @param users Array of User structures.
 * @param user_count Current number of users.
 */
void display_all_parcels(ParcelNode *head, Address addresses[], int addr_count, User users[], int user_count) {
    if (head == NULL) {
        printf("No parcels in system.\n");
        return;
    }
    
    print_table_header();
    
    int count = 0;
    ParcelNode *current = head;
    while (current != NULL) {
        print_table_row(&current->data, addresses, addr_count, users, user_count);
        count++;
        current = current->next;
    }
    print_divider();
    printf("Total: %d parcels\n", count);
}

/**
 * @brief Displays a tabular list of parcels sorted by priority.
 * 
 * @param sorted_head Head node of the sorted parcel queue.
 * @param addresses Array of Address structures.
 * @param addr_count Current number of addresses.
 * @param users Array of User structures.
 * @param user_count Current number of users.
 */
void display_sorted_parcels(ParcelNode *sorted_head, Address addresses[], int addr_count, User users[], int user_count) {
    printf("=== SORTED DELIVERY QUEUE ===\n");
    if (sorted_head == NULL) {
        printf("No active parcels to sort.\n");
        return;
    }

    print_table_header();
    ParcelNode *current = sorted_head;
    while (current != NULL) {
        print_table_row(&current->data, addresses, addr_count, users, user_count);
        current = current->next;
    }
    print_divider();
}

/**
 * @brief Filters and displays parcels matching a specific status string in tabular format.
 * 
 * @param head Head node of the parcel linked list.
 * @param status Status filter query (e.g. "Pending").
 * @param addresses Array of Address structures.
 * @param addr_count Current number of addresses.
 * @param users Array of User structures.
 * @param user_count Current number of users.
 */
void display_parcels_by_status(ParcelNode *head, const char *status, Address addresses[], int addr_count, User users[], int user_count) {
    if (head == NULL) {
        printf("No parcels in system.\n");
        return;
    }
    
    print_table_header();
    int count = 0;
    ParcelNode *current = head;
    while (current != NULL) {
        if (strcmp(current->data.status, status) == 0) {
            print_table_row(&current->data, addresses, addr_count, users, user_count);
            count++;
        }
        current = current->next;
    }
    print_divider();
    printf("Total %s: %d parcels\n", status, count);
}

/**
 * @brief Prints an analytical summary containing total counts categorized by status and type.
 * 
 * Shows total parcels, pending counts, out-for-delivery counts, delivered counts, standard, and fast types.
 * 
 * @param head Head node of the parcel linked list.
 */
void display_summary(ParcelNode *head) {
    int total = 0, pending = 0, out = 0, delivered = 0, fast = 0, standard = 0;
    
    ParcelNode *current = head;
    while (current != NULL) {
        total++;
        if (strcmp(current->data.status, "Pending") == 0) pending++;
        else if (strcmp(current->data.status, "Out for Delivery") == 0) out++;
        else if (strcmp(current->data.status, "Delivered") == 0) delivered++;
        
        if (strcmp(current->data.delivery_type, "Fast") == 0) fast++;
        else if (strcmp(current->data.delivery_type, "Standard") == 0) standard++;
        
        current = current->next;
    }
    
    printf("\n+-------------------------+\n");
    printf("|   PARCEL SUMMARY        |\n");
    printf("+-------------------------+\n");
    printf("| Total:      %-11d |\n", total);
    printf("| Pending:    %-11d |\n", pending);
    printf("| Out:        %-11d |\n", out);
    printf("| Delivered:  %-11d |\n", delivered);
    printf("| Fast:       %-11d |\n", fast);
    printf("| Standard:   %-11d |\n", standard);
    printf("+-------------------------+\n");
}

/**
 * @brief Prints the master analytical summary and displays all system parcels in a table.
 * 
 * @param head Head node of the parcel linked list.
 * @param addresses Array of Address structures.
 * @param addr_count Current number of addresses.
 * @param users Array of User structures.
 * @param user_count Current number of users.
 */
void display_admin_report(ParcelNode *head, Address addresses[], int addr_count, User users[], int user_count) {
    display_summary(head);
    printf("\n");
    display_all_parcels(head, addresses, addr_count, users, user_count);
}

/**
 * @brief Displays all parcels assigned to a Rider's specific road assignment.
 * 
 * @param head Head node of the parcel linked list.
 * @param assigned_address_id Address ID of the Rider's assigned road.
 * @param addresses Array of Address structures.
 * @param addr_count Current number of addresses.
 * @param users Array of User structures.
 * @param user_count Current number of users.
 */
void display_rider_parcels(ParcelNode *head, int assigned_address_id, Address addresses[], int addr_count, User users[], int user_count) {
    int count = 0;
    print_table_header();
    
    ParcelNode *current = head;
    while (current != NULL) {
        if (current->data.address_id == assigned_address_id) {
            print_table_row(&current->data, addresses, addr_count, users, user_count);
            count++;
        }
        current = current->next;
    }
    print_divider();
    Address *addr = find_address(addresses, addr_count, assigned_address_id);
    printf("Total active parcels assigned to your road (%s): %d parcels\n", 
           addr ? addr->street : "Unknown", count);
}

/**
 * @brief Displays a sorted queue of active parcels that belong only to a Rider's assigned road.
 * 
 * @param sorted_head Head node of the sorted parcel queue.
 * @param assigned_address_id Address ID of the Rider's assigned road.
 * @param addresses Array of Address structures.
 * @param addr_count Current number of addresses.
 * @param users Array of User structures.
 * @param user_count Current number of users.
 */
void display_rider_sorted_parcels(ParcelNode *sorted_head, int assigned_address_id, Address addresses[], int addr_count, User users[], int user_count) {
    printf("=== SORTED DELIVERY QUEUE ===\n");
    if (sorted_head == NULL) {
        printf("No active parcels to sort.\n");
        return;
    }

    int has_displayed_any = 0;
    ParcelNode *current = sorted_head;
    while (current != NULL) {
        // Only process parcels that belong to this Rider's road!
        if (current->data.address_id == assigned_address_id) {
            if (!has_displayed_any) {
                print_table_header();
                has_displayed_any = 1;
            }
            print_table_row(&current->data, addresses, addr_count, users, user_count);
        }
        current = current->next;
    }
    if (has_displayed_any) {
        print_divider();
    } else {
        printf("No active sorted parcels assigned to your road.\n");
    }
}

/**
 * @brief Displays all active (non-delivered) parcels in the system.
 * 
 * @param head Head node of the parcel linked list.
 * @param addresses Array of Address structures.
 * @param addr_count Current number of addresses.
 * @param users Array of User structures.
 * @param user_count Current number of users.
 */
void display_active_parcels(ParcelNode *head, Address addresses[], int addr_count, User users[], int user_count) {
    if (head == NULL) {
        printf("No parcels in system.\n");
        return;
    }
    
    print_table_header();
    int count = 0;
    ParcelNode *current = head;
    while (current != NULL) {
        if (strcmp(current->data.status, "Delivered") != 0) {
            print_table_row(&current->data, addresses, addr_count, users, user_count);
            count++;
        }
        current = current->next;
    }
    print_divider();
    printf("Total Active (Non-Delivered) Parcels: %d\n", count);
}

/**
 * @brief Displays all active (non-delivered) parcels assigned to a Rider's specific road assignment.
 * 
 * @param head Head node of the parcel linked list.
 * @param assigned_address_id Address ID of the Rider's assigned road.
 * @param addresses Array of Address structures.
 * @param addr_count Current number of addresses.
 * @param users Array of User structures.
 * @param user_count Current number of users.
 */
void display_rider_active_parcels(ParcelNode *head, int assigned_address_id, Address addresses[], int addr_count, User users[], int user_count) {
    if (head == NULL) {
        printf("No parcels in system.\n");
        return;
    }
    
    print_table_header();
    int count = 0;
    ParcelNode *current = head;
    while (current != NULL) {
        if (current->data.address_id == assigned_address_id && strcmp(current->data.status, "Delivered") != 0) {
            print_table_row(&current->data, addresses, addr_count, users, user_count);
            count++;
        }
        current = current->next;
    }
    print_divider();
    Address *addr = find_address(addresses, addr_count, assigned_address_id);
    printf("Total active (non-delivered) parcels assigned to your road (%s): %d\n", 
           addr ? addr->street : "Unknown", count);
}

/**
 * @brief Generates and prints a pseudo-graphical barcode representation of a parcel ID.
 * 
 * Uses block and line symbols to represent distinct guard and digit patterns.
 * 
 * @param parcel_id The parcel ID to represent.
 */
void print_barcode(int parcel_id) {
    char code_str[10];
    snprintf(code_str, sizeof(code_str), "%04d", parcel_id);
    
    printf("  Barcode Label :\n");
    for (int line = 0; line < 3; line++) {
        printf("  ");
        // Start guard bars
        printf("█║│");
        for (int i = 0; i < 4; i++) {
            int val = code_str[i] - '0';
            // Print distinct thick/thin bars based on digit
            switch (val) {
                case 0: printf("█│║│"); break;
                case 1: printf("║█│║"); break;
                case 2: printf("│█║│"); break;
                case 3: printf("█║│█"); break;
                case 4: printf("║│█║"); break;
                case 5: printf("│║█│"); break;
                case 6: printf("█│█║"); break;
                case 7: printf("║█│█"); break;
                case 8: printf("│█│║"); break;
                case 9: printf("█║█│"); break;
                default: printf("││││"); break;
            }
        }
        // End guard bars
        printf("│║█\n");
    }
    printf("    *P-%04d*\n", parcel_id);
}

/**
 * @brief Displays commission report for a specific Rider.
 * 
 * Standard delivery: RM 3.00, Fast delivery: RM 6.00. Filters by Rider's assigned parcels
 * that have been successfully delivered.
 * 
 * @param head Head node of the parcel linked list.
 * @param users Array of User structures.
 * @param user_count Current number of users.
 * @param addresses Array of Address structures.
 * @param addr_count Current number of addresses.
 * @param rider_idx Array index of the Rider user.
 */
void display_rider_commission(ParcelNode *head, User users[], int user_count, Address addresses[], int addr_count, int rider_idx) {
    int std_count = 0;
    int fast_count = 0;
    User *rider = &users[rider_idx];
    
    printf("=== COMMISSION REPORT FOR RIDER: %s ===\n", rider->username);
    printf("Commission Rates: Standard = RM 3.00, Fast = RM 6.00\n\n");
    
    int has_any = 0;
    ParcelNode *current = head;
    while (current != NULL) {
        Parcel *p = &current->data;
        if (strcmp(p->status, "Delivered") == 0) {
            if (p->rider_id == rider->user_id || (p->rider_id == 0 && p->address_id == rider->assigned_address_id)) {
                if (!has_any) {
                    print_table_header();
                    has_any = 1;
                }
                print_table_row(p, addresses, addr_count, users, user_count);
                if (strcmp(p->delivery_type, "Fast") == 0) {
                    fast_count++;
                } else {
                    std_count++;
                }
            }
        }
        current = current->next;
    }
    
    if (has_any) {
        print_divider();
    } else {
        printf("No delivered parcels found for your assigned road.\n");
    }
    
    double std_pay = std_count * 3.00;
    double fast_pay = fast_count * 6.00;
    double total_pay = std_pay + fast_pay;
    
    printf("\n+---------------------------------------+\n");
    printf("|          COMMISSION SUMMARY           |\n");
    printf("+---------------------------------------+\n");
    printf("| Standard:   %-3d @ RM 3.00 = RM %-6.2f |\n", std_count, std_pay);
    printf("| Fast:       %-3d @ RM 6.00 = RM %-6.2f |\n", fast_count, fast_pay);
    printf("+---------------------------------------+\n");
    printf("| Total Commission Earned:   RM %-6.2f |\n", total_pay);
    printf("+---------------------------------------+\n");
}

/**
 * @brief Displays a system-wide commission report summarizing stats for all Riders.
 * 
 * Shows total Standard/Fast deliveries made and cumulative commission earned per Rider.
 * 
 * @param head Head node of the parcel linked list.
 * @param users Array of User structures.
 * @param user_count Current number of users.
 * @param addresses Array of Address structures.
 * @param addr_count Current number of addresses.
 */
void display_all_riders_commission_report(ParcelNode *head, User users[], int user_count, Address addresses[], int addr_count) {
    printf("=== SYSTEM-WIDE RIDER COMMISSION REPORT ===\n");
    printf("Commission Rates: Standard = RM 3.00, Fast = RM 6.00\n\n");
    
    printf("----------------------------------------------------------------------------------------------------\n");
    printf("%-6s | %-15s | %-25s | %-13s | %-14s | %-15s\n", 
           "ID", "Rider Username", "Assigned Road", "Std Delivered", "Fast Delivered", "Total Earned");
    printf("----------------------------------------------------------------------------------------------------\n");
    
    int total_std = 0;
    int total_fast = 0;
    double grand_total = 0.0;
    int rider_exists = 0;
    
    for (int i = 0; i < user_count; i++) {
        if (users[i].role != ROLE_RIDER) continue;
        
        rider_exists = 1;
        int std_count = 0;
        int fast_count = 0;
        
        ParcelNode *current = head;
        while (current != NULL) {
            Parcel *p = &current->data;
            if (strcmp(p->status, "Delivered") == 0) {
                if (p->rider_id == users[i].user_id || (p->rider_id == 0 && p->address_id == users[i].assigned_address_id)) {
                    if (strcmp(p->delivery_type, "Fast") == 0) {
                        fast_count++;
                    } else {
                        std_count++;
                    }
                }
            }
            current = current->next;
        }
        
        double total_pay = (std_count * 3.00) + (fast_count * 6.00);
        
        total_std += std_count;
        total_fast += fast_count;
        grand_total += total_pay;
        
        char road_str[50] = "N/A";
        Address *addr = find_address(addresses, addr_count, users[i].assigned_address_id);
        if (addr) {
            snprintf(road_str, sizeof(road_str), "%s (%s)", addr->street, addr->city);
        }
        
        printf("%-6d | %-15.15s | %-25.25s | %-13d | %-14d | RM %-11.2f\n",
               users[i].user_id,
               users[i].username,
               road_str,
               std_count,
               fast_count,
               total_pay);
    }
    
    if (!rider_exists) {
        printf("No riders registered in the system.\n");
    }
    printf("----------------------------------------------------------------------------------------------------\n");
    printf("Grand Totals: %-42s | Std: %-8d | Fast: %-9d | RM %-11.2f\n", 
           "", total_std, total_fast, grand_total);
    printf("----------------------------------------------------------------------------------------------------\n");
}

/**
 * @brief Displays a highly detailed formatted card representation of a single parcel.
 * 
 * Shows details such as parcel ID, sender name, receiver name, house number, delivery type,
 * status, address road, and its printed barcode.
 * 
 * @param parcel Pointer to the Parcel structure.
 * @param addresses Array of Address structures.
 * @param addr_count Current number of addresses.
 * @param users Array of User structures.
 * @param user_count Current number of users.
 */
void display_parcel_detail(Parcel *parcel, Address addresses[], int addr_count, User users[], int user_count) {
    if (parcel == NULL) return;
    (void)users;
    (void)user_count;

    printf("===========================================\n");
    printf("             PARCEL SEARCH RESULT          \n");
    printf("===========================================\n");
    printf("Details:\n");
    printf("  Parcel ID     : %d\n", parcel->parcel_id);
    printf("  Sender Name   : %s\n", parcel->sender_name);
    printf("  Receiver Name : %s\n", parcel->receiver_name);
    printf("  House Number  : %d\n", parcel->house_number);
    printf("  Delivery Type : %s\n", parcel->delivery_type);
    printf("  Status        : %s\n", parcel->status);
    
    Address *addr = find_address(addresses, addr_count, parcel->address_id);
    char road_str[150] = "Unknown Address";
    if (addr) {
        snprintf(road_str, sizeof(road_str), "%s, %s, %s", addr->street, addr->city, addr->state);
    }
    printf("  Address Road  : %s\n", road_str);
    printf("===========================================\n");
    print_barcode(parcel->parcel_id);
    printf("===========================================\n");
}

