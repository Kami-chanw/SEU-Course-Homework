#include <iostream>
using namespace std;
template <size_t N, size_t M>
int arrayCompare(int (&a)[N], int (&b)[M]) {
    for (int i = 0;; ++i) {
        if (i == N)
            return i == M ? 0 : -1;
        if (i == M)
            return 1;
        if (a[i] < b[i])
            return -1;
        if (a[i] > b[i])
            return 1;
    }
}
int main() {
    int a[] = { 1, 2, 3, 4 }, b[] = { 1, 2, 3 };
    cout << arrayCompare(a, b);
}