// Question 2: count program

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h> // needed for strtoul
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// count does not need sigpipe handler

void sigterm_handler() {
    fprintf(stderr, "I've been killed\n");
    exit(1);
}

int main(int argc, char *argv[]) {
    // this program needs -n
    signal(SIGTERM, &sigterm_handler);

    int n = -1;
    if (argc == 2) {
        int error = 0;

        // n = strtoul(argv[1] + 1, NULL, 10);
        n = atoi(argv[1] + 1);
        assert(n >= 0);

        if (errno != 0) {
            fprintf(stderr, "Invalid argument passed\n");
            exit(2);
        }
    }
    int non_alpha_count = 0;
    signal(SIGTERM, &sigterm_handler);
    char inp = '\0';

    while (n != 0) {
        int status =
            read(STDIN_FILENO, &inp,
                 sizeof(char)); // using syscall to read from stdio

        if (status == 0) { // check for EOF
            close(STDOUT_FILENO);
            break;
        }
        else if (status == -1) { // other errors as returned by read()
            fprintf(stderr, "An error occured\n");
            exit(1);
        }
        else if (!isalpha(inp)) {
            non_alpha_count++;
        }

        fflush(stdout); // trying stdout to prevent output after shell
                        // prompt

        // using write() (a syscall) to print
        if (write(STDOUT_FILENO, &inp, sizeof(char)) == -1) {
            fprintf(stderr, "Error: %d\n", __LINE__);
        }

        n -= 1;
    }

    fflush(stdout);
    fprintf(stderr, "%d\n", non_alpha_count);
    close(STDOUT_FILENO);
    close(STDIN_FILENO);
    return 0;
}