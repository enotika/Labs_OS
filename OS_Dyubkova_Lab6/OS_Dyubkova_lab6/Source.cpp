#include <iostream>
#include <thread>
#include <vector>
#include <condition_variable>
#include <mutex>

using namespace std;

double* arr;
double* filtered_arr;
int filtered_arr_size;
int k;
int sum;
mutex critical_sec;
condition_variable_any event_work_count_element;
condition_variable_any event_count_element_main;

void workThread();
void countElementThread();

int main() {
    // 1. ���� ����������� � ��������� �������
    int arr_size;
    cout << "Enter array size: ";
    cin >> arr_size;

    arr = new double[arr_size];
    filtered_arr = new double[arr_size];

    cout << "Enter array elements: ";
    for (int i = 0; i < arr_size; i++) {
        cin >> arr[i];
    }

    // 2. ����� ����������� � ��������� �������
    cout << "Array size: " << arr_size << endl;
    cout << "Array elements: ";
    for (int i = 0; i < arr_size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    // 4. ���� ����� K
    cout << "Enter K: ";
    cin >> k;

    thread work_thread(workThread);
    thread count_element_thread(countElementThread);

    // 5. ��������� �� work ������� � ������ K ��������� �������
    unique_lock<mutex> lock_work(critical_sec);
    event_work_count_element.wait(lock_work);
    lock_work.unlock();

    // 6. ����� K ��������� �������
    cout << "Filtered array elements: ";
    for (int i = 0; i < k; i++) {
        cout << filtered_arr[i] << " ";
    }
    cout << endl;

    // 7. ��������� ������� ������ countElement
    unique_lock<mutex> lock_count(critical_sec);
    event_count_element_main.wait(lock_count);

    // 8. ������� �� ����� ��������� ������ ������ countElement
    cout << "Count of integer elements in filtered array: " << sum << endl;

    // 9. ������� �� ����� ���������� �������� �������
    cout << "Remaining array elements: ";
    for (int i = k; i < arr_size; i++) {
        cout << filtered_arr[i] << " ";
    }
    cout << endl;

    // 10. �������� ���������� �������
    work_thread.join();
    count_element_thread.join();

    // 11. ������������ ��������
    delete[] arr;
    delete[] filtered_arr;

    return 0;

}

// ���������� ������� workThread
void workThread() {
    int arr_size = _msize(arr) / sizeof(double);

    // ���������� �������
    for (int i = 0; i < arr_size; i++) {
        if (arr[i] > 0) {
            filtered_arr[filtered_arr_size] = arr[i];
            filtered_arr_size++;
        }
    }
    for (int i = 0; i < arr_size; i++) {
        if (arr[i] <= 0) {
            filtered_arr[filtered_arr_size] = arr[i];
            filtered_arr_size++;
        }
    }

    event_work_count_element.notify_all();

}

void countElementThread() {
    unique_lock<mutex> lock(critical_sec);
    event_work_count_element.wait(lock);
    // ������� ���������� ������������� ���������
    for (int i = 0; i < k; i++) {
        if (filtered_arr[i] == floor(filtered_arr[i])) {
            sum++;
        }
    }

    lock.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    event_count_element_main.notify_all();

}