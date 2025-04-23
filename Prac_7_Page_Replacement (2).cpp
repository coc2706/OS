#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

// Function to check if a page is already in the frame
bool isInFrame(int page, vector<int>& frame) {
    for (int i = 0; i < frame.size(); i++) {
        if (frame[i] == page) return true; // Page found
    }
    return false; // Page not found
}

// FCFS Page Replacement Algorithm
void FCFS(int pages[], int n, int capacity) {
    try {
        vector<int> frame; // Stores current pages in frame
        int pageFaults = 0; // Count of page faults
        int index = 0; // Index to replace page in FCFS order

        cout << "\nFCFS Page Replacement:\n";

        for (int i = 0; i < n; i++) {
            // If page is not in frame
            if (!isInFrame(pages[i], frame)) {
                pageFaults++; // Increment page fault
                if (frame.size() < capacity) {
                    frame.push_back(pages[i]); // Add page if space available
                } else {
                    frame[index] = pages[i]; // Replace oldest page
                    index = (index + 1) % capacity; // Move to next slot in circular order
                }
            }

            // Display current frame
            for (int f : frame) cout << f << " ";
            cout << "\n";
        }

        cout << "Total Page Faults (FCFS): " << pageFaults << "\n";
    } catch (exception& e) {
        cout << "Error in FCFS: " << e.what() << "\n";
    }
}

// LRU Page Replacement Algorithm
void LRU(int pages[], int n, int capacity) {
    try {
        vector<int> frame;
        int pageFaults = 0;

        cout << "\nLRU Page Replacement:\n";

        for (int i = 0; i < n; i++) {
            int page = pages[i];

            // If page is not in frame
            if (!isInFrame(page, frame)) {
                pageFaults++;
                if (frame.size() < capacity) {
                    frame.push_back(page); // Add page if space available
                } else {
                    // Find least recently used page
                    int lruIndex = 0, minLastUsed = i;
                    for (int j = 0; j < frame.size(); j++) {
                        int lastUsed = -1;
                        // Search backwards to find last use
                        for (int k = i - 1; k >= 0; k--) {
                            if (pages[k] == frame[j]) {
                                lastUsed = k;
                                break;
                            }
                        }
                        // Update if this page was used least recently
                        if (lastUsed < minLastUsed) {
                            minLastUsed = lastUsed;
                            lruIndex = j;
                        }
                    }
                    frame[lruIndex] = page; // Replace LRU page
                }
            }

            // Display current frame
            for (int f : frame) cout << f << " ";
            cout << "\n";
        }

        cout << "Total Page Faults (LRU): " << pageFaults << "\n";
    } catch (exception& e) {
        cout << "Error in LRU: " << e.what() << "\n";
    }
}

// Optimal Page Replacement Algorithm
void Optimal(int pages[], int n, int capacity) {
    try {
        vector<int> frame;
        int pageFaults = 0;

        cout << "\nOptimal Page Replacement:\n";

        for (int i = 0; i < n; i++) {
            int page = pages[i];

            // If page is not in frame
            if (!isInFrame(page, frame)) {
                pageFaults++;
                if (frame.size() < capacity) {
                    frame.push_back(page); // Add page if space available
                } else {
                    int replaceIndex = -1, farthest = i + 1;

                    // Find the page not needed for the longest time
                    for (int j = 0; j < frame.size(); j++) {
                        int k;
                        for (k = i + 1; k < n; k++) {
                            if (pages[k] == frame[j]) break; // Found future use
                        }

                        // Update farthest used page
                        if (k > farthest) {
                            farthest = k;
                            replaceIndex = j;
                        }

                        // If page is never used again
                        if (k == n) {
                            replaceIndex = j;
                            break;
                        }
                    }

                    // If no suitable farthest found, pick first
                    if (replaceIndex == -1) replaceIndex = 0;
                    frame[replaceIndex] = page; // Replace page
                }
            }

            // Display current frame
            for (int f : frame) cout << f << " ";
            cout << "\n";
        }

        cout << "Total Page Faults (Optimal): " << pageFaults << "\n";
    } catch (exception& e) {
        cout << "Error in Optimal: " << e.what() << "\n";
    }
}

// Main function
int main() {
    try {
        int n, capacity, choice;
        cout << "Enter number of pages: ";
        cin >> n;
        if (n <= 0) throw runtime_error("Number of pages must be positive.");

        int pages[n];
        cout << "Enter the page reference string:\n";
        for (int i = 0; i < n; i++) {
            cin >> pages[i]; // Input page references
        }

        cout << "Enter number of frames: ";
        cin >> capacity;
        if (capacity <= 0) throw runtime_error("Number of frames must be positive.");

        // Menu for algorithm selection
        cout << "\nChoose algorithm:\n";
        cout << "1. FCFS\n2. LRU\n3. Optimal\n";
        cout << "Enter your choice: ";
        cin >> choice;

        // Call respective algorithm
        switch (choice) {
            case 1: FCFS(pages, n, capacity); break;
            case 2: LRU(pages, n, capacity); break;
            case 3: Optimal(pages, n, capacity); break;
            default: cout << "Invalid choice.\n";
        }
    } catch (exception& e) {
        cout << "Error: " << e.what() << "\n";
    }

    return 0;
}
