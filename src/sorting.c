#include "../include/sorting.h"
#include <string.h>

ParcelNode* filter_active_parcels(ParcelNode *head) {
    ParcelNode *active_head = NULL;
    ParcelNode *current = head;
    
    while (current != NULL) {
        if (strcmp(current->data.status, "Delivered") != 0) {
            insert_parcel(&active_head, current->data);
        }
        current = current->next;
    }
    
    return active_head;
}

void split_by_delivery_type(ParcelNode *active, ParcelNode **fast_head, ParcelNode **standard_head) {
    ParcelNode *current = active;
    while (current != NULL) {
        if (strcmp(current->data.delivery_type, "Fast") == 0) {
            insert_parcel(fast_head, current->data);
        } else {
            insert_parcel(standard_head, current->data);
        }
        current = current->next;
    }
}

void sort_by_house_number(ParcelNode **head) {
    if (*head == NULL || (*head)->next == NULL) return;

    int swapped;
    ParcelNode *ptr1;
    ParcelNode *lptr = NULL;

    do {
        swapped = 0;
        ptr1 = *head;

        while (ptr1->next != lptr) {
            if (ptr1->data.house_number > ptr1->next->data.house_number) {
                // Swap data
                Parcel temp = ptr1->data;
                ptr1->data = ptr1->next->data;
                ptr1->next->data = temp;
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

ParcelNode* merge_sorted_lists(ParcelNode *fast, ParcelNode *standard) {
    if (fast == NULL) return standard;
    if (standard == NULL) return fast;

    ParcelNode *current = fast;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = standard;
    
    return fast;
}

ParcelNode* generate_sorted_queue(ParcelNode *head) {
    ParcelNode *active = filter_active_parcels(head);
    ParcelNode *fast = NULL;
    ParcelNode *standard = NULL;

    split_by_delivery_type(active, &fast, &standard);
    free_sorted_queue(&active); // We split them, so we free the intermediate active list

    sort_by_house_number(&fast);
    sort_by_house_number(&standard);

    return merge_sorted_lists(fast, standard);
}

void free_sorted_queue(ParcelNode **sorted_head) {
    free_all_parcels(sorted_head); // Safe since it's just a copy list
}
