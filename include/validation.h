#ifndef VALIDATION_H
#define VALIDATION_H

void clear_screen(void);
int validate_integer(const char *input);
int validate_string_length(const char *input, int min, int max);
int validate_alphanumeric(const char *input);
void trim_whitespace(char *str);
int safe_read_string(char *buffer, int size);
int parse_parcel_id_input(const char *input);

#endif
