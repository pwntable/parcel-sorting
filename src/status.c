#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../include/status.h"

/**
 * @brief Retrieves the current calendar date and time formatted as a string.
 * 
 * Formats date/time as "YYYY-MM-DD HH:MM".
 * 
 * @param buffer Character array to store the formatted time.
 * @param size The size of the buffer.
 */
void get_current_time(char *buffer, int size) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(buffer, size, "%04d-%02d-%02d %02d:%02d",
             tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
             tm.tm_hour, tm.tm_min);
}

/**
 * @brief Prints static list of all possible parcel delivery statuses.
 */
void display_status_options(void) {
    printf("Available Statuses:\n");
    printf("1. Pending\n");
    printf("2. Out for Delivery\n");
    printf("3. Delivered\n");
}

/**
 * @brief Displays valid status options a parcel can transition to based on its current status.
 * 
 * Logic:
 * - Pending can go to "Out for Delivery" or "Delivered".
 * - Out for Delivery can only go to "Delivered".
 * - Delivered is terminal (no options shown).
 * 
 * @param current_status String of the parcel's current status.
 */
void display_dynamic_status_options(const char *current_status) {
    printf("Current Status: %s\n", current_status);
    printf("Available Transitions:\n");
    if (strcmp(current_status, "Pending") == 0) {
        printf("1. Out for Delivery\n");
        printf("2. Delivered\n");
    } else if (strcmp(current_status, "Out for Delivery") == 0) {
        printf("1. Delivered\n");
    }
    printf("0. Cancel Update\n");
}

/**
 * @brief Validates if a proposed status transition is permitted.
 * 
 * Transitions:
 * - Pending -> Out for Delivery, Delivered (Valid)
 * - Out for Delivery -> Delivered (Valid)
 * - All other transitions are invalid.
 * 
 * @param current_status Existing status string.
 * @param new_status Proposed new status string.
 * @return int 1 if valid, 0 if invalid.
 */
int is_valid_transition(const char *current_status, const char *new_status) {
    if (strcmp(current_status, "Pending") == 0) {
        return (strcmp(new_status, "Out for Delivery") == 0 || strcmp(new_status, "Delivered") == 0);
    }
    if (strcmp(current_status, "Out for Delivery") == 0) {
        return (strcmp(new_status, "Delivered") == 0);
    }
    return 0; // "Delivered" is a terminal state, no outgoing transitions
}

/**
 * @brief Updates the status of a parcel and re-sorts the list.
 * 
 * If transition is to "Delivered", automatically stamps the current timestamp in time_out.
 * Triggers a list re-sort to move "Delivered" history to the bottom.
 * 
 * @param head Pointer to the head pointer of the linked list.
 * @param parcel_id Unique ID of the parcel to update.
 * @param new_status Proposed new status string.
 * @return int 1 on success, 0 if parcel not found, -1 if transition is invalid.
 */
int update_parcel_status(ParcelNode **head, int parcel_id, const char *new_status) {
    ParcelNode *node = find_parcel(*head, parcel_id);
    if (node == NULL) return 0; // Parcel not found

    if (!is_valid_transition(node->data.status, new_status)) {
        return -1; // Invalid transition
    }

    strcpy(node->data.status, new_status);
    
    if (strcmp(new_status, "Delivered") == 0) {
        get_current_time(node->data.time_out, 20);
    }

    // Auto-sort after status change to push Delivered parcels to the bottom
    sort_parcel_list(head);

    return 1; // Success
}
