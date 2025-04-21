#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <vector>

using namespace std;

int sharedData = 0;  // Shared resource (no sync)

void* reader(void* arg) {
    int id = *((int*)arg);
    usleep(50000); // Delay before reading
    cout << "[Reader " << id << "] Reading data: " << sharedData << endl;
    return nullptr;
}

void* writer(void* arg) {
    int id = *((int*)arg);
    int localCopy = sharedData;
    localCopy += 10;
    usleep(100000); // Simulate writing delay
    sharedData = localCopy;
    cout << ">>> [Writer " << id << "] Writing data: " << sharedData << endl;
    return nullptr;
}

int main() {
    int numReaders, numWriters;

    cout << "Enter number of readers: ";
    cin >> numReaders;
    cout << "Enter number of writers: ";
    cin >> numWriters;

    vector<pthread_t> readerThreads(numReaders);
    vector<pthread_t> writerThreads(numWriters);
    vector<int> readerIds(numReaders);
    vector<int> writerIds(numWriters);

    for (int i = 0; i < numWriters; ++i) {
        writerIds[i] = i + 1;
        pthread_create(&writerThreads[i], NULL, writer, &writerIds[i]);
        usleep(30000); // slight stagger
    }

    for (int i = 0; i < numReaders; ++i) {
        readerIds[i] = i + 1;
        pthread_create(&readerThreads[i], NULL, reader, &readerIds[i]);
        usleep(30000); // slight stagger
    }

    for (int i = 0; i < numWriters; ++i) {
        pthread_join(writerThreads[i], NULL);
    }

    for (int i = 0; i < numReaders; ++i) {
        pthread_join(readerThreads[i], NULL);
    }

    cout << "\nFinal value of shared data: " << sharedData << endl;
    return 0;
}
