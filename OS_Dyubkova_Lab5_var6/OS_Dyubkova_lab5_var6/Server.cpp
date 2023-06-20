#include <iostream>
#include <windows.h>
using namespace std;

void main(int argc, char** argv) {
    constexpr __int16 ERROR_VALUE = -1;
    int size;
    cout << "-Server-\n Enter array size: ";
    cin >> size;

    STARTUPINFO *si;
    PROCESS_INFORMATION *pi;
    HANDLE hWritePipe, hReadPipe, hEvent;
    SECURITY_ATTRIBUTES sa;

    hEvent = CreateEvent(nullptr, FALSE, FALSE, L"Event");
    if (!hEvent) {
        cout << "-Server-\n Event was not created";
        return;
    }

    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = nullptr;
    sa.bInheritHandle = FALSE;

    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
        cout << "-Server-\n Pipe was not created";
        return;
    }

    HANDLE hDuplicateReadPipe, hDuplicateWritePipe;
    if (!DuplicateHandle(GetCurrentProcess(), hReadPipe, GetCurrentProcess(), &hDuplicateReadPipe, 0, TRUE, DUPLICATE_SAME_ACCESS)) {
        cout << "-Server-\n Duplicate handle failed";
        return;
    }
    if (!DuplicateHandle(GetCurrentProcess(), hWritePipe, GetCurrentProcess(), &hDuplicateWritePipe, 0, TRUE, DUPLICATE_SAME_ACCESS)) {
        cout << "-Server-\n Duplicate handle failed";
        return;
    }

    si = new STARTUPINFO;
    ZeroMemory(si, sizeof(STARTUPINFO));
    si->cb = sizeof(STARTUPINFO);
    pi = new PROCESS_INFORMATION;
    wchar_t* wstr = new wchar_t[80];
    wsprintf(wstr, L"%d %d", (int)hDuplicateWritePipe, (int)hDuplicateReadPipe);

    if (!CreateProcessW(L"D:/Temp/OS_Dyubkova_lab5_var6/Debug/OS_Dyubkova_lab5_var6_Hignt.exe", wstr, nullptr, nullptr, TRUE, CREATE_NEW_CONSOLE, nullptr, nullptr, si, pi)) {
        cout << "-Server-\n sum creation failed";
        cout << GetLastError();
        return;
    }

    __int16* res = new __int16[size];

    DWORD dwBytesWritten;
    if (!WriteFile(hWritePipe, &size, sizeof(__int16), &dwBytesWritten, nullptr)) {
        cout << "-Server-\n info was not written";
        return;
    }

    WaitForSingleObject(hEvent, INFINITE);

    DWORD dwBytesRead;
    if (!ReadFile(hReadPipe, res, sizeof(__int16) * (size), &dwBytesRead, nullptr)) {
        cout << "-Server-\n res was not read";
        return;
    }

    cout << "-Server-\n";
    
    cout << " Size: " << size << "\n";
    cout << "-Resulting array-\n";
    if (res[0] == ERROR_VALUE) {
        cout << "N was larger than any generated number so Hignt returned no numbers!\n";
    }
    else {
        for (int i = 0; i < dwBytesRead / sizeof(__int16); i++) {
            cout << res[i] << " ";
        } cout << "\n";
    }

    CloseHandle(hWritePipe);
    CloseHandle(hReadPipe);
    CloseHandle(hEvent);
}