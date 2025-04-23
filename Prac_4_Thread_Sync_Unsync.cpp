#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>
#include <stdexcept>  // For exception handling

using namespace std;

// Constants
#define READER_COUNT 5
#define WRITER_COUNT 5
#define READ_TIMES 3
#define WRITE_TIMES 1

// Shared data
int available_balance = 2000;
int active_readers = 0;

// Synchronization tools
pthread_mutex_t lock_access;
sem_t writer_control;

// -------------------- Synchronized Reader --------------------
void* sync_reader(void* arg) {
    try {
        int id = *static_cast<int*>(arg);
        delete static_cast<int*>(arg);

        for (int i = 0; i < READ_TIMES; i++) {

            pthread_mutex_lock(&lock_access);
            active_readers++;
            if (active_readers == 1) {
                sem_wait(&writer_control);  // First reader blocks writers
            }
            pthread_mutex_unlock(&lock_access);

            // Reading
            cout << "[Reader ID = " << id << "] Current Balance: $" << available_balance << endl;
            usleep(400000);  // Simulate reading

            // Reader exit section
            pthread_mutex_lock(&lock_access);
            active_readers--;
            if (active_readers == 0) {
                sem_post(&writer_control);  // Last reader unblocks writers
            }
            pthread_mutex_unlock(&lock_access);

            usleep(500000);
        }
    } catch (const exception& e) {
        cerr << "[Reader Thread Error] " << e.what() << endl;
    }

    return nullptr;
}

// -------------------- Synchronized Writer --------------------
void* sync_writer(void* arg) {
    try {
        int id = *static_cast<int*>(arg);
        delete static_cast<int*>(arg);

        for (int i = 0; i < WRITE_TIMES; i++) {
            sem_wait(&writer_control);  // Block readers and writers

            int amount = id * 25;
            available_balance += amount;
            cout << "[Writer ID = " << id << "] UPDATED BALANCE = $" << available_balance << " (+$" << amount << ")" << endl;

            usleep(900000);  // Simulate writing
            sem_post(&writer_control);  // Release access

            usleep(500000);
        }
    } catch (const exception& e) {
        cerr << "[Writer Thread Error] " << e.what() << endl;
    }

    return nullptr;
}

// -------------------- Unsynchronized Reader --------------------
void* unsync_reader(void* arg) {
    try {
        int id = *static_cast<int*>(arg);
        delete static_cast<int*>(arg);

        for (int i = 0; i < READ_TIMES; i++) {
            cout << "[Reader ID = " << id << "] BALANCE = $" << available_balance << endl;
            usleep(700000);  // Simulate reading
        }
    } catch (const exception& e) {
        cerr << "[Unsync Reader Error] " << e.what() << endl;
    }

    return nullptr;
}

// -------------------- Unsynchronized Writer --------------------
void* unsync_writer(void* arg) {
    try {
        int id = *static_cast<int*>(arg);
        delete static_cast<int*>(arg);

        for (int i = 0; i < WRITE_TIMES; i++) {
            // Force overlap
            usleep(100000);
            int amount = id * 100;
            available_balance += amount;
            usleep(200000);
            cout << "[Writer ID = " << id << "] UPDATED BALANCE = $" << available_balance << " (+$" << amount << ")" << endl;

            usleep(1000000);
        }
    } catch (const exception& e) {
        cerr << "[Unsync Writer Error] " << e.what() << endl;
    }

    return nullptr;
}

// -------------------- Synchronized Execution --------------------
void run_sync() {
    pthread_t readers[READER_COUNT], writers[WRITER_COUNT];

    pthread_mutex_init(&lock_access, nullptr);
    sem_init(&writer_control, 0, 1);

    // Create reader threads
    for (int i = 0; i < READER_COUNT; i++) {
        int* reader_id = new int(i + 1);
        pthread_create(&readers[i], nullptr, sync_reader, reader_id);
    }

    // Create writer threads
    for (int i = 0; i < WRITER_COUNT; i++) {
        int* writer_id = new int(i + 1);
        pthread_create(&writers[i], nullptr, sync_writer, writer_id);
    }

    // Wait for all threads
    for (int i = 0; i < READER_COUNT; i++) {
        pthread_join(readers[i], nullptr);
    }
    for (int i = 0; i < WRITER_COUNT; i++) {
        pthread_join(writers[i], nullptr);
    }

    pthread_mutex_destroy(&lock_access);
    sem_destroy(&writer_control);
}

// -------------------- Unsynchronized Execution --------------------
void run_unsync() {
    pthread_t readers[READER_COUNT], writers[WRITER_COUNT];

    // Create reader threads
    for (int i = 0; i < READER_COUNT; i++) {
        int* reader_id = new int(i + 1);
        pthread_create(&readers[i], nullptr, unsync_reader, reader_id);
    }

    // Create writer threads
    for (int i = 0; i < WRITER_COUNT; i++) {
        int* writer_id = new int(i + 1);
        pthread_create(&writers[i], nullptr, unsync_writer, writer_id);
    }

    // Wait for all threads
    for (int i = 0; i < READER_COUNT; i++) {
        pthread_join(readers[i], nullptr);
    }
    for (int i = 0; i < WRITER_COUNT; i++) {
        pthread_join(writers[i], nullptr);
    }
}

// -------------------- Main Function --------------------
int main() {
    try {
        int choice;
        cout << "Choose Mode: 1 Synchronized  2 Unsynchronized\nPlease enter the choice (1/2): ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "\nRunning Synchronized Execution...\n";
                run_sync();
                cout << "\nSynchronized Execution Completed!\n";
                break;
            case 2:
                cout << "\n Running Unsynchronized Execution...\n";
                run_unsync();
                cout << "\nUnsynchronized Execution Completed (May Be Inconsistent)!\n";
                break;
            default:
                cout << "\nInvalid Choice! Please restart the program.\n";
        }
    } catch (const exception& e) {
        cerr << "[Main Error] " << e.what() << endl;
    }

    return 0;
}
