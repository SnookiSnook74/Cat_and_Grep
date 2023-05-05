#include "s21_cat.h"

int main(int argc, char *argv[]) {
  int error_status = 0;
  flag_options flags = {0};

  if (argc > 1) {
    for (int i = 1; i < argc && error_status == 0; i++) {
      if (argv[i][0] == '-') {
        if (parse_flags(&flags, argv[i], &error_status) == 0) {
        }
      } else {
        FILE *file = fopen(argv[i], "r");
        if (file == NULL) {
          printf("Error: failed to open file '%s'\n", argv[i]);
          error_status = 1;
        } else {
          if (error_status == 0) {
            process_and_print_file_contents(file, &flags);
          }
          fclose(file);
        }
      }
    }
  } else {
    print_error();
  }
  return 0;
}

void process_and_print_file_contents(FILE *file, flag_options *flags) {
  int symbol;
  while ((symbol = getc(file)) != EOF) {
    int empty_line = 0;
    if (flags->s && flags->number_of_empty_rows_s == 0 && symbol == '\n') {
      flags->number_of_empty_rows_s += 1;
    } else if (flags->number_of_empty_rows_s != 0 && symbol == '\n') {
      flags->number_of_empty_rows_s += 1;
      empty_line = 1;
    } else if (flags->number_of_empty_rows_s > 1 && symbol != '\n') {
      flags->number_of_empty_rows_s = 0;
      flags->e ? printf("$\n") : printf("\n");
      if (flags->n != 0) printf("%6d\t", flags->n++);
    } else {
      flags->number_of_empty_rows_s = 0;
    }
    handle_numbering_flags(symbol, empty_line, flags);
    if (symbol == '\n' && flags->e && empty_line == 0) printf("$");

    if (empty_line == 0) unprintable_and_tabs_symbol(symbol, flags);
  }
}

void handle_numbering_flags(int symbol, int empty_line, flag_options *flags) {
  if (flags->n != 0 || flags->b != 0) {
    if (flags->new_line == 1) {
      flags->new_line = 0;
      printf("%6d\t", flags->n++);
    }
    if (flags->n == 1) printf("%6d\t", flags->n++);
    if (flags->b == 1) printf("%6d\t", flags->b++);
    if (symbol == '\n' && flags->n != 0 && empty_line == 0) flags->new_line = 1;
    if (symbol == '\n' && flags->b != 0) flags->number_of_empty_rows_b++;
    if (symbol != '\n' && flags->number_of_empty_rows_b != 0 &&
        flags->number_of_empty_rows_s == 0) {
      flags->number_of_empty_rows_b = 0;
      printf("%6d\t", flags->b++);
    }
  }
}

void unprintable_and_tabs_symbol(int symbol, flag_options *flags) {
  if (symbol < 32 && symbol != '\t' && symbol != '\n' && flags->v) {
    printf("^%c", symbol + '@');
  } else if (symbol == 127 && flags->v) {
    printf("^?");
  } else if (symbol >= 128 && flags->v) {
    printf("M-%c", symbol - 128);
  } else if (symbol == '\t' && flags->t) {
    printf("^I");
  } else {
    printf("%c", symbol);
  }
}

int parse_flags(flag_options *flags, const char *str, int *error_status) {
  int len = strlen(str);
  int i = 0;
  if (strncmp("--number", str, len) == 0) {
    if (flags->b) {
      flags->n = 0;
    } else {
      flags->n = 1;
    }
  } else if (strncmp("--number-nonblank", str, len) == 0) {
    flags->n = 0;
    flags->b = 1;
  } else if (strncmp("--squeeze-blank", str, len) == 0) {
    flags->s = 1;
  } else {
    while (i < len) {
      char c = str[i];
      if (c == '-') {
        i++;
        continue;
      }
      switch (c) {
        case 'b':
          flags->n = 0;
          flags->b = 1;
          break;
        case 'e':
          flags->e = 1;
          flags->v = 1;
          break;
        case 'E':
          flags->e = 1;
          break;
        case 'n':
          if (flags->b) {
            flags->n = 0;
          } else {
            flags->n = 1;
          }
          break;
        case 's':
          flags->s = 1;
          break;
        case 't':
          flags->t = 1;
          flags->v = 1;
          break;
        case 'T':
          flags->t = 1;
          break;
        case 'v':
          flags->v = 1;
          break;
        default:
          printf("illegal option %c\n", c);
          print_error();
          *error_status = 1;
      }
      i++;
    }
  }
  return *error_status;
}

void print_error() { printf("usage: ./s21_cat [-benstv] [file ...]\n"); }