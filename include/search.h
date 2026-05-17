#ifndef SEARCH_H
#define SEARCH_H

#include "parcel_list.h"

ParcelNode* search_by_id(ParcelNode *head, int parcel_id);
ParcelNode* search_by_receiver(ParcelNode *head, const char *name);
ParcelNode* search_by_status(ParcelNode *head, const char *status);
ParcelNode* search_by_delivery_type(ParcelNode *head, const char *type);

#endif
