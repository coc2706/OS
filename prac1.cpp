#include <iostream>      // cout, cin
#include <fstream>       // file I/O
#include <stdexcept>     // exceptions
#include <unistd.h>      // fork(), getpid()
#include <sys/wait.h>    // wait()
#include <string.h>      // string funcs
#include <stdlib.h>      // exit(), malloc()

using namespace std;

// Function to copy content from one file to another (Manual simulation)
void copy_file(const char* src, const char* dest) 
{
    pid_t pid = fork();  // Create a child process

    if (pid == 0)  // Child process
    {
        cout << "Child (cp) PID: " << getpid() << endl;

        try {
            ifstream inFile(src);  // Open source file
            if (!inFile)
                throw runtime_error("❌ Error opening source file.");

            ofstream outFile(dest);  // Open destination file
            if (!outFile)
                throw runtime_error("❌ Error opening destination file.");

            char ch;
            while (inFile.get(ch)) { 
                outFile.put(ch);  // Write to dest
            }

            cout << "✅ File successfully copied in child process.\n";
            inFile.close();
            outFile.close();
        } 
        catch (const exception& e) {
            cout << e.what() << endl;
            exit(1);
        }

        exit(0);  // Exit child
    } 
    else if (pid > 0)  // Parent process
    {
        wait(NULL);  // Wait for child to finish
        cout << "✅ File copied from " << src << " to " << dest << endl;
    } 
    else  // Fork failed
    {
        perror("Fork failed");
    }
}

// Function to count word occurrences in a file (Manual simulation)
void grep_count(const char* filename, const char* word) 
{
    pid_t pid = fork();  // Create child

    if (pid == 0)  // Child process
    {
        cout << "Child (grep) PID: " << getpid() << endl;

        try {
            ifstream file(filename);  // Open file
            if (!file)
                throw runtime_error("❌ Error opening file.");

            string w;
            int count = 0;

            while (file >> w) { 
                if (w == word)     // Check match
                    count++;
            }

            file.close();

            if (count > 0)
                cout << "The word '" << word << "' appeared " << count << " times in the file." << endl;
            else
                cout << "Word not found in the file." << endl;
        } 
        catch (const exception& e) {
            cout << e.what() << endl;
            exit(1);
        }

        exit(0);  // Exit child
    } 
    else if (pid > 0)  // Parent
    {
        wait(NULL);  // Wait for child
        cout << "🔍 Grep search complete.\n";
    } 
    else  // Fork failed
    {
        perror("Fork failed");
    }
}

// Function to list files in a directory using exec()
void list_files(const char* dir)
{
    pid_t pid = fork();  // Create child process

    if (pid == 0)  // Child process
    {
        cout << "Child (ls) PID: " << getpid() << endl;
        
        // Use exec() to list files in the directory
        execlp("ls", "ls", dir, NULL);  // Execute ls command
        perror("exec failed");  // If exec fails, print error
        exit(1);  // Exit if exec fails
    }
    else if (pid > 0)  // Parent process
    {
        wait(NULL);  // Wait for child to finish
        cout << "🔍 File listing complete.\n";
    }
    else  // Fork failed
    {
        perror("Fork failed");
    }
}

int main() {
    int choice;
    char src[100], dest[100], file[100], word[100], dir[100];

    while (true) {
        // Menu display
        cout << "\n===== Linux Command Simulator =====\n";
        cout << "1. Copy File (cp)\n";
        cout << "2. Search Word in File (grep)\n";
        cout << "3. List Files in Directory (ls) - Using exec()\n";  
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                // File copy inputs
                cout << "Enter source filename: ";
                cin >> src;
                cout << "Enter destination filename: ";
                cin >> dest;
                copy_file(src, dest);  // Call copy
                break;

            case 2:
                // Word search inputs
                cout << "Enter filename: ";
                cin >> file;
                cout << "Enter word to search: ";
                cin >> word;
                grep_count(file, word);  // Call grep
                break;

            case 3:
                // Directory listing inputs (new functionality)
                cout << "Enter directory to list: ";
                cin >> dir;
                list_files(dir);  // Call list_files using exec()
                break;

            case 4:
                // Exit program
                cout << "Exiting... PID: " << getpid() << endl;
                exit(0);

            default:
                // Invalid menu input
                cout << "Invalid choice. Try again.\n"; 
        }
    }

    return 0;
}












/*

#include <iostream>
#include <dirent.h>  // For directory operations
#include <unistd.h>  // For fork
#include <sys/wait.h>  // For wait

using namespace std;

void list_files(const char* dir) 
{
    pid_t pid = fork();  // Create child process

    if (pid == 0)  // Child process
    {
        cout << "Child (manual ls) PID: " << getpid() << endl;

        DIR* d = opendir(dir);  // Open directory
        if (d == nullptr) {
            perror("Failed to open directory");
            exit(1);
        }

        struct dirent* entry;
        while ((entry = readdir(d)) != nullptr) {
            cout << entry->d_name << endl;  // Print each file name
        }

        closedir(d);  // Close directory
        exit(0);  // Exit child
    }
    else if (pid > 0)  // Parent process
    {
        wait(NULL);  // Wait for child to finish
        cout << "File listing complete.\n";
    }
    else  // Fork failed
    {
        perror("Fork failed");
    }
}


*/