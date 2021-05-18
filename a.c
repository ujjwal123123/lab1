#include <stdio.h>

int main(int argc, char const *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            printf("%s\n", argv[i]);
        }
    }
    return 0;
}
