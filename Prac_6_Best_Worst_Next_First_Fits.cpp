#include <iostream>
#include <exception>  // For exception handling
using namespace std;

// First Fit Memory Allocation
void firstFit(int block[], int m, int process[], int n) 
{
    cout << "\nFirst Fit:\n";
    int alloc[m];  // Copy of original block sizes
    for (int i = 0; i < m; i++) 
        alloc[i] = block[i];

    for (int i = 0; i < n; i++) 
    {
        bool allocated = false;
        for (int j = 0; j < m; j++) 
        {
            // Allocate if block is big enough
            if (alloc[j] >= process[i]) 
            {
                cout << "Process " << i + 1 << " -> Block " << j + 1 << "\n";
                alloc[j] -= process[i];  // Reduce block size
                allocated = true;
                break;
            }
        }
        // If not allocated
        if (!allocated) 
            cout << "Process " << i + 1 << " -> Not Allocated\n";
    }
}

// Best Fit Memory Allocation
void bestFit(int block[], int m, int process[], int n) {
    cout << "\nBest Fit:\n";
    int alloc[m];
    for (int i = 0; i < m; i++) 
        alloc[i] = block[i];

    for (int i = 0; i < n; i++) {
        int best = -1;
        for (int j = 0; j < m; j++) {
            // Find the smallest suitable block
            if (alloc[j] >= process[i]) {
                if (best == -1 || alloc[j] < alloc[best])
                    best = j;
            }
        }
        if (best != -1) {
            cout << "Process " << i + 1 << " -> Block " << best + 1 << "\n";
            alloc[best] -= process[i];
        } 
        else 
            cout << "Process " << i + 1 << " -> Not Allocated\n";
    }
}

// Worst Fit Memory Allocation
void worstFit(int block[], int m, int process[], int n) {
    cout << "\nWorst Fit:\n";
    int alloc[m];
    for (int i = 0; i < m; i++) 
        alloc[i] = block[i];
        
    for (int i = 0; i < n; i++) {
        int worst = -1;
        for (int j = 0; j < m; j++) {
            // Find the largest suitable block
            if (alloc[j] >= process[i]) {
                if (worst == -1 || alloc[j] > alloc[worst])
                    worst = j;
            }
        }
        if (worst != -1) {
            cout << "Process " << i + 1 << " -> Block " << worst + 1 << "\n";
            alloc[worst] -= process[i];
        } 
        else 
            cout << "Process " << i + 1 << " -> Not Allocated\n";
    }
}

// Next Fit Memory Allocation
void nextFit(int block[], int m, int process[], int n) {
    cout << "\nNext Fit:\n";
    int alloc[m];
    for (int i = 0; i < m; i++) 
        alloc[i] = block[i];

    int pos = 0;  // Keeps track of last allocation position
    
    for (int i = 0; i < n; i++) {
        bool allocated = false;
        for (int j = 0; j < m; j++) {
            int idx = (pos + j) % m;  // Circular traversal
            if (alloc[idx] >= process[i]) {
                cout << "Process " << i + 1 << " -> Block " << idx + 1 << "\n";
                alloc[idx] -= process[i];
                pos = idx;  // Update last position
                allocated = true;
                break;
            }
        }
        if (!allocated) 
            cout << "Process " << i + 1 << " -> Not Allocated\n";
    }
}

int main() {
    try {
        int m, n;
        cout << "Enter number of memory blocks: ";
        cin >> m;
        if (m <= 0) throw runtime_error("Number of memory blocks must be positive.");

        int block[m];
        cout << "Enter sizes of memory blocks:\n";
        for (int i = 0; i < m; i++) cin >> block[i];

        cout << "Enter number of processes: ";
        cin >> n;
        if (n <= 0) throw runtime_error("Number of processes must be positive.");

        int process[n];
        cout << "Enter sizes of processes:\n";
        for (int i = 0; i < n; i++) cin >> process[i];

        // Call all memory allocation strategies
        firstFit(block, m, process, n);
        bestFit(block, m, process, n);
        worstFit(block, m, process, n);
        nextFit(block, m, process, n);
    }
    catch (exception &e) {
        // Handle any standard exception
        cerr << "Exception: " << e.what() << endl;
    }
    catch (...) {
        // Handle unknown exceptions
        cerr << "An unknown error occurred." << endl;
    }

    return 0;
}
