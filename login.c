#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct User {
    char username[50];
    char password[50];
};

void registerUser() {
    
    struct User user;

    file = fopen("users.txt", "a");

    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("\n=== REGISTER ===\n");

    printf("Enter Username: ");
    scanf("%s", user.username);

    printf("Enter Password: ");
    scanf("%s", user.password);

    fprintf(file, "%s %s\n", user.username, user.password);

    fclose(file);

    printf("Registration Successful!\n");
}

void loginUser() {
    FILE *file;
    struct User user;

    char username[50];
    char password[50];

    int found = 0;

    file = fopen("users.txt", "r");

    if (file == NULL) {
        printf("No user data found!\n");
        return;
    }

    printf("\n=== LOGIN ===\n");

    printf("Enter Username: ");
    scanf("%s", username);

    printf("Enter Password: ");
    scanf("%s", password);

    while (fscanf(file, "%s %s", user.username, user.password) != EOF) {
        if (strcmp(username, user.username) == 0 && strcmp(password, user.password) == 0) {
            found = 1;
            break;
        }
    }

    fclose(file);

    if (found) {
        printf("Login Successful!\n");
    } else {
        printf("Invalid Username or Password!\n");
    }
}

int main() {
    int choice;

    do {
        printf("\n===== LOGIN SYSTEM =====\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerUser();
                break;

            case 2:
                loginUser();
                break;

            case 3:
                printf("Program Ended.\n");
                break;

            default:
                printf("Invalid Choice!\n");
        }

    } while (choice != 3);

    return 0;
}
