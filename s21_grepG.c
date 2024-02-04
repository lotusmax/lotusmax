#include "s21_grep.h"

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("na");
    return 0;
  }
  char *templates[100];
  Options option = {0};
  int opt, template_count = 0;
  while ((opt = getopt(argc, argv, "eivcln")) != -1) {
    get_option(opt, &option);
    if (opt == 'e') {
      templates[template_count] = argv[optind];
      template_count++;
    }
  }
  char *template = NULL;
  if (option.pattern != 1) {
    template = (char *)malloc(strlen(argv[optind]) + 1);
    strcpy(template, argv[optind]);
    template_count = 1;
  } else
    template = get_concat_template(templates, template_count);
  if (argc - optind - template_count > 1)
    option.show_file_name = 1;
  else if (option.pattern == 1 && argc - optind - template_count > 1)
    option.show_file_name = 1;
  regex_t re;
  regex_t *re_ptr = NULL;
  re_ptr = compile_regular_expression(template, &option, &re);
  FILE *file = NULL;
  for (int i = optind + template_count; i < argc; i++) {
    char buffer = '\n';
    file = fopen(argv[i], "r");
    if (file == NULL) {
      printf("na");
      return 0;
    }
    if (get_comp(file, re_ptr, &option, argv[i], &buffer) && option.files) {
      printf("%s", argv[i]);
      printf("\n");
    } else if (buffer != '\n') {
      printf("\n");
    }
    fclose(file);
  }
  free(template);
  regfree(&re);
  return 0;
}

char *get_concat_template(char *templates[], int template_count) {
  size_t total_length = template_count - 1;
  for (int i = 0; i < template_count; ++i) {
    total_length += strlen(templates[i]);
  }

  char *template = (char *)malloc(total_length + 1);

  size_t current_position = 0;
  for (int i = 0; i < template_count; ++i) {
    size_t len = strlen(templates[i]);
    strncpy(template + current_position, templates[i], len);
    current_position += len;

    if (i < template_count - 1) {
      template[current_position] = '|';
      current_position++;
    }
  }

  template[current_position] = '\0';
  return template;
}

regex_t *compile_regular_expression(char *template, Options *option,
                                    regex_t *re_ptr) {
  if (option->ignore)
    regcomp(re_ptr, template, REG_ICASE);
  else if (option->pattern)
    regcomp(re_ptr, template, REG_EXTENDED);
  else
    regcomp(re_ptr, template, 0);
  return re_ptr;
}

int get_comp(FILE *file, regex_t *re_ptr, Options *option, char *file_name,
             char *buffer) {
  int count = 0, line_number = 0;
  char *line = NULL;
  size_t len = 0;
  regmatch_t match;

  while (getline(&line, &len, file) != -1) {
    line_number++;
    int result = regexec(re_ptr, line, 1, &match, 0);
    if ((!result && !option->invert) || (result && option->invert)) {
      count++;
      if (!option->count && !option->files) {
        if (option->show_file_name) printf("%s:", file_name);
        if (option->line_num) {
          printf("%d:", line_number);
        }
        printf("%s", line);
        *buffer = line[strlen(line) - 1];
      }
    }
  }
  if (option->count) {
    if (option->show_file_name) printf("%s:", file_name);
    printf("%d\n", count);
    *buffer = '\n';
  }
  free(line);
  return count;
}

void get_option(int opt, Options *option) {
  switch (opt) {
    case 'e':
      option->pattern = 1;
      break;
    case 'i':
      option->ignore = 1;
      break;
    case 'v':
      option->invert = 1;
      break;
    case 'c':
      option->count = 1;
      break;
    case 'l':
      option->files = 1;
      break;
    case 'n':
      option->line_num = 1;
      break;
  }
}