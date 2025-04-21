#include <iostream>
#include <vector>
using namespace std;

void calculateNeed(vector<vector<int>>& max, vector<vector<int>>& allocation, vector<vector<int>>& need, int n, int m) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = max[i][j] - allocation[i][j];
}

bool isSafe(vector<vector<int>>& allocation, vector<vector<int>>& need, vector<int>& available, int n, int m) {
    vector<bool> finished(n, false);
    vector<int> work = available;
    vector<int> safeSequence;

    cout << "\nNeed Matrix:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
            cout << need[i][j] << " ";
        cout << endl;
    }

    cout << "\nChecking for Safe Sequence...\n";
    int count = 0;
    while (count < n) {
        bool found = false;
        for (int i = 0; i < n; i++) {
            if (!finished[i]) {
                bool possible = true;
                for (int j = 0; j < m; j++) {
                    if (need[i][j] > work[j]) {
                        possible = false;
                        break;
                    }
                }

                if (possible) {
                    cout << "Process P" << i << " is executing.\n";
                    for (int j = 0; j < m; j++)
                        work[j] += allocation[i][j];
                    safeSequence.push_back(i);
                    finished[i] = true;
                    found = true;
                    count++;
                }
            }
        }

        if (!found) {
            cout << "\nSystem is in an UNSAFE state. No safe sequence exists.\n";
            return false;
        }
    }

    cout << "\nSystem is in a SAFE state.\nSafe Sequence: ";
    for (int i = 0; i < safeSequence.size(); i++)
        cout << "P" << safeSequence[i] << (i == safeSequence.size() - 1 ? "\n" : " -> ");
    return true;
}

int main() {
    int n, m;
    cout << "Enter number of processes: ";
    cin >> n;
    cout << "Enter number of resource types: ";
    cin >> m;

    vector<vector<int>> allocation(n, vector<int>(m));
    vector<vector<int>> max(n, vector<int>(m));
    vector<vector<int>> need(n, vector<int>(m));
    vector<int> available(m);

    cout << "\nEnter Allocation Matrix:\n";
    for (int i = 0; i < n; i++) {
        cout << "P" << i << ": ";
        for (int j = 0; j < m; j++)
            cin >> allocation[i][j];
    }

    cout << "\nEnter Maximum Matrix (must be >= allocation):\n";
    for (int i = 0; i < n; i++) {
        while (true) {
            cout << "P" << i << ": ";
            bool valid = true;
            for (int j = 0; j < m; j++) {
                cin >> max[i][j];
                if (max[i][j] < allocation[i][j]) {
                    valid = false;
                }
            }

            if (!valid) {
                cout << "Error: Maximum values must be >= Allocation values for P" << i << ". Re-enter row.\n";
            } else {
                break;
            }
        }
    }

    cout << "\nEnter Available Resources:\n";
    for (int j = 0; j < m; j++)
        cin >> available[j];

    calculateNeed(max, allocation, need, n, m);
    isSafe(allocation, need, available, n, m);

    return 0;
}
