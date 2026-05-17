#include <stdio.h>
#include <string.h>
#include "../include/output.h"

void print_divider(void) {
    printf("---------------------------------------------------------------------------------------------------------------------------\n");
}

void print_table_header(void) {
    print_divider();
    printf("%-4s | %-15s | %-8s | %-12s | %-6s | %-20s | %-15s | %-15s\n",
           "ID", "Receiver", "Type", "Status", "House#", "Street", "City", "State");
    print_divider();
}

void print_table_row(Parcel *parcel, Address addresses[], int addr_count) {
    Address *addr = find_address(addresses, addr_count, parcel->address_id);
    char street[21] = "N/A", city[16] = "N/A", state[16] = "N/A";
    
    if (addr) {
        strncpy(street, addr->street, 20); street[20] = '\0';
        strncpy(city, addr->city, 15); city[15] = '\0';
        strncpy(state, addr->state, 15); state[15] = '\0';
    }

    printf("%-4d | %-15.15s | %-8.8s | %-12.12s | %-6d | %-20.20s | %-15.15s | %-15.15s\n",
           parcel->parcel_id,
           parcel->receiver_name,
           parcel->delivery_type,
           parcel->status,
           parcel->house_number,
           street,
           city,
           state);
}

void display_all_parcels(ParcelNode *head, Address addresses[], int addr_count) {
    if (head == NULL) {
        printf("No parcels in system.\n");
        return;
    }
    
    print_table_header();
    
    int count = 0;
    ParcelNode *current = head;
    while (current != NULL) {
        print_table_row(&current->data, addresses, addr_count);
        count++;
        current = current->next;
    }
    print_divider();
    printf("Total: %d parcels\n", count);
}

void display_sorted_parcels(ParcelNode *sorted_head, Address addresses[], int addr_count) {
    printf("=== SORTED DELIVERY QUEUE ===\n");
    if (sorted_head == NULL) {
        printf("No active parcels to sort.\n");
        return;
    }

    int position = 1;
    char current_type[20] = "";

    ParcelNode *current = sorted_head;
    while (current != NULL) {
        if (strcmp(current->data.delivery_type, current_type) != 0) {
            strcpy(current_type, current->data.delivery_type);
            printf("\n--- %s DELIVERY ---\n", strcmp(current_type, "Fast") == 0 ? "FAST" : "STANDARD");
            print_table_header();
        }
        
        print_table_row(&current->data, addresses, addr_count);
        current = current->next;
    }
    print_divider();
}

void display_parcels_by_status(ParcelNode *head, const char *status, Address addresses[], int addr_count) {
    if (head == NULL) {
        printf("No parcels in system.\n");
        return;
    }
    
    print_table_header();
    int count = 0;
    ParcelNode *current = head;
    while (current != NULL) {
        if (strcmp(current->data.status, status) == 0) {
            print_table_row(&current->data, addresses, addr_count);
            count++;
        }
        current = current->next;
    }
    print_divider();
    printf("Total %s: %d parcels\n", status, count);
}

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

void display_admin_report(ParcelNode *head, Address addresses[], int addr_count) {
    display_summary(head);
    printf("\n");
    display_all_parcels(head, addresses, addr_count);
}

void display_rider_parcels(ParcelNode *head, int assigned_address_id, Address addresses[], int addr_count) {
    int count = 0;
    print_table_header();
    
    ParcelNode *current = head;
    while (current != NULL) {
        if (current->data.address_id == assigned_address_id) {
            print_table_row(&current->data, addresses, addr_count);
            count++;
        }
        current = current->next;
    }
    print_divider();
    Address *addr = find_address(addresses, addr_count, assigned_address_id);
    printf("Total active parcels assigned to your road (%s): %d parcels\n", 
           addr ? addr->street : "Unknown", count);
}

void display_rider_sorted_parcels(ParcelNode *sorted_head, int assigned_address_id, Address addresses[], int addr_count) {
    printf("=== SORTED DELIVERY QUEUE ===\n");
    if (sorted_head == NULL) {
        printf("No active parcels to sort.\n");
        return;
    }

    char current_type[20] = "";
    int has_displayed_header = 0;

    ParcelNode *current = sorted_head;
    while (current != NULL) {
        // Only process parcels that belong to this Rider's road!
        if (current->data.address_id == assigned_address_id) {
            if (strcmp(current->data.delivery_type, current_type) != 0) {
                strcpy(current_type, current->data.delivery_type);
                printf("\n--- %s DELIVERY ---\n", strcmp(current_type, "Fast") == 0 ? "FAST" : "STANDARD");
                print_table_header();
                has_displayed_header = 1;
            }
            
            print_table_row(&current->data, addresses, addr_count);
        }
        current = current->next;
    }
    if (has_displayed_header) {
        print_divider();
    } else {
        printf("No active sorted parcels assigned to your road.\n");
    }
}

void display_active_parcels(ParcelNode *head, Address addresses[], int addr_count) {
    if (head == NULL) {
        printf("No parcels in system.\n");
        return;
    }
    
    print_table_header();
    int count = 0;
    ParcelNode *current = head;
    while (current != NULL) {
        if (strcmp(current->data.status, "Delivered") != 0) {
            print_table_row(&current->data, addresses, addr_count);
            count++;
        }
        current = current->next;
    }
    print_divider();
    printf("Total Active (Non-Delivered) Parcels: %d\n", count);
}

void display_rider_active_parcels(ParcelNode *head, int assigned_address_id, Address addresses[], int addr_count) {
    if (head == NULL) {
        printf("No parcels in system.\n");
        return;
    }
    
    print_table_header();
    int count = 0;
    ParcelNode *current = head;
    while (current != NULL) {
        if (current->data.address_id == assigned_address_id && strcmp(current->data.status, "Delivered") != 0) {
            print_table_row(&current->data, addresses, addr_count);
            count++;
        }
        current = current->next;
    }
    print_divider();
    Address *addr = find_address(addresses, addr_count, assigned_address_id);
    printf("Total active (non-delivered) parcels assigned to your road (%s): %d\n", 
           addr ? addr->street : "Unknown", count);
}
