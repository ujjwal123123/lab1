// Question 3: convert program

#include <assert.h>
#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void sigpipe_handler() {
    fprintf(stderr, "I'been killed because my pipe reader died\n");
    exit(2);
}

void sigterm_handler() {
    fprintf(stderr, "I've been killed\n");
    exit(1);
}

int main(int argc, char *argv[]) {
    signal(SIGPIPE, &sigpipe_handler);
    signal(SIGTERM, &sigterm_handler);

    char inp = '\0';

    while (1) {
        int status = read(STDIN_FILENO, &inp,
                          sizeof(char)); // using syscall to print

        if (status == 0) { // check for EOF
            exit(0);
        }
        else if (status == -1) {
            fprintf(stderr, "An error occured\n");
            exit(1);
        }
        else if (islower(inp)) {
            inp += 'A' - 'a';
        }
        else if (isupper(inp)) {
            inp += 'a' - 'A';
        }
        write(STDOUT_FILENO, &inp, sizeof(char));
    }
}