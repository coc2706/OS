#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
using namespace std;

// Function to convert string to uppercase
string to_uppercase(const string &input) {
    string result = input;
    for (char &ch : result) ch = toupper(ch);
    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <message_to_child>\n";
        return 1;
    }

    string message = argv[1];

    int pipe1[2]; // Parent -> Child
    int pipe2[2]; // Child -> Parent

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    else if (pid == 0) {
        // Child process
        close(pipe1[1]); // Close write end of pipe1
        close(pipe2[0]); // Close read end of pipe2

        char buffer[256];
        read(pipe1[0], buffer, sizeof(buffer));
        close(pipe1[0]);

        string received(buffer);
        string processed = to_uppercase(received);

        // Redirect stdout to pipe2
        dup2(pipe2[1], STDOUT_FILENO);
        close(pipe2[1]);

        cout << processed << endl;
    }

    else {
        // Parent process
        close(pipe1[0]); // Close read end of pipe1
        close(pipe2[1]); // Close write end of pipe2

        write(pipe1[1], message.c_str(), message.length() + 1);
        close(pipe1[1]);

        char result[256];
        read(pipe2[0], result, sizeof(result));
        close(pipe2[0]);

        wait(NULL); // Wait for child to finish

        cout << "Parent received from child: " << result;
    }

    return 0;
}
