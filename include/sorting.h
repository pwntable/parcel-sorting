#ifndef SORTING_H
#define SORTING_H

#include "parcel_list.h"

// Main sorting function — returns new sorted list
ParcelNode* generate_sorted_queue(ParcelNode *head);

// Filter out delivered parcels
ParcelNode* filter_active_parcels(ParcelNode *head);

// Split into Fast and Standard groups
void split_by_delivery_type(ParcelNode *active, ParcelNode **fast_head, ParcelNode **standard_head);

// Sort a sub-list by house number (ascending)
void sort_by_house_number(ParcelNode **head);

// Merge two sorted lists (Fast first, then Standard)
ParcelNode* merge_sorted_lists(ParcelNode *fast, ParcelNode *standard);

// Free the sorted copy list
void free_sorted_queue(ParcelNode **sorted_head);

#endif
