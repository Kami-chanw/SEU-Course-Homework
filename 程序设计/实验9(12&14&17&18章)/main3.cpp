#include "Array.h"
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    const int    size = 5;
    Array<int>   intArray(size);
    Array<float> floatArray(size);

    cout << "Enter 5 integer values:\n";
    cin >> intArray;
    cout << "\nThe values in intArray are:\n" << intArray << endl;

    cout << "\nEnter 5 float values:\n";
    cin >> floatArray;
    cout << "\nThe values in floatArray are:\n" << floatArray << endl;
}
