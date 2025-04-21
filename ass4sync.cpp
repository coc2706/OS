#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <vector>

using namespace std;

int sharedData = 0;          // Shared resource
int readCount = 0;
pthread_mutex_t mutexReadCount;
pthread_mutex_t mutexWrite;

void* reader(void* arg) {
    int id = *((int*)arg);

    // Entry section
    pthread_mutex_lock(&mutexReadCount);
    readCount++;
    if (readCount == 1) {
        pthread_mutex_lock(&mutexWrite); // First reader locks the writer
    }
    pthread_mutex_unlock(&mutexReadCount);

    // Reading
    cout << "[Reader " << id << "] Reading data: " << sharedData << endl;
    usleep(100000);

    // Exit section
    pthread_mutex_lock(&mutexReadCount);
    readCount--;
    if (readCount == 0) {
        pthread_mutex_unlock(&mutexWrite); // Last reader unlocks the writer
    }
    pthread_mutex_unlock(&mutexReadCount);

    return nullptr;
}

void* writer(void* arg) {
    int id = *((int*)arg);

    pthread_mutex_lock(&mutexWrite);

    // Writing
    sharedData += 10;
    cout << ">>> [Writer " << id << "] Writing data: " << sharedData << endl;
    usleep(150000);

    pthread_mutex_unlock(&mutexWrite);

    return nullptr;
}

int main() {
    int numReaders, numWriters;

    cout << "Enter number of readers: ";
    cin >> numReaders;
    cout << "Enter number of writers: ";
    cin >> numWriters;

    pthread_mutex_init(&mutexReadCount, NULL);
    pthread_mutex_init(&mutexWrite, NULL);

    vector<pthread_t> readerThreads(numReaders);
    vector<pthread_t> writerThreads(numWriters);
    vector<int> readerIds(numReaders);
    vector<int> writerIds(numWriters);

    for (int i = 0; i < numWriters; ++i) {
        writerIds[i] = i + 1;
        pthread_create(&writerThreads[i], NULL, writer, &writerIds[i]);
        usleep(50000);
    }

    for (int i = 0; i < numReaders; ++i) {
        readerIds[i] = i + 1;
        pthread_create(&readerThreads[i], NULL, reader, &readerIds[i]);
        usleep(50000);
    }

    for (int i = 0; i < numWriters; ++i) {
        pthread_join(writerThreads[i], NULL);
    }

    for (int i = 0; i < numReaders; ++i) {
        pthread_join(readerThreads[i], NULL);
    }

    pthread_mutex_destroy(&mutexReadCount);
    pthread_mutex_destroy(&mutexWrite);

    cout << "\nFinal value of shared data: " << sharedData << endl;
    return 0;
}
