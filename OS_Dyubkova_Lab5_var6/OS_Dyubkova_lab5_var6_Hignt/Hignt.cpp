#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <vector>
using namespace std;

int main(int argc, char** argv) {
	constexpr __int16 ERROR_VALUE = -1;
	srand(time(0));
	HANDLE hWritePipe = (HANDLE)(atoi(argv[0]));
	HANDLE hReadPipe = (HANDLE)(atoi(argv[1]));


	if (hReadPipe == INVALID_HANDLE_VALUE || hWritePipe == INVALID_HANDLE_VALUE) {
		printf("Error: Invalid handle value\n");
		return 1;
	}

	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Event");
	if (!hEvent) {
		cout << "-Hignt-\n event was not read";
	}

	__int16 size;
	DWORD dwBytesRead;
	if (!ReadFile(hReadPipe, &size, sizeof(__int16), &dwBytesRead, NULL)) {
		cout << "-Hignt-\n info was not read";
		cout << GetLastError();
	}

	__int16* arr = new __int16[size];
	for (int i = 0; i < size; ++i) {
	    cout << (arr[i] = rand() % 100 + 1) << " ";
	}
	cout << '\n';


	vector<__int16> v;
	__int16 total = 0, count = 0;
	__int16 n;
	cout << "Enter N:\n";
	cin >> n;

	for (int i = 0; i < size; i++) {
		if (arr[i] > n) {
			v.push_back(arr[i]);
		}
	}

	__int16* res = new __int16[v.size() + 1];
	for (int i = 0; i < v.size(); ++i) {
		res[i] = v[i];
		cout << res[i] << " ";
	}
	cout << "\n";

	if (v.size() == 0) {
		res[0] = ERROR_VALUE;
	}

	DWORD dwBytesWritten;
	if (!WriteFile(hWritePipe, res, sizeof(__int16) * (v.size() == 0 ? 1 : v.size()), &dwBytesWritten, NULL)) {
		cout << "-Hignt-\n res was not written";
	}

	SetEvent(hEvent);
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(hEvent);

	system("pause");
	return 0;
}