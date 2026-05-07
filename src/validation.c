#include "validation.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/**
 * @brief Clears the terminal screen using system commands.
 */
void clear_screen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Stubs for other validation functions in the plan
void trim_whitespace(char *str) {
    char *end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
}
