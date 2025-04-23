#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <stdexcept>
using namespace std;

// FCFS (First-Come, First-Serve) Disk Scheduling
void FCFS(int n, vector<int>& req, int head, int disk) {
    int seek = 0;
    cout << head << " -> ";
    for (int r : req) {
        seek += abs(r - head); // calculate distance
        head = r;
        cout << r << " ";
    }
    cout << endl << "FCFS Seek time = " << seek << endl;
}

// SSTF (Shortest Seek Time First) Disk Scheduling
void SSTF(int n, vector<int>& req, int head, int disk) {
    int seek = 0;
    vector<bool> visited(n, false); // track which requests are done
    cout << head << " -> ";

    for (int i = 0; i < n; i++) {
        int minDist = INT_MAX, index = -1;

        // find nearest unvisited request
        for (int j = 0; j < n; j++) {
            if (!visited[j] && abs(req[j] - head) < minDist) {
                minDist = abs(req[j] - head);
                index = j;
            }
        }

        visited[index] = true;
        seek += minDist;
        head = req[index];
        cout << head << " ";
    }

    cout << endl << "SSTF Seek time = " << seek << endl;
}

// SCAN (Elevator Algorithm) Disk Scheduling
void SCAN(int n, vector<int>& req, int head, int disk) {
    int seek = 0;
    vector<int> left, right;

    // separate requests to the left and right of head
    for (int r : req) {
        if (r < head) left.push_back(r);
        else right.push_back(r);
    }

    // sort in required order
    sort(left.begin(), left.end(), greater<int>());
    sort(right.begin(), right.end());

    cout << head << " -> ";

    // move right (towards higher tracks)
    for (int r : right) {
        seek += abs(r - head);
        head = r;
        cout << r << " ";
    }

    // go to end and reverse to left
    if (!left.empty()) {
        seek += abs((disk - 1) - head); // go to end
        head = disk - 1;
        seek += abs(head - left[0]);   // reverse to leftmost

        for (int r : left) {
            seek += abs(r - head);
            head = r;
            cout << r << " ";
        }
    }

    cout << endl << "SCAN Seek time = " << seek << endl;
}

// C-SCAN (Circular SCAN) Disk Scheduling
void CSCAN(int n, vector<int>& req, int head, int disk) {
    int seek = 0;
    vector<int> left, right;

    // separate requests to the left and right of head
    for (int r : req) {
        if (r < head) left.push_back(r);
        else right.push_back(r);
    }

    // sort both parts
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    cout << head << " -> ";

    // move right first
    for (int r : right) {
        seek += abs(r - head);
        head = r;
        cout << r << " ";
    }

    // jump from end to start
    if (!left.empty()) {
        seek += abs((disk - 1) - head); // go to last track
        head = 0;                       // jump to start (0)
        seek += disk - 1;               // jump cost

        for (int r : left) {
            seek += abs(r - head);
            head = r;
            cout << r << " ";
        }
    }

    cout << endl << "C-SCAN Seek time = " << seek << endl;
}

int main() {
    int n;
    try {
        cout << "Enter number of requests: ";
        cin >> n;
        if (cin.fail()) throw invalid_argument("Invalid input for number of requests");

        vector<int> req(n);
        cout << "Enter the requests: ";
        for (int i = 0; i < n; i++) {
            cin >> req[i];
            if (cin.fail()) throw invalid_argument("Invalid input for request values");
        }

        int head;
        cout << "Enter head: ";
        cin >> head;
        if (cin.fail()) throw invalid_argument("Invalid input for head");

        int disk;
        cout << "Enter the Disk size: ";
        cin >> disk;
        if (cin.fail()) throw invalid_argument("Invalid input for disk size");

        // Menu to choose scheduling algorithm
        while (true) {
            cout << "\n1. FCFS \n2. SSTF \n3. SCAN \n4. C-SCAN \n5. Exit";
            cout << "\nEnter the Choice: ";
            int ch;
            cin >> ch;

            if (cin.fail()) throw invalid_argument("Invalid input for choice");

            switch (ch) {
                case 1:
                    FCFS(n, req, head, disk);
                    break;
                case 2:
                    SSTF(n, req, head, disk);
                    break;
                case 3:
                    SCAN(n, req, head, disk);
                    break;
                case 4:
                    CSCAN(n, req, head, disk);
                    break;
                case 5:
                    cout << "Exit\n";
                    return 0;
                default:
                    cout << "Enter Valid Choice!!" << endl;
                    break;
            }
        }
    }
    catch (const invalid_argument& e) {
        cout << "Error: " << e.what() << endl;
    }
    catch (...) {
        cout << "An unexpected error occurred." << endl;
    }

    return 0;
}
