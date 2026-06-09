#include "../include/parcel_list.h"

// Helper to determine the priority order of two parcels
// Returns < 0 if p1 should come before p2
/**
 * @brief Determines the priority sorting order of two parcels.
 * 
 * Sorting logic:
 * 1. Status: Non-Delivered items always come before Delivered items.
 * 2. If status is same:
 *    - For active (Non-Delivered) parcels, prioritize Fast Delivery type over Standard.
 *    - If type is also same, sort by house number ascending.
 *    - For delivered parcels, sort by parcel_id ascending (history).
 * 
 * @param p1 First parcel.
 * @param p2 Second parcel.
 * @return int Negative value if p1 should come before p2, positive if after, or 0 if equal.
 */
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

/**
 * @brief Inserts a parcel into the linked list in self-sorted priority order.
 * 
 * Uses compare_parcels to find the correct sorted insertion point.
 * 
 * @param head Pointer to the head pointer of the linked list.
 * @param new_parcel The Parcel structure to insert.
 */
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

/**
 * @brief Re-sorts the entire linked list of parcels.
 * 
 * Useful after a status transition (e.g. from Pending to Delivered) which impacts priority order.
 * Re-inserts all nodes into a new linked list.
 * 
 * @param head Pointer to the head pointer of the linked list.
 */
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

/**
 * @brief Deletes a parcel from the linked list by its unique parcel ID.
 * 
 * Frees the memory of the deleted node.
 * 
 * @param head Pointer to the head pointer of the linked list.
 * @param parcel_id The unique ID of the parcel to delete.
 * @return int Returns 1 if deleted successfully, or 0 if not found.
 */
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

/**
 * @brief Finds a parcel node in the linked list by its unique ID.
 * 
 * @param head Pointer to the head of the linked list.
 * @param parcel_id The unique ID to search for.
 * @return ParcelNode* Pointer to the matching ParcelNode, or NULL if not found.
 */
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

/**
 * @brief Frees all nodes in the parcel linked list and sets the head pointer to NULL.
 * 
 * @param head Pointer to the head pointer of the linked list.
 */
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

/**
 * @brief Counts the total number of parcels in the linked list.
 * 
 * @param head Pointer to the head of the linked list.
 * @return int Total number of nodes.
 */
int count_parcels(ParcelNode *head) {
    int count = 0;
    ParcelNode *current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}
