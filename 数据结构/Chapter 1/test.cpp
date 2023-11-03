#include <Windows.h>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
using namespace std;

double  PCFreq       = 0.0;
__int64 CounterStart = 0;

void StartCounter() {
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li))
        cout << "QueryPerformanceFrequency failed!\n";

    PCFreq = double(li.QuadPart) / 1000000.0;

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}
double GetCounter() {
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart - CounterStart) / PCFreq;
}
int binarySearch(int* a, const int x, const int n) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int middle = (left + right) / 2;
        if (x < a[middle])
            right = middle - 1;
        else if (x > a[middle])
            left = middle + 1;
        else
            return middle;
    }
    return -1;
}
void TimeSearch() {
    int a[101] = { 0 }, n[101] = { 0 };

    for (int i = 0; i < 101; ++i)
        a[i] = n[i] = i;
    fstream fs("data.csv", ios::trunc | ios::out);
    if (!fs)
        return;
    fs << "n,total(ms),runTime(ms)" << endl;
    for (int i = 0; i < 100; ++i) {

        double start, stop;
        start = GetCounter();
        for (long b = 1; b <= 5e7; ++b)
            int k = binarySearch(a, n[i], n[i]);
        stop = GetCounter();

        auto totalTime = stop - start;
        auto runTime   = totalTime / 5e7;
        fs << n[i] << "," << totalTime << "," << runTime << endl;
    }
}
int main() {
    StartCounter();
    TimeSearch();
}