#define _GNU_SOURCE
#include "s21_grep.h"

int main(int argc, char *argv[]) {
  flags_list flags = {0};
  char **str = NULL;
  if (argc > 2) {
    char patterns[BUFF_SIZE] = {'\0'};
    parse_flags(argc, argv, &flags, patterns);
    str = parse_args(argc, argv, &flags, patterns);
  } else {
    printf(
        "Invalid number of arguments\nusage: ./s21_grep [-eivclnhsfo] "
        "filename");
  }
  if (str != NULL) free(str);
  return 0;
}

char **parse_args(int argc, char *argv[], flags_list *flags, char *patterns) {
  char reg[BUFF_SIZE] = {'\0'};
  char **filenames = NULL;

  filenames = (char **)malloc(BUFF_SIZE * BUFF_SIZE * sizeof(char *) +
                              BUFF_SIZE * sizeof(char *));
  char *ptr = (char *)filenames + BUFF_SIZE;
  for (int i = 0; i < BUFF_SIZE; i++) {
    filenames[i] = ptr + BUFF_SIZE * i;
  }

  if (!flags->e && !flags->f) {
    sprintf(reg, "%s", argv[optind++]);
  } else {
    sprintf(reg, "%s", patterns);
  }
  if (flags->e) {
    sprintf(reg, "%s", patterns);
  }
  for (int i = optind, files_count = 0; i < argc; i++, files_count++) {
    strcpy(filenames[files_count], argv[i]);
  }
  output(argc, flags, reg, filenames);
  return filenames;
}

void process_file(FILE *file, flags_list *flags, regex_t *preg, char *filename, int multiple_files) {
  char *line = NULL;
  regmatch_t match;
  size_t length = 0;
  flags->n_flag_count = 0;
  flags->l_flag_count = 0;

  while (getline(&line, &length, file) > 0) {
    int reg_comp;
    flags->n_flag_count++;
    if (((reg_comp = regexec(preg, line, 1, &match, 0)) == 0) && !flags->v) {
      if (flags->c || flags->l) {
        flags->l_flag_count = 1;
        if (flags->c) {
          flags->c_lines_count++;
          continue;
        }
        continue;
      }
      if (!flags->h && multiple_files && !flags->o) printf("%s:", filename);
      if (flags->n && !flags->o) printf("%d:", flags->n_flag_count);
      if (flags->o) {
        flag_o(line, &match, *preg, flags->n_flag_count, flags->n,
               filename, multiple_files);
        continue;
      }
      printf("%s", line);
      if (line[strlen(line) - 1] != '\n') printf("\n");
    } else if (flags->v && reg_comp == 1) {
      if (flags->c || flags->l) {
        flags->l_flag_count = 1;
        if (flags->c) {
          flags->c_lines_count++;
          continue;
        }
        continue;
      }
      if (!flags->h && multiple_files) {
        printf("%s:", filename);
      }
      if (flags->n) {
        printf("%d:", flags->n_flag_count);
      }
      printf("%s", line);
      if (line[strlen(line) - 1] != '\n') {
        printf("\n");
      }
    }
  }

  if (flags->c) {
    if (!flags->h && multiple_files) {
      printf("%s:", filename);
    }
    if (flags->l && flags->c_lines_count > 1) {
      flags->c_lines_count = 1;
    }
    printf("%d\n", flags->c_lines_count);
    flags->c_lines_count = 0;
  }

  if (flags->l && flags->l_flag_count) {
    printf("%s\n", filename);
  }

  if (line != NULL) {
    free(line);
  }
}


void output(int argc, flags_list *flags, char *reg, char **filenames) {
  flags->c_lines_count = 0;
  regex_t preg_storage;
  regex_t *preg = &preg_storage;
  int flag = 0;
  if (flags->f || flags->e) flag = REG_EXTENDED;
  int multiple_files = argc - optind > 1;
  if (flags->i) {
    flag |= REG_ICASE;
  }

  for (int i = 0; i < argc - optind; i++) {
    // Linux
    if (flags->o && flags->v) break;
    FILE *file = fopen(filenames[i],"r");
    if (file == NULL) {
      if (!flags->s) {
        perror("s21_grep");
      }
    } else {
      if ((regcomp(preg, reg, flag)) == 0) {
        process_file(file, flags, preg, filenames[i], multiple_files);
      }
      if (file != NULL) fclose(file);
      regfree(preg);
    }
  }
}

void flag_o(char *line, regmatch_t *match, regex_t preg, int line_number,
            int should_print_line_number, char *filename, int multiple_files) {
  while (1) {
    if (regexec(&preg, line, 1, match, 0) != 0) {
      break;
    }
    if (multiple_files) {
      printf("%s:", filename);
    }
    if (should_print_line_number) {
      printf("%d:", line_number);
    }
    for (int i = match[0].rm_so; i < match[0].rm_eo; ++i) {
      printf("%c", line[i]);
    }
    printf("\n");
    line += match[0].rm_eo;
  }
}

void flag_f(char *patterns) {
  FILE *file;
  file = fopen(optarg, "r");
  if (file != NULL) {
    char buffer[BUFF_SIZE] = {'\0'};
    while (!feof(file)) {
      if (fgets(buffer, sizeof(buffer), file)) {
        strcat(patterns, buffer);
        if (patterns[strlen(patterns) - 1] == '\n')
          patterns[strlen(patterns) - 1] = '\0';
        strcat(patterns, "|");
      }
    }
    patterns[strlen(patterns) - 1] = '\0';
    fclose(file);
  } else {
    perror("s21_grep");
  }
}

void parse_flags(int argc, char *argv[], flags_list *flags, char *patterns) {
  char tmp[BUFF_SIZE] = {'\0'};
  int c = 0;

  for (; c != -1; c = getopt(argc, argv, "e:ivclnhsf:o")) {
    switch (c) {
      case 'e':
        flags->e = 1;
        strcpy(tmp, patterns);
        sprintf(patterns, "%s%s%s", tmp, *patterns == '\0' ? "\0" : "|",
                optarg);
        break;
      case 'i':
        flags->i = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        // Linux
        flags->c = 0;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'f':
        flags->f = 1;
        flag_f(patterns);
        break;
      case 'o':
        flags->o = 1;
        break;
      default:
        break;
    }
  }
}