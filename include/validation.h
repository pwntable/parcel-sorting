#ifndef VALIDATION_H
#define VALIDATION_H

void clear_screen(void);
int validate_integer(const char *input);
int validate_string_length(const char *input, int min, int max);
int validate_alphanumeric(const char *input);
void trim_whitespace(char *str);
int safe_read_string(char *buffer, int size);
int parse_parcel_id_input(const char *input);

int get_validated_choice(const char *prompt, int min, int max);
int get_validated_string(const char *prompt, char *dest, int dest_size, int min_len, int max_len, int is_alphanumeric, int allow_empty);
int get_validated_int_id(const char *prompt, int allow_empty, int *out_id);

#endif
