#include <iostream>
#include "windows.h"
#include <vector>
#include <conio.h>
#include <set>
#include <algorithm>
//#include <map>
#include <regex>
using namespace std;
void changeVec(vector < int >& v, int& A) {
    sort(v.rbegin(), v.rend());
    for (int i = 0; i < v.size(); i++) {
        if (v[i] <= A)v[i] = 0;
    }
}
int main(int argc, char* argv[]) {
    vector<int> vec;
    int A = std::atoi(argv[0]);
    for (int i = 1; i < argc; i++) {
        vec.push_back(std::atoi(argv[i]));
    }
    changeVec(vec, A);
    for (int i = 0; i < vec.size(); i++) {
        cout << vec[i] << ' ';
    }
    cout << '\n';
    
    cout << "Press any key to exit:";
    _getch();//задержать результат на экране дабы увидеть его
    return 0;
}
