#include <stdio.h>
#include <stdlib.h>
#include "validation.h"
#include <sqlite3.h>
#include "db_setup.h"


int main() {

    sqlite3 *db = NULL;
    int rc;
    clear_screen();
    
    printf("=========================================\n");
    printf("       PARCEL SORTING SYSTEM             \n");
    printf("=========================================\n");

    // ====================== DATABASE INITIALIZATION ======================
    printf("Initializing Database...\n");
    rc = init_database(&db);
    
    if (rc != SQLITE_OK) {
        printf("❌ Database initialization failed!\n");
        printf("Press Enter to exit...");
        getchar();
        return 1;
    }
    
    printf("✅ Database ready!\n\n");
    // =====================================================================

    // TODO: Put your main menu function here later
    // For example: main_menu(db);


    printf("\nPress Enter to exit...");
    
    getchar();

    //=======================================================
    sqlite3_close(db);
    return 0;
}
