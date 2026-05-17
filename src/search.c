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

ParcelNode* search_by_id(ParcelNode *head, int parcel_id) {
    return find_parcel(head, parcel_id);
}

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
