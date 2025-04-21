#include <iostream>
#include <vector>
#include <limits>
using namespace std;

int n, m;
vector<int> available;
vector<int> favail;
vector<vector<int>> maxMatrix, allocated, need;
vector<bool> finished;
vector<int> safeSequence;

// Function to clear input buffer
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Function to print step-by-step execution
void printStepByStepExecution(vector<int> &sequence) {
    cout << "\nStep-by-step execution of a safe sequence:\n";
    vector<int> tempAvailable = favail;
    for (int i = 0; i < sequence.size(); i++) {
        int process = sequence[i];
        cout << "\nProcess " << process << " is executing..." << endl;
        cout << "Allocated resources: ";
        for (int j = 0; j < m; j++) {
            cout << allocated[process][j] << " ";
        }
        cout << "\nAvailable resources before execution: ";
        for (int j = 0; j < m; j++)
            cout << tempAvailable[j] << " ";
        for (int j = 0; j < m; j++) {
            tempAvailable[j] += allocated[process][j];
        }
        cout << "\nAvailable resources after Process " << process << " completes: ";
        for (int j = 0; j < m; j++)
            cout << tempAvailable[j] << " ";
        cout << "\nSafe Sequence so far: ";
        for (int k = 0; k <= i; k++) {
            cout << "P" << sequence[k];
            if (k != i)
                cout << " -> ";
        }
        cout << endl;
    }
}
int correct = 0;
// Function to find all safe sequences
void findSafeSequences(vector<int> &sequence, bool printFirst) {
    if (sequence.size() == n) {
        correct++;
        if (printFirst) {
            printStepByStepExecution(sequence);
            cout << "\nAll Safe Sequences: " << endl;
        }
        for (int i = 0; i < n; i++) {
            cout << "P" << sequence[i];
            if (i != n - 1)
                cout << " -> ";
        }
        cout << endl;
        return;
    }
    for (int i = 0; i < n; i++) {
        if (!finished[i]) {
            bool canExecute = true;
            for (int j = 0; j < m; j++) {
                if (need[i][j] > available[j]) {
                    canExecute = false;
                    break;
                }
            }
            if (canExecute) {
                for (int j = 0; j < m; j++)
                    available[j] += allocated[i][j];
                finished[i] = true;
                sequence.push_back(i);
                findSafeSequences(sequence, printFirst);

                printFirst = false;
                sequence.pop_back();
                finished[i] = false;
                for (int j = 0; j < m; j++)
                    available[j] -= allocated[i][j];
            }
        }
    }
}

int main() {
    // Input number of processes
    while (true) {
        cout << "Enter Number of processes (must be a positive integer): ";
        if (cin >> n && n > 0) {
            break;
        }
        cout << "Invalid input! Please enter a positive integer.\n";
        clearInputBuffer();
    }

    // Input number of resources
    while (true) {
        cout << "Enter Number of resources (must be a positive integer): ";
        if (cin >> m && m > 0) {
            break;
        }
        cout << "Invalid input! Please enter a positive integer.\n";
        clearInputBuffer();
    }

    // Resize vectors
    available.resize(m);
    maxMatrix.resize(n, vector<int>(m));
    allocated.resize(n, vector<int>(m));
    need.resize(n, vector<int>(m));
    finished.resize(n, false);

    // Input available resources
    for (int i = 0; i < m; i++) {
        while (true) {
            cout << "Enter available instances for resource " << i << " (must be non-negative integer): ";
            if (cin >> available[i] && available[i] >= 0) {
                break;
            }
            cout << "Invalid input! Please enter a non-negative integer.\n";
            clearInputBuffer();
        }
    }

    // Input max matrix
    cout << "\nEnter Max matrix (each process's maximum resource needs):\n";
    cout << "Format: Enter " << m << " integers per process separated by spaces\n";
    for (int i = 0; i < n; i++) {
        while (true) {
            cout << "Process " << i << " max needs: ";
            bool valid = true;
            for (int j = 0; j < m; j++) {
                if (!(cin >> maxMatrix[i][j])) {
                    valid = false;
                    break;
                }
                if (maxMatrix[i][j] < 0) {
                    valid = false;
                    break;
                }
            }
            if (valid) break;
            cout << "Invalid input! Please enter " << m << " non-negative integers.\n";
            clearInputBuffer();
        }
    }

    // Input allocated matrix
    cout << "\nEnter Allocated matrix (currently allocated resources for each process):\n";
    cout << "Format: Enter " << m << " integers per process separated by spaces\n";
    for (int i = 0; i < n; i++) {
        while (true) {
            cout << "Process " << i << " allocated resources: ";
            bool valid = true;
            for (int j = 0; j < m; j++) {
                if (!(cin >> allocated[i][j])) {
                    valid = false;
                    break;
                }
                if (allocated[i][j] < 0) {
                    valid = false;
                    break;
                }                
                // Check if allocated exceeds max
                if (allocated[i][j] > maxMatrix[i][j]) {
                    cout << "Error: Allocated resources cannot exceed maximum needs!\n";
                    valid = false;
                    break;
                }
            }
            if (valid) break;
            cout << "Invalid input! Please enter " << m << " non-negative integers that don't exceed maximum needs.\n";
            clearInputBuffer();
        }
    }

    // Calculate available after allocation
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            available[j] -= allocated[i][j];
            if (available[j] < 0) {
                cout << "\nError: System is in an unsafe state! Total allocated resources exceed available.\n";
                return 1;
            }
        }
    }
    favail = available;

    // Calculate and display need matrix
    cout << "\nNeed Matrix (Max - Allocated):\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            need[i][j] = maxMatrix[i][j] - allocated[i][j];
            cout << need[i][j] << " ";
        }
        cout << endl;
    }

    // Find safe sequences
    vector<int> sequence;
    cout << "\nFinding Safe Sequences...\n";
    findSafeSequences(sequence, true);
    if(!correct){
        cout << "No Safe sequences \n";
    }

    return 0;
}