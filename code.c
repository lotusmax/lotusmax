#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000

// Функция для чтения и печати содержимого файла
void cat_utility(char* filename, int number_nonblank, int number, int squeeze_blank) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    int line_number = 1;
    int last_line_was_blank = 0;

    // Читаем файл построчно
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        // Проверяем, является ли строка пустой
        if (strcmp(line, "\n") == 0) {
            if (squeeze_blank && last_line_was_blank) {
                continue;
            }
            last_line_was_blank = 1;
        } else {
            last_line_was_blank = 0;
        }

        // Печатаем номер строки, если необходимо
        if (number || (number_nonblank && strcmp(line, "\n") != 0)) {
            printf("%d ", line_number);
        }

        // Печатаем строку
        printf("%s", line);
        line_number++;
    }

    fclose(file);
}

int main(int argc, char* argv[]) {
    int number_nonblank = 0;
    int number = 0;
    int squeeze_blank = 0;
    char* filename;

    // Парсим аргументы командной строки
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--number-nonblank") == 0) {
            number_nonblank = 1;
        } else if (strcmp(argv[i], "--number") == 0) {
            number = 1;
        } else if (strcmp(argv[i], "--squeeze-blank") == 0) {
            squeeze_blank = 1;
        } else {
            filename = argv[i];
        }
    }

    // Вызываем функцию cat_utility с указанными флагами
    cat_utility(filename, number_nonblank, number, squeeze_blank);

    return 0;
}
