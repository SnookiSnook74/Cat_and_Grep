#ifndef GREP_H
#define GREP_H
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 1024
// Структура для работы со всеми фалагами
typedef struct flags_list {
  int e;
  //  Игнорирует различия регистра
  int i;
  //  Инвертирует смысл поиска соответствий
  int v;
  // Выводит только количество совпадающих строк
  int c;
  //  Выводит только совпадающие файлы
  int l;
  // Предваряет каждую строку вывода номером строки из файла ввода
  int n;
  // Выводит совпадающие строки, не предваряя их именами файлов
  int h;
  // Подавляет сообщения об ошибках о несуществующих или нечитаемых файлах
  int s;
  // Получает регулярные выражения из файла
  int f;
  // Печатает только совпадающие (непустые) части совпавшей строки
  int o;
  // Количество совпадающих файлов
  int l_flag_count;
  // Номера строк
  int n_flag_count;
  // Количество совпадающих строк
  int c_lines_count;
} flags_list;

void parse_flags(int argc, char *argv[], flags_list *flags, char *patterns);
char **parse_args(int argc, char *argv[], flags_list *flags, char *patterns);
void flag_f(char *patterns);
void process_line(flags_list *flags, char *line, regex_t *preg, char *filename,
                  int *n_flag_count, int *l_flag_count, int *c_lines_count);
void process_file(FILE *file, flags_list *flags, regex_t *preg, char *filename, int multiple_files);
void output(int argc, flags_list *flags, char *reg, char **filenames);
void flag_o(char *line, regmatch_t *match, regex_t preg, int line_number,
            int should_print_line_number, char *filename, int multiple_files);

#endif  // GREP_H