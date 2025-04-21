#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>
using namespace std;

void displayResult(const vector<int>& allocation, const vector<int>& processSize, const vector<int>& fragment) {
    cout << left << setw(10) << "Process"
         << setw(15) << "Process Size"
         << setw(10) << "Block No"
         << setw(10) << "Fragment" << endl;
    cout << "---------------------------------------------\n";

    int totalFragment = 0;
    for (int i = 0; i < processSize.size(); i++) {
        cout << left << setw(10) << (i + 1)
             << setw(15) << processSize[i];

        if (allocation[i] != -1) {
            cout << setw(10) << (allocation[i] + 1)
                 << setw(10) << fragment[i];
            totalFragment += fragment[i];
        } else {
            cout << setw(10) << "NA"
                 << setw(10) << "-";
        }
        cout << endl;
    }
    cout << "\nTotal Internal Fragmentation: " << totalFragment << "\n";
}

void firstFit(const vector<int>& originalBlockSize, const vector<int>& processSize) {
    vector<int> blockSize = originalBlockSize;
    vector<int> allocation(processSize.size(), -1);
    vector<int> fragment(processSize.size(), 0);

    for (int i = 0; i < processSize.size(); i++) {
        for (int j = 0; j < blockSize.size(); j++) {
            if (blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                fragment[i] = blockSize[j] - processSize[i];
                blockSize[j] -= processSize[i];
                break;
            }
        }
    }

    cout << "\nFirst Fit Allocation:\n";
    displayResult(allocation, processSize, fragment);
}

void bestFit(const vector<int>& originalBlockSize, const vector<int>& processSize) {
    vector<int> blockSize = originalBlockSize;
    vector<int> allocation(processSize.size(), -1);
    vector<int> fragment(processSize.size(), 0);

    for (int i = 0; i < processSize.size(); i++) {
        int bestIdx = -1;
        for (int j = 0; j < blockSize.size(); j++) {
            if (blockSize[j] >= processSize[i]) {
                if (bestIdx == -1 || blockSize[j] < blockSize[bestIdx])
                    bestIdx = j;
            }
        }
        if (bestIdx != -1) {
            allocation[i] = bestIdx;
            fragment[i] = blockSize[bestIdx] - processSize[i];
            blockSize[bestIdx] -= processSize[i];
        }
    }

    cout << "\nBest Fit Allocation:\n";
    displayResult(allocation, processSize, fragment);
}

void worstFit(const vector<int>& originalBlockSize, const vector<int>& processSize) {
    vector<int> blockSize = originalBlockSize;
    vector<int> allocation(processSize.size(), -1);
    vector<int> fragment(processSize.size(), 0);

    for (int i = 0; i < processSize.size(); i++) {
        int worstIdx = -1;
        for (int j = 0; j < blockSize.size(); j++) {
            if (blockSize[j] >= processSize[i]) {
                if (worstIdx == -1 || blockSize[j] > blockSize[worstIdx])
                    worstIdx = j;
            }
        }
        if (worstIdx != -1) {
            allocation[i] = worstIdx;
            fragment[i] = blockSize[worstIdx] - processSize[i];
            blockSize[worstIdx] -= processSize[i];
        }
    }

    cout << "\nWorst Fit Allocation:\n";
    displayResult(allocation, processSize, fragment);
}

void nextFit(const vector<int>& originalBlockSize, const vector<int>& processSize) {
    vector<int> blockSize = originalBlockSize;
    vector<int> allocation(processSize.size(), -1);
    vector<int> fragment(processSize.size(), 0);

    int lastIndex = 0;
    for (int i = 0; i < processSize.size(); i++) {
        int j = lastIndex;
        int count = 0;
        while (count < blockSize.size()) {
            if (blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                fragment[i] = blockSize[j] - processSize[i];
                blockSize[j] -= processSize[i];
                lastIndex = j;
                break;
            }
            j = (j + 1) % blockSize.size();
            count++;
        }
    }

    cout << "\nNext Fit Allocation:\n";
    displayResult(allocation, processSize, fragment);
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    int m, n;

    cout << "Enter number of memory blocks: ";
    while (!(cin >> m) || m <= 0) {
        cout << "Invalid input. Enter a positive number: ";
        clearInputBuffer();
    }

    vector<int> blockSize(m);
    cout << "Enter size of each block:\n";
    for (int i = 0; i < m; i++) {
        cout << "Block " << i+1 << ": ";
        while (!(cin >> blockSize[i]) || blockSize[i] <= 0) {
            cout << "Invalid size. Enter again: ";
            clearInputBuffer();
        }
    }

    cout << "Enter number of processes: ";
    while (!(cin >> n) || n <= 0) {
        cout << "Invalid input. Enter a positive number: ";
        clearInputBuffer();
    }

    vector<int> processSize(n);
    cout << "Enter size of each process:\n";
    for (int i = 0; i < n; i++) {
        cout << "Process " << i+1 << ": ";
        while (!(cin >> processSize[i]) || processSize[i] <= 0) {
            cout << "Invalid size. Enter again: ";
            clearInputBuffer();
        }
    }

    int choice;
    do {
        cout << "\n--- Memory Allocation Menu ---\n";
        cout << "1. First Fit\n";
        cout << "2. Best Fit\n";
        cout << "3. Worst Fit\n";
        cout << "4. Next Fit\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        while (!(cin >> choice)) {
            cout << "Invalid input. Enter a number between 1 and 5: ";
            clearInputBuffer();
        }

        switch (choice) {
            case 1: firstFit(blockSize, processSize); break;
            case 2: bestFit(blockSize, processSize); break;
            case 3: worstFit(blockSize, processSize); break;
            case 4: nextFit(blockSize, processSize); break;
            case 5: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 5);

    return 0;
}
