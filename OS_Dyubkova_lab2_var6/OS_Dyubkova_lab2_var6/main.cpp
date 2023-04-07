#define _CRT_SECURE_NO_WARNINGS
#include "iostream"
#include <conio.h>
#include <sstream>
#include <map>
#include "windows.h"
#include <string>
#include "stdio.h"
#include "winuser.h"
using namespace  std;
int main(int argc, char* argv[]) {
    LPCWSTR child_process_name2 = L"D:\\Temp\\OS_Dyubkova_lab2_var6\\Debug\\OS_Dyubkova_lab2_var6_child.exe";
    STARTUPINFO startup_info; 
    PROCESS_INFORMATION process_information;
    ZeroMemory(&startup_info, sizeof(STARTUPINFO));//все пол€ структуры заполн€ютс€ нул€ми
    startup_info.cb = sizeof(STARTUPINFO);//устанавливает размер структуры в батах
    startup_info.dwFlags = STARTF_USEFILLATTRIBUTE;//утсанавливает будут ли использованф некоторые члены Startupinfo когда процесс создает окно
    startup_info.dwFillAttribute = (2 << 4 | 15);//устанавливаем цвет
    int size = 0;
    int A = 0;
    cout << "Enter A: ";
    cin >> A;
    cout << "Enter size of array:  ";
    cin >> size;
    int* array = new int[size];
    cout << "Enter elements of array: ";
    for (int i = 0; i < size; i++) {
        cin >> array[i];
    }


    int j = 0;
    string temp = to_string(A) + " ";

    for (int i = 0; i < size; i++) {
        temp += to_string(array[i]);
        if (i != size - 1) {
            temp += " ";
        }
    }
    char* command_line = new char[temp.length() + 1];
    strcpy(command_line, temp.c_str());
    wchar_t* wtext = new wchar_t[strlen(command_line) + 1];
    mbstowcs(wtext, command_line, strlen(command_line) + 1);
    LPWSTR cmd = wtext;
    if (CreateProcess(child_process_name2, cmd, NULL,
        NULL, false, CREATE_NEW_CONSOLE,
        NULL, NULL, &startup_info, &process_information)) {
        WaitForSingleObject(process_information.hProcess, INFINITE);
        CloseHandle(process_information.hProcess);
        CloseHandle(process_information.hThread);
    }
    else {
        cout << "Error";
        return 0;
    }
    system("pause");
    return 0;
}