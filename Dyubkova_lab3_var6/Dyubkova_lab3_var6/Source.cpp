#include <iostream>
#include <Windows.h>

using namespace std;

double* arr;
double* filtered_arr;
int filtered_arr_size;
int k;
int sum;
CRITICAL_SECTION critical_sec;
HANDLE event_main_work;
HANDLE event_work_count_element;
HANDLE event_count_element_main;

DWORD WINAPI workThread(LPVOID lpParameter);
DWORD WINAPI countElementThread(LPVOID lpParameter);

int main() {
    // 1. Инициализация событий и критической секции
    InitializeCriticalSection(&critical_sec);
   // event_main_work = CreateEvent(NULL, FALSE, FALSE, NULL);
    event_work_count_element = CreateEvent(NULL, TRUE, FALSE, NULL);
    event_count_element_main = CreateEvent(NULL, FALSE, FALSE, NULL);

    // 2. Ввод размерности и элементов массива
    int arr_size;
    cout << "Enter array size: ";
    cin >> arr_size;

    arr = new double[arr_size];
    filtered_arr = new double[arr_size];

    cout << "Enter array elements: ";
    for (int i = 0; i < arr_size; i++) {
        cin >> arr[i];
    }

    // 3. Вывод размерности и элементов массива
    cout << "Array size: " << arr_size << endl;
    cout << "Array elements: ";
    for (int i = 0; i < arr_size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    EnterCriticalSection(&critical_sec);

    // 4. Запуск потока work
    HANDLE work_thread = CreateThread(NULL, 0, workThread, NULL, 0, NULL);

    // 5. Запуск потока CountElement
    HANDLE count_element_thread = CreateThread(NULL, 0, countElementThread, NULL, 0, NULL);

    // 6. Ввод числа K
    cout << "Enter K: ";
    cin >> k;

    // 7. Сигнализация work о начале работы
   
   // SetEvent(event_main_work);
    LeaveCriticalSection(&critical_sec);

    // 8. Получение от work сигнала о выводе K элементов массива
    WaitForSingleObject(event_work_count_element, INFINITE);

    // 9. Вывод K элементов массива
    cout << "Filtered array elements: ";
    for (int i = 0; i < k; i++) {
        cout << filtered_arr[i] << " ";
    }
    cout << endl;

    // 10. Дождаться сигнала потока CountElement
    WaitForSingleObject(event_count_element_main, INFINITE);

    // 11. Вывести на экран результат работы потока CountElement
    cout << "Count of integer elements in filtered array: " << sum << endl;

    // 12. Вывести на экран оставшиеся элементы массива
    cout << "Remaining array elements: ";
    for (int i = k; i < arr_size; i++) {
        cout << filtered_arr[i] << " ";
    }
    cout << endl;

    // Ожидание завершения потоков
    WaitForSingleObject(work_thread, INFINITE);
    WaitForSingleObject(count_element_thread, INFINITE);

    // Освобождение ресурсов
    CloseHandle(work_thread);
    CloseHandle(count_element_thread);
    CloseHandle(event_main_work);
    CloseHandle(event_work_count_element);
    CloseHandle(event_count_element_main);
    DeleteCriticalSection(&critical_sec);

    return 0;
}

// Реализация функции workThread
DWORD WINAPI workThread(LPVOID lpParam) {
    int arr_size = _msize(arr) / sizeof(double);
    // 1. Дождаться сигнала от main
    EnterCriticalSection(&critical_sec);
   //WaitForSingleObject(event_main_work, INFINITE);
    // 2. Фильтрация массива
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

    // 3. Сигнализация main о готовности к выводу K элементов массива
    SetEvent(event_work_count_element);
    LeaveCriticalSection(&critical_sec);
    return 0;

}

DWORD WINAPI countElementThread(LPVOID lpParam) {
    WaitForSingleObject(event_work_count_element, INFINITE);

    // Подсчет количества целочисленных элементов
    for (int i = 0; i < k; i++) {
        if (filtered_arr[i] == floor(filtered_arr[i])) {
            sum += filtered_arr[i];
        }
    }

    SetEvent(event_count_element_main);
    return 0;
}
