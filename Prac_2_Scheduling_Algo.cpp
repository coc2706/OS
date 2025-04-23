#include <iostream>
#include <vector>
#include <algorithm>
#include <limits> // for numeric_limits
#include <stdexcept>  // For exception handling
using namespace std;

// Function to print final results
void printResults(int pid[], int at[], int bt[], int wt[], int tat[], int ct[], int n) 
{
    cout << "Process Id   Arrival Time   Burst Time   Completion Time   TurnAround Time   Waiting Time\n";
    float twt = 0.0, ttat = 0.0;
    for (int i = 0; i < n; i++) 
    {
        cout << " P" << pid[i] << "\t\t" << at[i] << "\t\t" << bt[i] << "\t\t" << ct[i] << "\t\t" << tat[i] << "\t\t" << wt[i] << "\n";
        twt += wt[i];
        ttat += tat[i];
    }
    cout << "Average Waiting Time = " << twt / n << "\n";
    cout << "Average TurnAround Time = " << ttat / n << "\n";
}

// Function to print Gantt chart
void printGanttChart(vector <int> &gid) 
{
    cout<<"Gant Chart: ";
    for(int i=0; i<gid.size(); i++)
    {
        cout<<"P"<<gid[i]<< " ";
    }
    cout<<endl;
}

// First Come First Serve Scheduling
void fcfs(int pid[], int bt[], int at[], int n) 
{
    int wt[n], tat[n], ct[n];
    vector <int> gid;

    // Sort processes based on arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (at[j] > at[j + 1]) {
                swap(at[j], at[j + 1]);
                swap(bt[j], bt[j + 1]);
                swap(pid[j], pid[j + 1]);
            }
        }
    }

    // First process calculations
    ct[0] = at[0] + bt[0];
    tat[0] = ct[0] - at[0];
    wt[0] = tat[0] - bt[0];
    gid.push_back(1);
    
    // Remaining processes
    for (int i = 1; i < n; i++) 
    {
        if (ct[i - 1] > at[i]) 
            ct[i] = ct[i - 1] + bt[i];
        else 
            ct[i] = at[i] + bt[i];

        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
        gid.push_back(i+1);
    }

    cout << "\nFCFS Scheduling\n";
    printResults(pid, at, bt, wt, tat, ct, n);
    printGanttChart(gid);
}

// Shortest Job First (Non-Preemptive)
void sjfNonPreemptive(int pid[], int bt[], int at[], int n) 
{
    int wt[n], tat[n], ct[n], completed[n] = {0}, time = 0, count = 0;
    vector <int> gid;

    while (count < n) 
    {
        int min_bt = 1e9, idx = -1;

        // Find the shortest job available at current time
        for (int i = 0; i < n; i++) 
        {
            if (!completed[i] && at[i] <= time && bt[i] < min_bt) 
            {
                min_bt = bt[i];
                idx = i;
            }
        }

        if (idx == -1) 
        {
            time++; // No process available
            continue;
        }

        time += bt[idx]; // Process execution
        ct[idx] = time;
        tat[idx] = ct[idx] - at[idx];
        wt[idx] = tat[idx] - bt[idx];
        completed[idx] = 1;
        count++;
        gid.push_back(idx+1);
    }

    cout << "\nNon-Preemptive SJF Scheduling\n";
    printResults(pid, at, bt, wt, tat, ct, n);
    printGanttChart(gid);
}

// Shortest Job First (Preemptive)
void sjfPreemptive(int pid[], int bt[], int at[], int n) 
{
    int remaining_bt[15], wt[15], tat[15], ct[15];
    int completed = 0, time = 0, min_bt, idx;
    vector <int> gid;

    // Initialize remaining burst times
    for (int i = 0; i < n; i++) 
        remaining_bt[i] = bt[i];

    while (completed < n) 
    {
        min_bt = 1e9;
        idx = -1;

        // Find shortest remaining burst time
        for (int i = 0; i < n; i++) 
        {
            if (at[i] <= time && remaining_bt[i] > 0 && remaining_bt[i] < min_bt) 
            {
                min_bt = remaining_bt[i];
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        remaining_bt[idx]--;
        time++;
        gid.push_back(idx+1);

        if (remaining_bt[idx] == 0) {
            ct[idx] = time;
            tat[idx] = ct[idx] - at[idx];
            wt[idx] = tat[idx] - bt[idx];
            completed++;
        }
    }

    cout << "\nPreemptive SJF Scheduling\n";
    printResults(pid, at, bt, wt, tat, ct, n);
    printGanttChart(gid);
}

// Round Robin Scheduling
void roundRobin(int pid[], int bt[], int at[], int n) 
{
    int wt[15] = {0}, tat[15] = {0}, ct[15] = {0}, remaining_bt[15];
    int quantum, time = 0, completed = 0, flag = 0;
    cout << "Enter time quantum: ";
    cin >> quantum;
    vector <int> gid;

    // Initialize remaining burst times
    for (int i = 0; i < n; i++) 
        remaining_bt[i] = bt[i];

    while (completed < n) 
    {
        flag = 0;
        for (int i = 0; i < n; i++) 
        {
            // Process ready and has remaining time
            if (remaining_bt[i] > 0 && at[i] <= time) 
            {
                flag = 1;
                if (remaining_bt[i] > quantum) 
                {
                    time += quantum;
                    remaining_bt[i] -= quantum;
                    gid.push_back(i+1);
                } 
                else 
                {
                    time += remaining_bt[i];
                    ct[i] = time;
                    tat[i] = ct[i] - at[i];
                    wt[i] = tat[i] - bt[i];
                    remaining_bt[i] = 0;
                    completed++;
                    gid.push_back(i+1);
                }
            }
        }

        if (!flag) time++; // No process ready
    }

    cout << "\nRound Robin Scheduling\n";
    printResults(pid, at, bt, wt, tat, ct, n);
    printGanttChart(gid);
}

// Priority Scheduling (Non-preemptive)
void priorityScheduling(int pid[], int bt[], int at[], int pri[], int n) 
{
    int wt[15], tat[15], ct[15], completed[15] = {0}, time = 0, count = 0;
    vector <int> gid;

    while (count < n) 
    {
        int min_priority = 1000, idx = -1;

        // Find process with highest priority (lowest number)
        for (int i = 0; i < n; i++) {
            if (!completed[i] && at[i] <= time && pri[i] < min_priority) {
                min_priority = pri[i];
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        time += bt[idx];
        ct[idx] = time;
        tat[idx] = ct[idx] - at[idx];
        wt[idx] = tat[idx] - bt[idx];
        completed[idx] = 1;
        count++;
        gid.push_back(idx+1);
    }

    cout << "\nPriority Scheduling\n";
    printResults(pid, at, bt, wt, tat, ct, n);
    printGanttChart(gid);
}

// Main driver function
int main() 
{
    int pid[15], bt[15], at[15], pri[15], n, choice;

    try {
        cout << "Enter the number of processes: ";
        cin >> n;
        if (cin.fail() || n <= 0) throw runtime_error("Invalid input for number of processes.");

        // Input for arrival and burst time
        for (int i = 0; i < n; i++) {
            pid[i] = i + 1;
            cout << "Enter Arrival Time and Burst Time for P" << pid[i] << ": ";
            cin >> at[i] >> bt[i];
            if (cin.fail() || at[i] < 0 || bt[i] <= 0) throw runtime_error("Invalid Arrival or Burst Time.");
        }

        // Menu for scheduling choices
        do {
            cout << "\nChoose Scheduling Algorithm:\n";
            cout << "1. First Come First Serve (FCFS)\n";
            cout << "2. Shortest Job First (Non-Preemptive)\n";
            cout << "3. Shortest Job First (Preemptive)\n";
            cout << "4. Round Robin\n";
            cout << "5. Priority Scheduling\n";
            cout << "6. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;
            if (cin.fail()) throw runtime_error("Invalid input for choice.");

            switch (choice) {
                case 1:
                    fcfs(pid, bt, at, n);
                    break;
                case 2:
                    sjfNonPreemptive(pid, bt, at, n);
                    break;
                case 3:
                    sjfPreemptive(pid, bt, at, n);
                    break;
                case 4:
                    roundRobin(pid, bt, at, n);
                    break;
                case 5:
                    cout << "Enter priority of all the processes: ";
                    for (int i = 0; i < n; i++) {
                        cin >> pri[i];
                        if (cin.fail()) throw runtime_error("Invalid input for priority.");
                    }
                    priorityScheduling(pid, bt, at, pri, n);
                    break;
                case 6:
                    cout << "Exiting...\n";
                    break;
                default:
                    cout << "Invalid choice. Try again.\n";
            }
        } while (choice != 6);

    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
        cin.clear(); 
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    return 0;
}
