#include <iostream>
#include <vector>
#include <unordered_map>
#include <limits>
#include <algorithm>
using namespace std;

void inputReferenceString(vector<int> &pages, int &n) {
    cout << "Enter number of page references: ";
    while (!(cin >> n) || n <= 0) {
        cout << "Invalid input! Enter a positive integer: ";
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    pages.resize(n);
    cout << "Enter the page reference string (space separated):\n";
    for (int i = 0; i < n; ++i) {
        while (!(cin >> pages[i])) {
            cout << "Invalid input! Enter an integer: ";
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

bool isPresent(int page, const vector<int>& frames) {
    return find(frames.begin(), frames.end(), page) != frames.end();
}

void printFrames(const vector<int>& frames) {
    cout << "[ ";
    for (int val : frames) cout << val << " ";
    cout << "]";
}

void FCFS(const vector<int>& pages, int f) {
    vector<int> frames;
    int pageFaults = 0;
    int pointer = 0;

    cout << "\n--- FCFS Page Replacement Step-by-Step ---\n";
    for (int page : pages) {
        cout << "Page: " << page << " --> ";
        if (!isPresent(page, frames)) {
            if (frames.size() < f) {
                frames.push_back(page);
            } else {
                frames[pointer] = page;
                pointer = (pointer + 1) % f;
            }
            pageFaults++;
            printFrames(frames);
            cout << " --> Page Fault\n";
        } else {
            printFrames(frames);
            cout << " --> Hit\n";
        }
    }
    cout << "Total Page Faults using FCFS: " << pageFaults << endl;
}

void LRU(const vector<int>& pages, int f) {
    vector<int> frames;
    unordered_map<int, int> lastUsed;
    int pageFaults = 0;

    cout << "\n--- LRU Page Replacement Step-by-Step ---\n";
    for (int i = 0; i < pages.size(); ++i) {
        int page = pages[i];
        cout << "Page: " << page << " --> ";
        if (!isPresent(page, frames)) {
            if (frames.size() < f) {
                frames.push_back(page);
            } else {
                int lruIndex = 0, minTime = i;
                for (int j = 0; j < frames.size(); ++j) {
                    if (lastUsed[frames[j]] < minTime) {
                        minTime = lastUsed[frames[j]];
                        lruIndex = j;
                    }
                }
                frames[lruIndex] = page;
            }
            pageFaults++;
            printFrames(frames);
            cout << " --> Page Fault\n";
        } else {
            printFrames(frames);
            cout << " --> Hit\n";
        }
        lastUsed[page] = i;
    }
    cout << "Total Page Faults using LRU: " << pageFaults << endl;
}

void Optimal(const vector<int>& pages, int f) {
    vector<int> frames;
    int pageFaults = 0;

    cout << "\n--- Optimal Page Replacement Step-by-Step ---\n";
    for (int i = 0; i < pages.size(); ++i) {
        int page = pages[i];
        cout << "Page: " << page << " --> ";
        if (!isPresent(page, frames)) {
            if (frames.size() < f) {
                frames.push_back(page);
            } else {
                int farthest = -1, index = -1;
                for (int j = 0; j < frames.size(); ++j) {
                    int k;
                    for (k = i + 1; k < pages.size(); ++k) {
                        if (pages[k] == frames[j]) break;
                    }
                    if (k > farthest) {
                        farthest = k;
                        index = j;
                    }
                }
                frames[index] = page;
            }
            pageFaults++;
            printFrames(frames);
            cout << " --> Page Fault\n";
        } else {
            printFrames(frames);
            cout << " --> Hit\n";
        }
    }
    cout << "Total Page Faults using Optimal: " << pageFaults << endl;
}

int main() {
    int choice, frames;
    vector<int> pages;
    int n;

    cout << "===== Page Replacement Algorithms =====" << endl;

    do {
        cout << "\nMenu:\n";
        cout << "1. Enter page reference string\n";
        cout << "2. FCFS Algorithm\n";
        cout << "3. LRU Algorithm\n";
        cout << "4. Optimal Algorithm\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        while (!(cin >> choice) || choice < 0 || choice > 4) {
            cout << "Invalid choice! Try again: ";
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1:
                inputReferenceString(pages, n);
                cout << "Enter number of frames: ";
                while (!(cin >> frames) || frames <= 0) {
                    cout << "Invalid input! Enter a positive integer: ";
                    cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                break;
            case 2:
                if (pages.empty()) {
                    cout << "Please enter the page reference string first.\n";
                } else {
                    FCFS(pages, frames);
                }
                break;
            case 3:
                if (pages.empty()) {
                    cout << "Please enter the page reference string first.\n";
                } else {
                    LRU(pages, frames);
                }
                break;
            case 4:
                if (pages.empty()) {
                    cout << "Please enter the page reference string first.\n";
                } else {
                    Optimal(pages, frames);
                }
                break;
            case 0:
                cout << "Exiting program.\n";
                break;
        }
    } while (choice != 0);

    return 0;
}
