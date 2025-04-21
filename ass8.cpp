#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <string>
using namespace std;

// FIFO Algorithm
int fifo(int head, int req, const vector<int>& arr) {
    int seek = 0;
    int current = head;
    for (int i = 0; i < req; i++) {
        seek += abs(current - arr[i]);
        current = arr[i];
    }
    return seek;
}

// SSTF Algorithm
int sstf(int head, int req, vector<int>& arr) {
    int seek = 0;
    int current = head;
    vector<bool> visited(req, false);

    for (int i = 0; i < req; i++) {
        int min = 1e9;
        int index = -1;
        for (int j = 0; j < req; j++) {
            if (!visited[j] && abs(current - arr[j]) < min) {
                min = abs(current - arr[j]);
                index = j;
            }
        }
        visited[index] = true;
        seek += min;
        current = arr[index];
    }
    return seek;
}

// SCAN Algorithm
int scan(int head, int req, vector<int>& arr, int tracks, const string& direction) {
    int seek = 0;
    int current = head;

    arr.push_back(head);
    sort(arr.begin(), arr.end());

    int idx = find(arr.begin(), arr.end(), head) - arr.begin();

    if (direction == "right") {
        for (int i = idx + 1; i < arr.size(); i++) {
            seek += abs(current - arr[i]);
            current = arr[i];
        }
        if (current != tracks - 1) {
            seek += abs(current - (tracks - 1));
            current = tracks - 1;
        }
        for (int i = idx - 1; i >= 0; i--) {
            seek += abs(current - arr[i]);
            current = arr[i];
        }
    } else if (direction == "left") {
        for (int i = idx - 1; i >= 0; i--) {
            seek += abs(current - arr[i]);
            current = arr[i];
        }
        if (current != 0) {
            seek += abs(current - 0);
            current = 0;
        }
        for (int i = idx + 1; i < arr.size(); i++) {
            seek += abs(current - arr[i]);
            current = arr[i];
        }
    } else {
        cout << "Invalid direction for SCAN. Please choose 'left' or 'right'.\n";
        return -1;
    }

    return seek;
}

// CSCAN Algorithm
int cscan(int head, int req, vector<int>& arr, int tracks, const string& direction) {
    int seek = 0;
    int current = head;

    arr.push_back(head);
    sort(arr.begin(), arr.end());

    int idx = find(arr.begin(), arr.end(), head) - arr.begin();

    if (direction == "right") {
        for (int i = idx + 1; i < arr.size(); i++) {
            seek += abs(current - arr[i]);
            current = arr[i];
        }
        if (current != tracks - 1) {
            seek += abs(current - (tracks - 1));
            current = tracks - 1;
        }
        current = 0;
        seek += (tracks - 1);
        for (int i = 0; i < idx; i++) {
            seek += abs(current - arr[i]);
            current = arr[i];
        }
    } else if (direction == "left") {
        for (int i = idx - 1; i >= 0; i--) {
            seek += abs(current - arr[i]);
            current = arr[i];
        }
        if (current != 0) {
            seek += abs(current - 0);
            current = 0;
        }
        current = tracks - 1;
        seek += (tracks - 1);
        for (int i = arr.size() - 1; i > idx; i--) {
            seek += abs(current - arr[i]);
            current = arr[i];
        }
    } else {
        cout << "Invalid direction for CSCAN. Please choose 'left' or 'right'.\n";
        return -1;
    }

    return seek;
}

int main() {
    int tracks, head, req, ch;
    string direction;

    cout << "Enter the number of tracks: ";
    cin >> tracks;

    cout << "Enter initial head position: ";
    cin >> head;

    // Validating head position
    if (head < 0 || head >= tracks) {
        cout << "Error: Head position out of bounds!\n";
        return -1;
    }

    cout << "Enter number of disk requests: ";
    cin >> req;

    // Validating number of requests
    if (req <= 0) {
        cout << "Error: Invalid number of disk requests!\n";
        return -1;
    }

    vector<int> arr(req);
    cout << "Enter the disk requests:\n";
    for (int i = 0; i < req; i++) {
        cin >> arr[i];
        // Validating request within bounds
        if (arr[i] < 0 || arr[i] >= tracks) {
            cout << "Error: Request out of bounds!\n";
            return -1;
        }
    }

    cout << "\nChoose Algorithm:\n";
    cout << "1. FIFO\n2. SSTF\n3. SCAN\n4. CSCAN\n";
    cin >> ch;

    if (ch == 3 || ch == 4) {
        cout << "Enter direction (left/right): ";
        cin >> direction;
    }

    int totalSeek = 0;
    switch (ch) {
        case 1:
            totalSeek = fifo(head, req, arr);
            cout << "Total Seek Time (FIFO): " << totalSeek << endl;
            break;
        case 2:
            totalSeek = sstf(head, req, arr);
            cout << "Total Seek Time (SSTF): " << totalSeek << endl;
            break;
        case 3:
            totalSeek = scan(head, req, arr, tracks, direction);
            if (totalSeek != -1)
                cout << "Total Seek Time (SCAN - " << direction << "): " << totalSeek << endl;
            break;
        case 4:
            totalSeek = cscan(head, req, arr, tracks, direction);
            if (totalSeek != -1)
                cout << "Total Seek Time (CSCAN - " << direction << "): " << totalSeek << endl;
            break;
        default:
            cout << "Invalid choice.\n";
    }

    return 0;
}
