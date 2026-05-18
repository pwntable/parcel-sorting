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
