// Question 3: convert program

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char c = '\0';

    while (1) {
        c = getchar();
        if (c == EOF) {
            exit(0);
        }
        else if (islower(c)) {
            putchar(c + 'A' - 'a');
        }
        else if (isupper(c)) {
            putchar(c + 'a' - 'A');
        }
        else {
            putchar(c);
        }
    }
}