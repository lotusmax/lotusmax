#include <stdio.h>
#include <stdlib.h>

size_t strlen(const char *str) {
    const char *s = str;
    while (*str) {
        str++;
    }
    return str - s;
}

int main() {
    char *filename = "txt.txt";
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Ошибка: не удалось открыть файл %s\n", filename);
        return  1;
    }

    char ch;
    size_t length =  0;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') { // Найден символ новой строки, подсчитываем длину строки
            printf("Длина строки: %zu\n", length);
            length =  0; // Сбрасываем счетчик длины для следующей строки
        } else {
            length++; // Увеличиваем счетчик длины строки
        }
    }

    // Проверяем, была ли последняя строка не пустой
    if (length >  0) {
        printf("Длина последней строки: %zu\n", length);
    }

    fclose(fp);
    return  0;
}
