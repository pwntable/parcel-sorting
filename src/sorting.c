#include "../include/sorting.h"
#include <string.h>

/**
 * @brief Filters the parcel list, returning a new self-sorted list of active (non-delivered) parcels.
 * 
 * @param head Pointer to the head of the original linked list.
 * @return ParcelNode* Head of the new filtered list.
 */
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

/**
 * @brief Splits a list of active parcels into two distinct self-sorted list pointers by delivery type.
 * 
 * @param active Head of the active parcels list.
 * @param fast_head Pointer to the head pointer of the new Fast list.
 * @param standard_head Pointer to the head pointer of the new Standard list.
 */
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

/**
 * @brief Sorts a linked list in-place by house number ascending.
 * 
 * Uses a bubble sort algorithm to swap the parcel data.
 * 
 * @param head Pointer to the head pointer of the linked list to sort.
 */
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

/**
 * @brief Merges two sorted lists together by appending the second list after the first.
 * 
 * @param fast Head of the first sorted list.
 * @param standard Head of the second sorted list.
 * @return ParcelNode* Head of the combined list.
 */
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

/**
 * @brief Generates a fully sorted queue of active parcels.
 * 
 * Logic:
 * 1. Filter out Delivered parcels.
 * 2. Split active parcels into Fast and Standard categories.
 * 3. Sort each category individually by house number ascending.
 * 4. Merge Fast and Standard categories back together.
 * 
 * @param head Head of the master parcel linked list.
 * @return ParcelNode* Head of the generated sorted delivery queue.
 */
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

/**
 * @brief Frees the memory allocated for a temporary sorted queue.
 * 
 * @param sorted_head Pointer to the head pointer of the temporary queue.
 */
void free_sorted_queue(ParcelNode **sorted_head) {
    free_all_parcels(sorted_head); // Safe since it's just a copy list
}
