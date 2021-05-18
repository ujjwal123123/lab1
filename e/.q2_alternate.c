
char inp[BUFSIZ];

if (n == -1) {
    //    while (n != 0) {
    int status =
        read(STDIN_FILENO, inp,
             sizeof(char)); // using syscall to read from stdio

    if (status == 0) { // check for EOF
        close(STDOUT_FILENO);
        //        break;
    }
    else if (status == -1) { // other errors as returned by read()
        fprintf(stderr, "An error occured\n");
        exit(1);
    }
    else if (status > 0) {
        for (int i = 0; i < status; i++) {
            if (!isalpha(inp[i]))
                non_alpha_count++;
        }
        // non_alpha_count++;
        // non_alpha_count++;
    }

    // using write() (a syscall) to print
    if (write(STDOUT_FILENO, inp, status) == -1) {
        fprintf(stderr, "Error: %d\n", __LINE__);
    }
}
if (n > 0) {
    //    while (n != 0) {
    int status =
        read(STDIN_FILENO, inp,
             sizeof(char)); // using syscall to read from stdio

    if (status == 0) { // check for EOF
        close(STDOUT_FILENO);
        //        break;
    }
    else if (status == -1) { // other errors as returned by read()
        fprintf(stderr, "An error occured\n");
        exit(1);
    }
    else if (status > 0) {
        for (int i = 0; i < status; i++) {
            if (!isalpha(inp[i]))
                non_alpha_count++;
        }
        // non_alpha_count++;
    }

    if (n <= status) {
        // using write() (a syscall) to print
        if (write(STDOUT_FILENO, inp, n) == -1) {
            fprintf(stderr, "Error: %d\n", __LINE__);
        }
    }
    else {
        if (write(STDOUT_FILENO, inp, status) == -1) {
            fprintf(stderr, "Error: %d\n", __LINE__);
        }
    }
    //        n -= 1;
    //  }
}
