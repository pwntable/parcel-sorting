#ifndef VALIDATION_H
#define VALIDATION_H

/**
 * @brief Clears the terminal screen.
 * 
 * Uses 'cls' for Windows and 'clear' for POSIX systems (macOS/Linux).
 */
void clear_screen(void);

// Add other validation prototypes here as planned
int validate_integer(const char *input);
int validate_string_length(const char *input, int min, int max);
int validate_alphanumeric(const char *input);
void trim_whitespace(char *str);

#endif
