#ifndef PARCEL_LIST_H
#define PARCEL_LIST_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    int parcel_id;
    char sender_name[50];
    char receiver_name[50];
    int address_id;
    char delivery_type[10];   // "Fast" or "Standard"
    char status[20];          // "Pending", "Out for Delivery", "Delivered"
    int house_number;
    char time_in[20];
    char time_out[20];
    int rider_id;
} Parcel;

typedef struct ParcelNode {
    Parcel data;
    struct ParcelNode *next;
} ParcelNode;

// Core linked list operations
void insert_parcel(ParcelNode **head, Parcel new_parcel);
void sort_parcel_list(ParcelNode **head);
int delete_parcel(ParcelNode **head, int parcel_id);
ParcelNode* find_parcel(ParcelNode *head, int parcel_id);
void free_all_parcels(ParcelNode **head);
int count_parcels(ParcelNode *head);

#endif
