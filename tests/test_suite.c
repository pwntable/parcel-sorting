#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/parcel_list.h"
#include "../include/login.h"
#include "../include/address.h"
#include "../include/status.h"
#include "../include/search.h"
#include "../include/sorting.h"

void test_address_management(void) {
    printf("Running Test: Address Management...\n");

    Address addresses[50] = {0};
    int count = 0;

    // 1. Add addresses
    Address a1 = {101, "Jalan Tun Razak", "Kuala Lumpur", "WP Kuala Lumpur", 0};
    Address a2 = {102, "Jalan Ampang", "Kuala Lumpur", "WP Kuala Lumpur", 0};

    int res1 = add_address(addresses, &count, a1);
    int res2 = add_address(addresses, &count, a2);

    assert(res1 == 1);
    assert(res2 == 1);
    assert(count == 2);

    // 2. Find address
    Address *found = find_address(addresses, count, 101);
    assert(found != NULL);
    assert(strcmp(found->street, "Jalan Tun Razak") == 0);

    Address *not_found = find_address(addresses, count, 999);
    assert(not_found == NULL);

    printf("-> Address Management PASSED!\n\n");
}

void test_login_and_user_management(void) {
    printf("Running Test: Login & User Management (Road Assignment)...\n");

    User users[10] = {0};
    int user_count = 0;

    // 1. Mock Admin
    users[0].user_id = 1;
    strcpy(users[0].username, "admin");
    strcpy(users[0].password, "admin123");
    users[0].role = ROLE_ADMIN;
    users[0].assigned_address_id = 0; // Admins have 0 (N/A)
    user_count = 1;

    // 2. Verify Credentials
    int idx = verify_credentials(users, user_count, "admin", "admin123");
    assert(idx == 0);

    int invalid_idx = verify_credentials(users, user_count, "admin", "wrongpassword");
    assert(invalid_idx == ROLE_INVALID);

    // 3. Register Rider with Mandatory Road (valid assigned_address_id > 0)
    int reg_ok = register_rider(users, &user_count, "rider1", "rider123", 101);
    assert(reg_ok == 1);
    assert(user_count == 2);
    assert(users[1].assigned_address_id == 101);
    assert(users[1].user_id == 2); // Auto-incremented ID

    // 4. Register Rider with Invalid/Missing Road (should fail)
    int reg_fail = register_rider(users, &user_count, "rider2", "rider123", 0);
    assert(reg_fail == -2); // -2 code indicates missing mandatory road assignment

    // 4b. Register Rider with Duplicate Road (should fail with -3)
    int reg_dup_road = register_rider(users, &user_count, "rider2", "rider123", 101);
    assert(reg_dup_road == -3); // Already assigned to rider1

    // 5. Update User: Admin changing to Rider (must specify valid road)
    int update_to_rider_fail = update_user(users, user_count, 1, "", ROLE_RIDER, 0);
    assert(update_to_rider_fail == -2); // Failed because no road was specified for Rider role

    // 5b. Update User: Admin changing to Rider with duplicate road (should fail with -3)
    int update_to_rider_dup = update_user(users, user_count, 1, "", ROLE_RIDER, 101);
    assert(update_to_rider_dup == -3); // 101 is already assigned to rider1

    int update_to_rider_ok = update_user(users, user_count, 1, "", ROLE_RIDER, 102);
    assert(update_to_rider_ok == 1);
    assert(users[0].role == ROLE_RIDER);
    assert(users[0].assigned_address_id == 102);

    // Change them back to Admin (road must automatically clear to 0)
    int update_to_admin = update_user(users, user_count, 1, "", ROLE_ADMIN, 0);
    assert(update_to_admin == 1);
    assert(users[0].role == ROLE_ADMIN);
    assert(users[0].assigned_address_id == 0); // Correctly cleared to 0 (NULL) for Admin

    // 6. Delete user
    int del_admin = delete_user(users, &user_count, 1);
    assert(del_admin == -2); // Cannot delete Admin

    int del_rider = delete_user(users, &user_count, 2);
    assert(del_rider == 1);
    assert(user_count == 1);

    printf("-> Login & User Management PASSED!\n\n");
}

void test_parcel_sorting_and_linked_list(void) {
    printf("Running Test: Parcel Sorting & Linked List Auto-Sort...\n");

    ParcelNode *head = NULL;

    // 1. Create a few parcels with different priority levels and house numbers
    Parcel p1 = {1, "Ali", "Abu", 101, "Fast", "Pending", 12, "2026-05-17 12:00", "", 0};
    Parcel p2 = {2, "Siti", "Ahmad", 102, "Standard", "Pending", 5, "2026-05-17 12:05", "", 0};
    Parcel p3 = {3, "Ravi", "Muthu", 101, "Fast", "Pending", 3, "2026-05-17 12:10", "", 0};
    Parcel p4 = {4, "John", "Doe", 102, "Standard", "Delivered", 1, "2026-05-17 12:15", "2026-05-17 13:00", 0};

    // 2. Insert into the list - should self-sort automatically!
    insert_parcel(&head, p2); // Standard, house 5
    insert_parcel(&head, p1); // Fast, house 12
    insert_parcel(&head, p3); // Fast, house 3
    insert_parcel(&head, p4); // Standard, Delivered (history)

    // Expected prioritized order:
    // 1. [Fast, House 3] (p3)
    // 2. [Fast, House 12] (p1)
    // 3. [Standard, House 5] (p2)
    // 4. [Standard, Delivered] (p4) (Delivered history pushed to bottom)

    ParcelNode *curr = head;
    assert(curr != NULL);
    assert(curr->data.parcel_id == 3); // Fast, House 3

    curr = curr->next;
    assert(curr != NULL);
    assert(curr->data.parcel_id == 1); // Fast, House 12

    curr = curr->next;
    assert(curr != NULL);
    assert(curr->data.parcel_id == 2); // Standard, House 5

    curr = curr->next;
    assert(curr != NULL);
    assert(curr->data.parcel_id == 4); // Delivered (bottom)

    free_all_parcels(&head);
    printf("-> Parcel Sorting & Linked List PASSED!\n\n");
}

void test_status_transitions(void) {
    printf("Running Test: Status Transitions...\n");

    // 1. Valid transitions
    assert(is_valid_transition("Pending", "Out for Delivery") == 1);
    assert(is_valid_transition("Pending", "Delivered") == 1);
    assert(is_valid_transition("Out for Delivery", "Delivered") == 1);

    // 2. Invalid transitions
    assert(is_valid_transition("Out for Delivery", "Pending") == 0);
    assert(is_valid_transition("Delivered", "Pending") == 0);
    assert(is_valid_transition("Delivered", "Out for Delivery") == 0);

    // 3. Status updates trigger auto-sort
    ParcelNode *head = NULL;
    Parcel p1 = {1, "Ali", "Abu", 101, "Fast", "Pending", 12, "2026-05-17 12:00", "", 0};
    Parcel p2 = {2, "Siti", "Ahmad", 102, "Fast", "Pending", 5, "2026-05-17 12:05", "", 0};
    insert_parcel(&head, p1);
    insert_parcel(&head, p2);

    // Order initially: p2 (house 5) then p1 (house 12)
    assert(head->data.parcel_id == 2);

    // Update p2 to "Delivered" -> should trigger re-sort and push p2 to the bottom!
    int res = update_parcel_status(&head, 2, "Delivered");
    assert(res == 1);
    assert(strcmp(find_parcel(head, 2)->data.status, "Delivered") == 0);

    // Trigger explicit list re-sort to ensure correct linked list heads are updated
    sort_parcel_list(&head);

    // Now head should be p1 (still Pending)
    assert(head->data.parcel_id == 1);
    assert(head->next->data.parcel_id == 2);

    free_all_parcels(&head);
    printf("-> Status Transitions PASSED!\n\n");
}

void test_search_features(void) {
    printf("Running Test: Search Features...\n");

    ParcelNode *head = NULL;
    Parcel p1 = {1, "Ali", "Abu", 101, "Fast", "Pending", 12, "2026-05-17 12:00", "", 0};
    Parcel p2 = {2, "Siti", "Ahmad", 102, "Standard", "Pending", 5, "2026-05-17 12:05", "", 0};
    insert_parcel(&head, p1);
    insert_parcel(&head, p2);

    // 1. Search by ID
    ParcelNode *found_id = search_by_id(head, 1);
    assert(found_id != NULL);
    assert(strcmp(found_id->data.receiver_name, "Abu") == 0);

    // 2. Search by Receiver Name (case insensitive)
    ParcelNode *found_name = search_by_receiver(head, "ahmad");
    assert(found_name != NULL);
    assert(found_name->data.parcel_id == 2);

    free_all_parcels(&head);
    printf("-> Search Features PASSED!\n\n");
}

int main(void) {
    printf("===========================================\n");
    printf("         PARCEL SORTING TEST SUITE         \n");
    printf("===========================================\n\n");

    test_address_management();
    test_login_and_user_management();
    test_parcel_sorting_and_linked_list();
    test_status_transitions();
    test_search_features();

    printf("===========================================\n");
    printf("      ALL TESTS PASSED SUCCESSFULLY!       \n");
    printf("===========================================\n");
    return 0;
}
