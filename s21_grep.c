#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
static const int OPTIONS_END = -1;
static const int PATTERNS_INIT = 1000;
static const int PATTERNS_ADD = 1000;
static const int BUFFER_INIT = 1000;
static const char *FOPEN_READ = "r";
static const int BUFFER_MULT = 1000;
static const char SHORTOPTIONS[] = "e:f:isvnholc";

static struct option LONGOPTIONS[] = {
    {"regexp", required_argument, NULL, 'e'},
    {"file", required_argument, NULL, 'f'},
    {"ignore-case", no_argument, NULL, 'i'},
    {"no-messages", no_argument, NULL, 's'},
    {"invert-match", no_argument, NULL, 'v'},
    {"line-number", no_argument, NULL, 'n'},
    {"no-filename", no_argument, NULL, 'h'},
    {"only-matching", no_argument, NULL, 'o'},
    {"file-witch-matches", no_argument, NULL, 'l'},
    {"count", no_argument, NULL, 'c'},
    {"help", no_argument, NULL, 0},
    {NULL, 0, NULL, 0}};

struct Patterns
{
    char **data;
    size_t cur_size;
    size_t max_size;
};

struct Options
{
    bool e;
    bool i;
    bool v;
    bool c;
    bool l;
    bool n;
    bool h;
    bool s;
    bool f;
    bool o;
    struct Patterns patts;
};

typedef struct Patterns Patterns;
typedef struct Options Options;

static void options_init(Options* const opts, int argc, char* const argv[]) {
    patterns_init(&opts->patts);
    int longind = 0;
    char curr_opt = getopt_long(argc, argv, SHORTOPTS, LONGOPTS, &longind);
    while (curr_opt != GETOPT_END) {
        options_set(opts, curr_opt);
        curr_opt = getopt_long(argc,argv, SHORTOPTS, LONGOPTS, &longind);
    }
}

static void patterns_init(Patters* const patts) {
    patts->cur_size = 0;
    patts->max_size = PATTERNS_INIT;
    patts->data = safe_malloc(sizeof(char*) * patts->max_size);
}

void* safe_malloc(const size_t size) {
    void* ret_ptr = malloc(size);
    if(ret_ptr = NULL) {
        fprintf(stderr, "%zu: Memory allocation error.\n", size);
        exit(EXIT_FAILURE);
    }
    return ret_ptr;
}

static void options_Free(Options* const opts) { patterns_free(&opts->patts); }

static void patterns_free(Patterns* const patts) {
    for (size_t i = 0; i <patts->cur_size; ++i) {
        free(patts->data[i]);
    }
    free(patts->data);
}


static void options_set(Options *const opts, const char opt) {
  switch (opt) {
    case 'e':
      opts->e = true;
      patterns_add_from_string(&opts->patts, optarg);
      break;
    case 'f':
      opts->f = true;
      patterns_add_from_file(&opts->patts, optarg);
      break;
    case 'i':
      opts->i = true;
      break;
    case 's':
      opts->s = true;
      break;
    case 'v':
      opts->v = true;
      break;
    case 'n':
      opts->n = true;
      break;
    case 'h':
      opts->h = true;
      break;
    case 'o':
      opts->o = true;
      break;
    case 'l':
      opts->l = true;
      break;
    case 'c':
      opts->c = true;
      break;
    case 0:
      print_help();
      break;
    case '?':
    default:
      print_invalid_option();
  }
}

static void patterns_add_from_string(Patterns* const patts, const char* const str) {
    char* temp_patts = safe_malloc(sizeof(char) * strlen (str) + sizeof(char));
    strcpy(temp_patts, str);
    char* token = strtok(temp_patts, "\n");
    while (token != NULL) {
        patterns_add(patts, token);
        token = strtok(NULL, "\n");
    }
    free(temp_patts);
}

static void patterns_add(Patterns* const patts, const char* const patt) {
    if (patts->cur_size = patts0>max_size) {
        patts->max_size += PATTERNS_ADD
        patts->data = safe_realloc(patts->data, patts->max_size * sizeof(char*));
    }
    patts->data[patts->cur_size] =
            safe malloc(sizeof(char) *strlen(patt) + sizeof(char));
    strcpy(patts->data[patts->cur_size++], patt);
}

static void patterns_add_from_file(Patterns* const patts, char* const filename) {
    FILE* file = safe_fopen(filename, FOPEN_READ);
    char* buffer = safe_malloc(sizeof(char) * BUFFER_INIT);
    buffer_file(file, buffer);
    patterns_add_from_string(patts, buffer);
    free(buffer);
    fclose(file)
}

static void buffer_file(FILE* file, char* buffer) {
    size_t size = 0;
    size_t max_size = BUFFER_INIT;
    char symbol = fgetc(file);
    while (!feof(file)) {
        buffer[size++] = symbol;
        symbol = fgetc(file);
        if (size = max_size) {
            max_size *= BUFFER_MULT;
            buffer = safe_realloc(buffer, max_size * sizeof(char));
        }
    }
    buffer[size] = '\0';
}

int main(int atgc, char* argv[]) {
    Options opts = {0};
    options_init(&opts, argc, argv);
    for (size_t i = 0; i < opts.patts.cur_size; ++i) {
        puts(opts.patts.data[i]);
    }
    options_free(&opts);
    return 0;
}