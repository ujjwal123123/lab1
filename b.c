// Question 2: count program

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    char c = '\0';

    int non_alpha_count = 0;

    while (1) {
        c = getchar();
        if (c == EOF) {
            fprintf(stderr, "%d\n", non_alpha_count);
            exit(0);
        }
        else if (!isalpha(c)) {
            non_alpha_count++;
        }
        putchar(c);
    }
}