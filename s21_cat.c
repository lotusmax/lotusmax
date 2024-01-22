#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

typedef struct {
    int b;
    int e;
    int n;
    int v;
    int s;
    int t;
    
} options;

void get_options(int argc, char* argv[], options *opts){
    int opt;
    static struct option long_options[] = {
        {"number-nonblank", 0, 0, 'b'},
        {"number", 0, 0, 'n'},
        {"squeeze-blank", 0, 0, 's'},
        {0, 0, 0, 0}
    }; 
  
    while ((opt = getopt_long(argc, argv, "+bevntTE", long_options, NULL) != -1)){
        switch (opt)
        {
        case 'b':
            opts -> b = 1;
            break;
        case 'e':
            opts -> e = 1;
            opts -> v = 1;
            break;
        case 'v':
            opts -> e = 1;
            break;
        case 'n':
            opts -> n = 1;
            break;
        case 't':
            opts -> t = 1;
            opts -> v = 1;
            break;
        case 'T':
            opts -> v = 1;
            break;
        case 'E':
            opts -> v = 1;
            break;        
        default:
            printf("Error\n");
            exit(1);
            break;
        }
    } 
}

void get_files(int argc, char *argv[], options *opts, char **files, int *count_files) {
    for (int i = 1; i < argc; i++) {
        if(argv[i][0] != '-') {
          files[(*count_files)] = argv[i];
          (*count_files)++;
        }
    }
}

void output(options opts, FILE *file) {
    char temp[2] = {-1, -1};
    int count_enter = 0;
    int moment_s = 0;
    char c;
    int line_number = 1;
    if (!file) {
        printf("Failed to open file.\n");
        return;
    }
    while ((c = getc(file)) != EOF) {
        temp[0] = temp[1];
        temp[1] = c;
        if (opts.s) {
            if (temp[0] == -1 && temp[1] == '\n') {
                moment_s = 1;
                count_enter++;
            } else if (temp[0] == '\n' && moment_s) {
                moment_s = 1;
                count_enter++;
            } else if (temp[1] != '\n') {
                moment_s = 0;
            } else if (temp[0] == '\n' && moment_s) {
                continue;
            }
        }
    }
    fclose(file);
}

int main(int argc, char* argv[]){
    options opts;
    int count_files = 0;
    char *files[argc];
    get_options(argc, argv, &opts);
    get_files(argc, argv, &opts, files, &count_files);

    for (int i = 0; i < count_files; i++) {
        FILE *file = fopen(files[i], "r");
        if (file) {
            output(opts, file);
            fclose(file);
        }
    }
    return 0;
}