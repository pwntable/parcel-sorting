#ifndef ADDRESS_H
#define ADDRESS_H

typedef struct {
    int address_id;
    char street[100];
    char city[50];
    char state[50];
    int house_number;
} Address;

int add_address(Address addresses[], int *count, Address new_addr);
int update_address(Address addresses[], int count, int address_id);
void display_address_list(Address addresses[], int count);
Address* find_address(Address addresses[], int count, int address_id);

#endif
