#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <climits>
#include<queue>

using namespace std;

struct Process {
    int pid;
    int burst_time;
    int arrival_time;
    int priority;
    int remaining_time;
    int completion_time;
    int waiting_time;
    int turnaround_time;
    bool completed = false;
};

void printResults(vector<Process> &proc) {
    float total_wt = 0, total_tat = 0;
    cout << "\nProcess\tBT\tAT\tWT\tTAT\n";
    for (auto &p : proc) {
        cout << "P" << p.pid << "\t" << p.burst_time << "\t" << p.arrival_time
             << "\t" << p.waiting_time << "\t" << p.turnaround_time << "\n";
        total_wt += p.waiting_time;
        total_tat += p.turnaround_time;
    }
    cout << fixed << setprecision(2);
    cout << "\nAverage Waiting Time: " << total_wt / proc.size();
    cout << "\nAverage Turnaround Time: " << total_tat / proc.size() << "\n";
}

void fcfs(vector<Process> proc) {
    sort(proc.begin(), proc.end(), [](Process a, Process b) {
        return a.arrival_time < b.arrival_time;
    });

    int time = 0;
    for (auto &p : proc) {
        time = max(time, p.arrival_time);
        p.waiting_time = time - p.arrival_time;
        time += p.burst_time;
        p.turnaround_time = p.waiting_time + p.burst_time;
    }

    cout << "\n--- FCFS Scheduling ---\n";
    printResults(proc);
}

void sjf(vector<Process> proc) {
    int n = proc.size(), completed = 0, time = 0;

    while (completed < n) {
        int idx = -1;
        int min_bt = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!proc[i].completed && proc[i].arrival_time <= time && proc[i].burst_time < min_bt) {
                min_bt = proc[i].burst_time;
                idx = i;
            }
        }

        if (idx != -1) {
            proc[idx].waiting_time = time - proc[idx].arrival_time;
            time += proc[idx].burst_time;
            proc[idx].turnaround_time = proc[idx].waiting_time + proc[idx].burst_time;
            proc[idx].completed = true;
            completed++;
        } else {
            time++;
        }
    }

    cout << "\n--- SJF Scheduling (Non-Preemptive) ---\n";
    printResults(proc);
}

void roundRobin(vector<Process> proc, int quantum) {
    int time = 0;
    queue<Process*> q;
    int n = proc.size();
    int completed = 0;

    for (auto &p : proc) p.remaining_time = p.burst_time;

    sort(proc.begin(), proc.end(), [](Process a, Process b) {
        return a.arrival_time < b.arrival_time;
    });

    int i = 0;
    q.push(&proc[0]);
    i = 1;

    while (!q.empty()) {
        Process* curr = q.front(); q.pop();

        if (curr->remaining_time > quantum) {
            time = max(time, curr->arrival_time);
            time += quantum;
            curr->remaining_time -= quantum;
        } else {
            time = max(time, curr->arrival_time);
            time += curr->remaining_time;
            curr->waiting_time = time - curr->arrival_time - curr->burst_time;
            curr->turnaround_time = curr->waiting_time + curr->burst_time;
            curr->remaining_time = 0;
            completed++;
        }

        while (i < n && proc[i].arrival_time <= time) {
            q.push(&proc[i]);
            i++;
        }

        if (curr->remaining_time > 0) q.push(curr);
        else curr->completed = true;

        if (q.empty() && i < n) {
            q.push(&proc[i]);
            i++;
        }
    }

    cout << "\n--- Round Robin Scheduling (Quantum = " << quantum << ") ---\n";
    printResults(proc);
}

void priorityScheduling(vector<Process> proc) {
    int n = proc.size(), completed = 0, time = 0;

    while (completed < n) {
        int idx = -1;
        int highest_priority = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!proc[i].completed && proc[i].arrival_time <= time && proc[i].priority < highest_priority) {
                highest_priority = proc[i].priority;
                idx = i;
            }
        }

        if (idx != -1) {
            proc[idx].waiting_time = time - proc[idx].arrival_time;
            time += proc[idx].burst_time;
            proc[idx].turnaround_time = proc[idx].waiting_time + proc[idx].burst_time;
            proc[idx].completed = true;
            completed++;
        } else {
            time++;
        }
    }

    cout << "\n--- Priority Scheduling (Non-Preemptive) ---\n";
    printResults(proc);
}

int main() {
    int choice, n, quantum;
    vector<Process> processes;

    do {
        cout << "\n===== CPU Scheduling Algorithms =====\n";
        cout << "1. FCFS\n2. SJF (Non-Preemptive)\n3. Round Robin\n4. Priority Scheduling\n5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice >= 1 && choice <= 4) {
            processes.clear();
            cout << "Enter number of processes: ";
            cin >> n;

            for (int i = 0; i < n; i++) {
                Process p;
                p.pid = i + 1;
                cout << "Enter Burst Time and Arrival Time for P" << p.pid << ": ";
                cin >> p.burst_time >> p.arrival_time;
                if (choice == 4) {
                    cout << "Enter Priority for P" << p.pid << ": ";
                    cin >> p.priority;
                }
                processes.push_back(p);
            }

            if (choice == 1)
                fcfs(processes);
            else if (choice == 2)
                sjf(processes);
            else if (choice == 3) {
                cout << "Enter Time Quantum: ";
                cin >> quantum;
                roundRobin(processes, quantum);
            }
            else if (choice == 4)
                priorityScheduling(processes);
        }

    } while (choice != 5);

    return 0;
}
