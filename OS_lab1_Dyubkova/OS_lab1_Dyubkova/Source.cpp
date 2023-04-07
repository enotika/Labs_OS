#include <iostream>
#include <random>
#include <windows.h>
#include "thread"
#include "algorithm"
using namespace std; 

struct Data {
private:
    int timeToSleep;
    int* array;
    int X;
    void CopyArr(int* array) {
        int array_size = _msize(array) / sizeof(int);
        this->array = new int[array_size];
        for (int i = 0; i < array_size; i++) {
            this->array[i] = array[i];
        }
    }
public:
    Data(int sleep_time, int* array, int _X) {
        this->timeToSleep = sleep_time;
        X = _X;
        CopyArr(array);
    }

    int GetSleepTime() {
        return this->timeToSleep;
    }

    int* GetArray() {
        return this->array;
    }

    int GetX() {
        return this->X;
    }

    void SetArray(int* array) {
        CopyArr(array);
    }

    void SetTime(int time) {
        this->timeToSleep = time;
    }

    void SetX(int _X) {
        this->X = _X;
    }

};

int* CreateRandomArr(int size) {
    int* array = new int[size];
    for (int i = 0; i < size; i++) {
        array[i] = rand() % 10;
    }
    return array;
}


DWORD WINAPI CountX(LPVOID  structure) {
    struct Data* p = (struct Data*)structure;
    int array_size = _msize(p->GetArray()) / sizeof(int);
    int result = 0;
    int time = p->GetSleepTime();
    int X = p->GetX();
    cout << "sleep\n";
    Sleep(time);

    for (int i = 0; i < array_size; i++) {
        int tmp = p->GetArray()[i];
        if (tmp == X) {
            result++;
        }
    }
    cout << result;

    return 0;
}

int main() {
    srand(time(NULL));
    cout << "Enter size of array: ";
    int size = 0;
    cin >> size;

    cout << "Enter new element X: ";
    int X = 0;
    cin >> X;

    cout << "Enter delay time in milliseconds:";
    int time = 0;
    cin >> time;
    int* array = CreateRandomArr(size);
    cout << "Array: ";
    for (int i = 0; i < size; i++) {
        cout << array[i] << " ";
    }
    cout << "\n";

    Data* myData   = new Data(time, array, X);
    DWORD workerid;
    HANDLE worker = CreateThread(NULL, 0, &CountX, myData, 0, &workerid);
    if (worker == NULL) {
        return GetLastError();
    }

    SuspendThread(worker);
    ResumeThread(worker);
    WaitForSingleObject(worker, INFINITE);
    CloseHandle(worker);
    delete[] array;
}