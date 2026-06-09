#include <string.h>
#include <ctype.h>
#include "../include/search.h"

// Helper function for case-insensitive substring search on platforms without strcasestr
static char* custom_strcasestr(const char *haystack, const char *needle) {
    if (!*needle) {
        return (char*)haystack;
    }
    for (; *haystack; haystack++) {
        if (tolower((unsigned char)*haystack) == tolower((unsigned char)*needle)) {
            const char *h, *n;
            for (h = haystack, n = needle; *h && *n; h++, n++) {
                if (tolower((unsigned char)*h) != tolower((unsigned char)*n)) {
                    break;
                }
            }
            if (!*n) {
                return (char*)haystack;
            }
        }
    }
    return NULL;
}

/**
 * @brief Search for a parcel in the list by its unique parcel ID.
 * 
 * @param head Pointer to the head of the linked list.
 * @param parcel_id The unique ID of the parcel to search.
 * @return ParcelNode* Pointer to the matching ParcelNode, or NULL if not found.
 */
ParcelNode* search_by_id(ParcelNode *head, int parcel_id) {
    return find_parcel(head, parcel_id);
}

/**
 * @brief Search for the first parcel in the list matching a receiver name (case-insensitive substring match).
 * 
 * @param head Pointer to the head of the linked list.
 * @param name The receiver name or substring to search for.
 * @return ParcelNode* Pointer to the first matching ParcelNode, or NULL if not found.
 */
ParcelNode* search_by_receiver(ParcelNode *head, const char *name) {
    ParcelNode *current = head;
    while (current != NULL) {
        if (custom_strcasestr(current->data.receiver_name, name) != NULL) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

/**
 * @brief Search for the first parcel in the list matching a status string (case-insensitive match).
 * 
 * @param head Pointer to the head of the linked list.
 * @param status The status string to search for (e.g. "Pending").
 * @return ParcelNode* Pointer to the first matching ParcelNode, or NULL if not found.
 */
ParcelNode* search_by_status(ParcelNode *head, const char *status) {
    ParcelNode *current = head;
    while (current != NULL) {
        if (strcasecmp(current->data.status, status) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

/**
 * @brief Search for the first parcel in the list matching a delivery type (case-insensitive match).
 * 
 * @param head Pointer to the head of the linked list.
 * @param type The delivery type string to search for (e.g. "Fast").
 * @return ParcelNode* Pointer to the first matching ParcelNode, or NULL if not found.
 */
ParcelNode* search_by_delivery_type(ParcelNode *head, const char *type) {
    ParcelNode *current = head;
    while (current != NULL) {
        if (strcasecmp(current->data.delivery_type, type) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}
