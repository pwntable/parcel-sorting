#include "../include/parcel_list.h"

// Helper to determine the priority order of two parcels
// Returns < 0 if p1 should come before p2
int compare_parcels(Parcel p1, Parcel p2) {
    // 1. Status: Non-Delivered items always come before Delivered items
    int p1_delivered = (strcmp(p1.status, "Delivered") == 0);
    int p2_delivered = (strcmp(p2.status, "Delivered") == 0);
    if (p1_delivered != p2_delivered) return p1_delivered - p2_delivered;

    // 2. Both are same status (either both active or both delivered)
    // For active parcels, prioritize Fast Delivery
    if (!p1_delivered) {
        int p1_fast = (strcmp(p1.delivery_type, "Fast") == 0);
        int p2_fast = (strcmp(p2.delivery_type, "Fast") == 0);
        if (p1_fast != p2_fast) return p2_fast - p1_fast; // Fast(1) - Standard(0) = -1 (Fast comes first)

        // 3. Same type, sort by house number ascending
        return p1.house_number - p2.house_number;
    }

    // For delivered parcels, sort by parcel_id (history)
    return p1.parcel_id - p2.parcel_id;
}

void insert_parcel(ParcelNode **head, Parcel new_parcel) {
    ParcelNode *new_node = (ParcelNode *)malloc(sizeof(ParcelNode));
    if (new_node == NULL) {
        printf("Error: Memory allocation failed!\n");
        return;
    }
    new_node->data = new_parcel;
    new_node->next = NULL;

    // If list is empty or new node should be the new head
    if (*head == NULL || compare_parcels(new_parcel, (*head)->data) < 0) {
        new_node->next = *head;
        *head = new_node;
        return;
    }

    // Find the correct insertion point
    ParcelNode *current = *head;
    while (current->next != NULL && compare_parcels(new_parcel, current->next->data) >= 0) {
        current = current->next;
    }

    new_node->next = current->next;
    current->next = new_node;
}

// Re-sort the entire list (useful after a status update)
void sort_parcel_list(ParcelNode **head) {
    if (*head == NULL || (*head)->next == NULL) return;

    ParcelNode *sorted = NULL;
    ParcelNode *current = *head;
    while (current != NULL) {
        ParcelNode *next = current->next;
        // Re-insert into a new sorted list
        insert_parcel(&sorted, current->data);
        free(current);
        current = next;
    }
    *head = sorted;
}

int delete_parcel(ParcelNode **head, int parcel_id) {
    ParcelNode *current = *head;
    ParcelNode *prev = NULL;

    while (current != NULL) {
        if (current->data.parcel_id == parcel_id) {
            if (prev == NULL) {
                *head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return 1;
        }
        prev = current;
        current = current->next;
    }
    return 0;
}

ParcelNode* find_parcel(ParcelNode *head, int parcel_id) {
    ParcelNode *current = head;
    while (current != NULL) {
        if (current->data.parcel_id == parcel_id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void free_all_parcels(ParcelNode **head) {
    ParcelNode *current = *head;
    ParcelNode *next_node;
    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
    *head = NULL;
}

int count_parcels(ParcelNode *head) {
    int count = 0;
    ParcelNode *current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}
