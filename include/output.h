#ifndef OUTPUT_H
#define OUTPUT_H

#include "parcel_list.h"
#include "address.h"
#include "login.h"

void display_all_parcels(ParcelNode *head, Address addresses[], int addr_count, User users[], int user_count);
void display_sorted_parcels(ParcelNode *sorted_head, Address addresses[], int addr_count, User users[], int user_count);
void display_parcels_by_status(ParcelNode *head, const char *status, Address addresses[], int addr_count, User users[], int user_count);
void display_parcel_detail(Parcel *parcel, Address addresses[], int addr_count, User users[], int user_count);
void display_summary(ParcelNode *head);

void print_table_header(void);
void print_table_row(Parcel *parcel, Address addresses[], int addr_count, User users[], int user_count);
void print_divider(void);

void display_admin_report(ParcelNode *head, Address addresses[], int addr_count, User users[], int user_count);
void display_rider_parcels(ParcelNode *head, int assigned_address_id, Address addresses[], int addr_count, User users[], int user_count);
void display_rider_sorted_parcels(ParcelNode *sorted_head, int assigned_address_id, Address addresses[], int addr_count, User users[], int user_count);
void display_active_parcels(ParcelNode *head, Address addresses[], int addr_count, User users[], int user_count);
void display_rider_active_parcels(ParcelNode *head, int assigned_address_id, Address addresses[], int addr_count, User users[], int user_count);
void print_barcode(int parcel_id);
void display_rider_commission(ParcelNode *head, User users[], int user_count, Address addresses[], int addr_count, int rider_idx);
void display_all_riders_commission_report(ParcelNode *head, User users[], int user_count, Address addresses[], int addr_count);

#endif
