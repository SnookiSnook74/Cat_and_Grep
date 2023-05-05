#ifndef CAT_H
#define CAT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

typedef struct {
  int b, e, n, s, t, v;
  int number_of_empty_rows_s, number_of_empty_rows_b, new_line;
} flag_options;

int parse_flags(flag_options *flags, const char *str, int *error_status);
void handle_numbering_flags(int symbol, int empty_line, flag_options *flags);
void process_and_print_file_contents(FILE *file, flag_options *flags);
void unprintable_and_tabs_symbol(int symbol, flag_options *flags);
void print_error();

#endif  // CAT_H