#include <iostream>
#include <stdexcept> // For runtime_error exceptions
using namespace std;

int main() {
    try {
        int P, R;
        cout << "enter the number of processes: ";
        cin >> P;
        cout << "enter the number of resources: ";
        cin >> R;

        // Validate input
        if (P <= 0 || R <= 0) throw runtime_error("Invalid number of processes or resources.");

        int max[P][R];         // Max resource matrix
        int allocation[P][R];  // Allocated resource matrix
        int need[P][R];        // Need = Max - Allocation
        int available[R];      // Available resources

        // Input Max matrix
        cout << "Enter the Max matrix: " << endl;
        for (int i = 0; i < P; i++) {
            for (int j = 0; j < R; j++) {
                cin >> max[i][j];
                if (max[i][j] < 0) throw runtime_error("Max matrix contains negative value.");
            }
        }

        // Input Allocation matrix
        cout << "Enter the Allocation matrix: " << endl;
        for (int i = 0; i < P; i++) {
            for (int j = 0; j < R; j++) {
                cin >> allocation[i][j];
                if (allocation[i][j] < 0) throw runtime_error("Allocation matrix contains negative value.");
            }
        }

        // Calculate Need matrix
        for (int i = 0; i < P; i++) {
            for (int j = 0; j < R; j++) {
                need[i][j] = max[i][j] - allocation[i][j];
                if (need[i][j] < 0) throw runtime_error("Need matrix has invalid (negative) value.");
            }
        }

        // Input Available resources
        cout << "Enter the available matrix: " << endl;
        for (int i = 0; i < R; i++) {
            cin >> available[i];
            if (available[i] < 0) throw runtime_error("Available matrix contains negative value.");
        }

        bool finish[P] = {false};   // Track if process has finished
        int work[R];                // Work = copy of available resources
        for (int i = 0; i < R; i++) {
            work[i] = available[i];
        }

        int safeSeq[P], count = 0;  // Safe sequence and counter

        // Banker's Algorithm main loop
        while (count < P) {
            bool found = false;

            for (int i = 0; i < P; i++) {
                if (!finish[i]) {
                    bool canRun = true;

                    // Check if process can run with available resources
                    for (int j = 0; j < R; j++) {
                        if (need[i][j] > work[j]) {
                            canRun = false;
                            break;
                        }
                    }

                    // If yes, simulate its execution
                    if (canRun) {
                        for (int j = 0; j < R; j++)
                            work[j] += allocation[i][j]; // Release resources

                        safeSeq[count++] = i; // Add to safe sequence
                        finish[i] = true;
                        found = true;
                    }
                }
            }

            // If no process can be safely executed, system is unsafe
            if (!found) {
                throw runtime_error("System is not in a safe state.");
            }
        }

        // Print safe sequence
        cout << "Safe sequence is: ";
        for (int i = 0; i < P; i++)
            cout << "P" << safeSeq[i] << " ";
        cout << endl;
    } catch (const exception& e) {
        // Print exception message
        cout << "Exception: " << e.what() << endl;
    }

    return 0;
}
