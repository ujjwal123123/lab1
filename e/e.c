/*
 * q3 : reverse lowercase and uppercase (convert)
 * q2 : print number of non-alphabetic characters to stdout or stderr
 * (count)
 *
 * We need to send output of q3 to q2. Output of second file is stored
 * in file_out. q3 is given file_in as an input.
 *
 * stdin: 0
 * stdout: 1
 * stderr: 2
 */

#include <fcntl.h> // needed for O_RDONLY (as described in the assignment specifications)
#include <setjmp.h>
#include <signal.h> // for SIGALARM
#include <stdio.h>
#include <stdlib.h> // for exit()
#include <sys/wait.h>
#include <unistd.h> // for close() and pipe()

jmp_buf env;

void quit() {
    fprintf(stderr, "I've been killed\n");
    exit(1);
}

void alarm_handler() {
    fprintf(stderr, "Child did not finnish executing. Exiting.\n");
    longjmp(env, 1);
    exit(1);
}

// q3 (convert) needs to run before q2
// Returns exit status of q3 (convert)
int run_q3_change_case(
    const int pipe_file_descriptors[]) { // we pass file descriptor
                                         // for the pipe
    int pid = fork();

    if (pid == 0) {
        signal(SIGTERM, &quit); // set up a signal handler

        // this line is only executed by the child
        // This reads from the file and writes to pipe

        if (dup2(pipe_file_descriptors[STDOUT_FILENO],
                 STDOUT_FILENO) < 0) {
            fprintf(stderr, "Error %d\n", __LINE__);
        }

        fprintf(stderr, "Child (convert) pid: %d\n", getpid());
        execl("./c", "c", (char *)NULL);

        return 0;
    }
    else {
        int status;
        wait(&status);
        // close(pipe_file_descriptors[STDIN_FILENO]);
        close(pipe_file_descriptors[STDOUT_FILENO]);
        return WEXITSTATUS(status);
    }
}

int run_q2_count(const int pipe_file_descriptors[],
                 const char *bytes_to_read) {
    int pid = fork();

    if (pid == 0) {
        signal(SIGTERM, &quit); // set up a signal handler

        if (dup2(pipe_file_descriptors[STDIN_FILENO], STDIN_FILENO) <
            0) {
            fprintf(stderr, "Error %d\n", __LINE__);
        }
        fprintf(stderr, "Child (count) pid: %d\n", getpid());
        execl("./b", "b", bytes_to_read, (char *)NULL);

        return 0;
    }
    else {
        int status;
        wait(&status);
        close(pipe_file_descriptors[STDIN_FILENO]);
        close(pipe_file_descriptors[STDOUT_FILENO]);
        return WEXITSTATUS(status);
    }
}

int main(int argc, char const *argv[]) {
    // set n count
    const char *n = '\0';
    if (argc >= 2 && argv[1][0] == '-') {
        n = argv[1];
    }

    // set input file
    int file_in = STDIN_FILENO;
    if (argc >= 2 && argv[1][0] != '-') {
        // q5 filein
        // q5 filein fileout
        file_in = open(argv[1], O_RDONLY);
        if (file_in < 0) {
            fprintf(stderr, "Cannot open file\n");
            exit(-1);
        }
    }
    else if (argc >= 3 && argv[1][0] == '-') {
        // q5 -n filein
        // q5 -n filein fileout
        file_in = open(argv[2], O_RDONLY);
        if (file_in < 0) {
            fprintf(stderr, "Cannot open file\n");
            exit(-1);
        }
    }

    // set output file
    int file_out = STDOUT_FILENO;
    if (argc == 4) {
        // q5 -n filein fileout
        file_out =
            creat(argv[2], 0644); // this is rw-r--r--. TODO: Can we
                                  // use a macro here, instead?
        if (file_out < 0) {
            fprintf(stderr, "Cannot create file %d\n", __LINE__);
            exit(1);
        }
    }
    else if (argc == 3 && argv[1][0] != '-') {
        // q5 filein fileout
        file_out = creat(argv[2], 0644); // this is rw-r--r--
        if (file_out < 0) {
            fprintf(stderr, "Cannot create file %d\n", __LINE__);
            exit(1);
        }
    }

    if (dup2(file_in, STDIN_FILENO) < 0) {
        fprintf(stderr, "Error %d\n", __LINE__);
    }
    if (dup2(file_out, STDOUT_FILENO) < 0) {
        fprintf(stderr, "Error %d\n", __LINE__);
    }

    // create a pipe
    int pipe_file_descriptor[2];

    if (pipe(pipe_file_descriptor) == -1) {
        exit(1);
    }

    signal(SIGALRM, &alarm_handler); // set up a signal handler

    // save the stack environment to `env` using setjmp
    if (setjmp(env) != 0) {
        // longjmp below causes setjmp to return here

        kill(-1, SIGTERM); // kill children
        // wait loop (as below) for children
        fprintf(stderr,
                "read timeout in child, killing both children.\n");
        exit(1);
    }
    // setjmp returns here first time called

    alarm(15); // set an alarm to go off in 15 seconds

    int q3_status = run_q3_change_case(pipe_file_descriptor);
    int q2_status = run_q2_count(pipe_file_descriptor, n);

    if (q3_status != 0 || q2_status != 0) {
        fprintf(stderr,
                "One of the children terminated abnormally.\n");
        exit(2);
    }

    close(file_in);
    close(file_out);
    close(pipe_file_descriptor[STDIN_FILENO]);
    close(pipe_file_descriptor[STDOUT_FILENO]);

    alarm(0); // cancel alarm
    fprintf(stderr, "normal children exit\n");
    exit(0);
}
