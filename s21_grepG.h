#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Options {
  int pattern;
  int ignore;
  int invert;
  int count;
  int files;
  int line_num;
  int show_file_name;
} Options;

void get_option(int opt, Options *option);
char *get_concat_template(char *templates[], int template_count);
regex_t *compile_regular_expression(char *template, Options *option,
                                    regex_t *re_ptr);
int get_comp(FILE *file, regex_t *re_ptr, Options *option, char *file_name,
             char *buffer);