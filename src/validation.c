#include "../include/validation.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/**
 * @brief Clears the terminal screen.
 * 
 * Invokes standard OS commands ("cls" on Windows, "clear" on macOS/Linux).
 */
void clear_screen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/**
 * @brief Strips leading and trailing white space characters from a string in-place.
 * 
 * @param str The string to trim.
 */
void trim_whitespace(char *str) {
    if (str == NULL) return;
    char *end;
    while(isspace((unsigned char)*str)) memmove(str, str + 1, strlen(str));
    if(*str == 0) return;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
}

/**
 * @brief Safely reads a string from stdin up to a specified size.
 * 
 * Cleans the input trailing newline character and flushes stdin if the input exceeds buffer size.
 * 
 * @param buffer Character array to store the input.
 * @param size The maximum size of the buffer.
 * @return int 1 if read successfully, 0 on failure.
 */
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

/**
 * @brief Validates if a string contains only digit characters.
 * 
 * @param input The string to validate.
 * @return int 1 if valid positive integer representation, 0 otherwise.
 */
int validate_integer(const char *input) {
    if (!input || *input == '\0') return 0;
    while (*input) {
        if (!isdigit((unsigned char)*input)) return 0;
        input++;
    }
    return 1;
}

/**
 * @brief Validates if the length of a string falls within a specified range.
 * 
 * @param input The string to check.
 * @param min Minimum character limit.
 * @param max Maximum character limit.
 * @return int 1 if length is within range, 0 otherwise.
 */
int validate_string_length(const char *input, int min, int max) {
    int len = strlen(input);
    return len >= min && len <= max;
}

/**
 * @brief Validates if a string contains only alphanumeric characters (letters and numbers).
 * 
 * @param input The string to validate.
 * @return int 1 if alphanumeric, 0 otherwise.
 */
int validate_alphanumeric(const char *input) {
    if (!input || *input == '\0') return 0;
    while (*input) {
        if (!isalnum((unsigned char)*input)) return 0;
        input++;
    }
    return 1;
}

/**
 * @brief Validates if a string contains only alphabetic characters and spaces.
 * 
 * @param input The string to validate.
 * @return int 1 if alphabetic/space, 0 otherwise.
 */
int validate_alpha_space(const char *input) {
    if (!input || *input == '\0') return 0;
    while (*input) {
        if (!isalpha((unsigned char)*input) && !isspace((unsigned char)*input)) return 0;
        input++;
    }
    return 1;
}

/**
 * @brief Validates an address field. Must contain at least one alphabetic word.
 * If the only alphabetic word is "jalan", it requires another alphabetic word.
 * 
 * @param input The string to validate.
 * @return int 1 if valid, 0 otherwise.
 */
int validate_address_field(const char *input) {
    if (!input || *input == '\0') return 0;
    
    int alpha_word_count = 0;
    int is_jalan_only = 1;
    
    const char *p = input;
    while (*p) {
        while (*p && !isalpha((unsigned char)*p)) {
            p++;
        }
        
        if (*p) {
            alpha_word_count++;
            const char *start = p;
            int len = 0;
            while (*p && isalpha((unsigned char)*p)) {
                len++;
                p++;
            }
            
            int is_jalan = 0;
            if (len == 5 &&
                tolower((unsigned char)start[0]) == 'j' &&
                tolower((unsigned char)start[1]) == 'a' &&
                tolower((unsigned char)start[2]) == 'l' &&
                tolower((unsigned char)start[3]) == 'a' &&
                tolower((unsigned char)start[4]) == 'n') {
                is_jalan = 1;
            }
            
            if (!is_jalan) {
                is_jalan_only = 0;
            }
        }
    }
    
    if (alpha_word_count == 0) return 0; 
    if (alpha_word_count == 1 && is_jalan_only) return 0; 
    
    return 1;
}

/**
 * @brief Parses the first sequence of numbers encountered in a string.
 * 
 * Useful for scanning scan-codes or barcode IDs containing prefixes (e.g. "*P-0001*").
 * 
 * @param input The input string containing digit sequences.
 * @return int The parsed integer ID, or 0 if no digits found.
 */
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

/**
 * @brief Interactively prompts the user to enter a numeric choice within a range.
 * 
 * Continues prompting until a valid integer in [min, max] is entered.
 * 
 * @param prompt Prompt string to print.
 * @param min Minimum acceptable choice value.
 * @param max Maximum acceptable choice value.
 * @return int The validated choice.
 */
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

/**
 * @brief Prompts user for a string input with rules on length, alphanumeric characters, and emptiness.
 * 
 * @param prompt Prompt string.
 * @param dest Output character array to copy input to.
 * @param dest_size Size of destination array.
 * @param min_len Minimum length of string.
 * @param max_len Maximum length of string.
 * @param is_alphanumeric If 1, string must be strictly alphanumeric.
 * @param allow_empty If 1, user can press Enter to submit empty string.
 * @return int Returns 1.
 */
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

/**
 * @brief Prompts user for a string input with rules on length, alphabetical characters + spaces, and emptiness.
 * 
 * @param prompt Prompt string.
 * @param dest Output character array to copy input to.
 * @param dest_size Size of destination array.
 * @param min_len Minimum length of string.
 * @param max_len Maximum length of string.
 * @param allow_empty If 1, user can press Enter to submit empty string.
 * @return int Returns 1.
 */
int get_validated_alpha_string(const char *prompt, char *dest, int dest_size, int min_len, int max_len, int allow_empty) {
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
        if (!validate_alpha_space(buffer)) {
            printf("Input must contain only letters and spaces.\n");
            continue;
        }
        
        strncpy(dest, buffer, dest_size - 1);
        dest[dest_size - 1] = '\0';
        return 1;
    }
}

/**
 * @brief Prompts user for a string input and ensures it's a valid address field.
 * 
 * @param prompt Prompt string.
 * @param dest Output character array to copy input to.
 * @param dest_size Size of destination array.
 * @param min_len Minimum length of string.
 * @param max_len Maximum length of string.
 * @param allow_empty If 1, user can press Enter to submit empty string.
 * @return int Returns 1.
 */
int get_validated_address_string(const char *prompt, char *dest, int dest_size, int min_len, int max_len, int allow_empty) {
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
        if (!validate_address_field(buffer)) {
            printf("Address must contain at least one valid word (e.g., 'jalan' alone is not enough or cannot be pure numbers).\n");
            continue;
        }
        
        strncpy(dest, buffer, dest_size - 1);
        dest[dest_size - 1] = '\0';
        return 1;
    }
}

/**
 * @brief Interactively prompts the user to enter a positive integer ID.
 * 
 * @param prompt Prompt string.
 * @param allow_empty If 1, pressing Enter returns 0 in out_id.
 * @param out_id Pointer to store the validated output ID.
 * @return int Returns 1.
 */
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

