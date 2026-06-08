#include "../include/validation.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void clear_screen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void trim_whitespace(char *str) {
    if (str == NULL) return;
    char *end;
    while(isspace((unsigned char)*str)) memmove(str, str + 1, strlen(str));
    if(*str == 0) return;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
}

int safe_read_string(char *buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        } else {
            // flush stdin
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
        return 1;
    }
    return 0;
}

int validate_integer(const char *input) {
    if (!input || *input == '\0') return 0;
    while (*input) {
        if (!isdigit((unsigned char)*input)) return 0;
        input++;
    }
    return 1;
}

int validate_string_length(const char *input, int min, int max) {
    int len = strlen(input);
    return len >= min && len <= max;
}

int validate_alphanumeric(const char *input) {
    if (!input || *input == '\0') return 0;
    while (*input) {
        if (!isalnum((unsigned char)*input)) return 0;
        input++;
    }
    return 1;
}

int parse_parcel_id_input(const char *input) {
    if (input == NULL || strlen(input) == 0) return 0;
    
    const char *p = input;
    // Find the first digit character
    while (*p && (*p < '0' || *p > '9')) {
        p++;
    }
    
    if (*p == '\0') return 0;
    
    return atoi(p);
}

int get_validated_choice(const char *prompt, int min, int max) {
    char buffer[64];
    while (1) {
        printf("%s", prompt);
        if (!safe_read_string(buffer, sizeof(buffer))) {
            continue;
        }
        trim_whitespace(buffer);
        if (validate_integer(buffer)) {
            int val = atoi(buffer);
            if (val >= min && val <= max) {
                return val;
            }
        }
        printf("Invalid choice! Please enter a number between %d and %d.\n", min, max);
    }
}

int get_validated_string(const char *prompt, char *dest, int dest_size, int min_len, int max_len, int is_alphanumeric, int allow_empty) {
    char buffer[512];
    while (1) {
        printf("%s", prompt);
        if (!safe_read_string(buffer, sizeof(buffer))) {
            continue;
        }
        trim_whitespace(buffer);
        int len = strlen(buffer);
        if (len == 0) {
            if (allow_empty) {
                dest[0] = '\0';
                return 1;
            }
            printf("Input cannot be empty.\n");
            continue;
        }
        if (!validate_string_length(buffer, min_len, max_len)) {
            printf("Input length must be between %d and %d characters.\n", min_len, max_len);
            continue;
        }
        if (is_alphanumeric && !validate_alphanumeric(buffer)) {
            printf("Input must contain only letters and numbers (alphanumeric).\n");
            continue;
        }
        
        strncpy(dest, buffer, dest_size - 1);
        dest[dest_size - 1] = '\0';
        return 1;
    }
}

int get_validated_int_id(const char *prompt, int allow_empty, int *out_id) {
    char buffer[64];
    while (1) {
        printf("%s", prompt);
        if (!safe_read_string(buffer, sizeof(buffer))) {
            continue;
        }
        trim_whitespace(buffer);
        if (strlen(buffer) == 0) {
            if (allow_empty) {
                *out_id = 0;
                return 1;
            }
            printf("Input cannot be empty. Please enter a valid ID.\n");
            continue;
        }
        if (validate_integer(buffer)) {
            *out_id = atoi(buffer);
            return 1;
        }
        printf("Invalid input! Please enter a valid positive integer ID.\n");
    }
}

