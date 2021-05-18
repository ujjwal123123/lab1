/*
 * q2 : print number of non-alphabetic characters to stdout or stderr
 * q3 : reverse lowercase and uppercase
 *
 * We need to send output of q3 to q2. Output of second file is stored
 * in file_out. q3 is given file_in as an input.
 *
 * stdin: 0
 * stdout: 1
 * stderr: 2
 */

#include <fcntl.h> // needed for O_RDONLY (as described in the assignment specifications)
#include <stdio.h>
#include <stdlib.h> // for exit()
#include <sys/wait.h>
#include <unistd.h> // for close() and pipe()

// q3 needs to run before q2
void run_q3_change_case(
    int file_in,
    int pipe_file_descriptors[]) { // we pass file descriptor for the
                                   // pipe
    int pid = fork();

    if (pid == 0) {
        // this line is only executed by the child
        // This reads from the file and writes to pipe
        dup2(file_in, STDIN_FILENO);
        dup2(pipe_file_descriptors[STDOUT_FILENO], STDOUT_FILENO);
        execl("./c", "c", (char *)NULL);
    }
    else {
        int status;
        wait(&status);
        close(pipe_file_descriptors[1]);
    }
}

void run_q2_count(int file_out, int pipe_file_descriptors[]) {
    int pid = fork();

    if (pid == 0) {
        dup2(file_out, STDOUT_FILENO);
        dup2(pipe_file_descriptors[STDIN_FILENO], STDIN_FILENO);
        close(pipe_file_descriptors[STDOUT_FILENO]);
        execl("./b", "b", (char *)NULL);
    }
    else {
        int status;
        wait(&status);
        close(pipe_file_descriptors[0]);
        close(pipe_file_descriptors[1]);
    }
}

int main(int argc, char const *argv[]) {
    // if arguments are missing
    if (argc != 3) {
        fprintf(stderr,
                "Error: Provide input file and output file\n");
        exit(1);
    }
    int file_in = open(argv[1], O_RDONLY);
    if (file_in == -1) {
        exit(-1);
    }

    int file_out =
        creat(argv[2], 0644); // this is rw-r--r--. TODO: Can we use a
                              // macro here, instead?
    if (file_out == -1) {
        exit(1);
    }

    int pipe_file_descriptor[2];

    if (pipe(pipe_file_descriptor) == -1) {
        exit(1);
    }

    run_q3_change_case(file_in, pipe_file_descriptor);

    run_q2_count(file_out, pipe_file_descriptor);

    close(file_in);
    close(file_out);

    exit(0);
}
