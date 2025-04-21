#include <iostream>
#include <unistd.h>  // For fork() and execlp()
#include <sys/wait.h>  // For waitpid()

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 5) {
        cerr << "Usage: ./main <arg1_for_copy> <arg2_for_copy> <arg1_for_grep> <arg2_for_grep>" << endl;
        return 1;
    }

    // Fork the first child process for the "copy" program
    pid_t pid1 = fork();

    if (pid1 == 0) {
        // In the child process 1 (running copy)
        cout << "Executing copy process with arguments: " << argv[1] << ", " << argv[2] << endl;
        execlp("./copy", "copy", argv[1], argv[2], NULL);  // Pass two arguments to copy program
        cerr << "Failed to execute copy\n";  // This will print if execlp fails
        return 1;
    } else if (pid1 < 0) {
        // If fork() fails
        cerr << "Failed to fork the first process.\n";
        return 1;
    }

    // Fork the second child process for the "grape" program
    pid_t pid2 = fork();

    if (pid2 == 0) {
        // In the child process 2 (running grape)
        cout << "Executing grep process with arguments: " << argv[3] << ", " << argv[4] << endl;
        execlp("./grep", "grep", argv[3], argv[4], NULL);  // Pass two arguments to grep program
        cerr << "Failed to execute grep\n";  // This will print if execlp fails
        return 1;
    } else if (pid2 < 0) {
        // If fork() fails
        cerr << "Failed to fork the second process.\n";
        return 1;
    }

    // Parent process waits for both child processes to finish
    waitpid(pid1, NULL, 0);  // Wait for the first child (copy)
    waitpid(pid2, NULL, 0);  // Wait for the second child (grape)

    cout << "Both processes executed successfully.\n";

    return 0;
}
