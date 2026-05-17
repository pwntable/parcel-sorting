#include <string.h>
#include "../include/search.h"

ParcelNode* search_by_id(ParcelNode *head, int parcel_id) {
    return find_parcel(head, parcel_id);
}

ParcelNode* search_by_receiver(ParcelNode *head, const char *name) {
    ParcelNode *current = head;
    while (current != NULL) {
        if (strcasestr(current->data.receiver_name, name) != NULL) {
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
