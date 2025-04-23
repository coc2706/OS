#include <iostream>  // For input/output operations
#include <unistd.h>  // For fork(), pipe(), execl(), getpid()
#include <stdexcept> // For exception handling

using namespace std;

int main() {
    int fd[2];  // File descriptors for the pipe
    int pid;    // Process ID for fork

    try {
        // Step 1: Create pipe
        if (pipe(fd) == -1) {
            throw runtime_error("Pipe creation failed!");
        }

        // Step 2: Fork a new process
        pid = fork();
        if (pid < 0) {
            throw runtime_error("Fork failed!");
        }

        if (pid == 0) {
            // Child Process
            if (dup2(fd[1], STDOUT_FILENO) == -1) {
                throw runtime_error("dup2 failed in child!");
            }

            close(fd[0]);  // Close the read end of the pipe
            close(fd[1]);  // Close the write end after redirection

            // Run the 'sort' command
            if (execl("/usr/bin/sort", "sort", (char*)NULL) == -1) {
                throw runtime_error("exec sort failed!");
            }
        } else {
            // Parent Process
            if (dup2(fd[0], STDIN_FILENO) == -1) {
                throw runtime_error("dup2 failed in parent!");
            }

            close(fd[1]);  // Close the write end of the pipe
            close(fd[0]);  // Close the read end after redirection

            // Run the 'uniq' command
            if (execl("/usr/bin/uniq", "uniq", (char*)NULL) == -1) {
                throw runtime_error("exec uniq failed!");
            }
        }
    } catch (const runtime_error& e) {
        cerr << "Error: " << e.what() << endl;  // Catch runtime errors and print them
        exit(1);  // Exit the program with an error code
    }

    return 0;
}
