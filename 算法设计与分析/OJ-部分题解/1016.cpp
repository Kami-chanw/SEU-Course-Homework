#include <algorithm>
#include <iostream>

int m, n, x;
int num[50100];

bool process() {
    std::sort(num, num + n);
    for (int i = 0; i < n - 1; ++i) {
        if (num[i] > x / 2) {
            return false;
        }
        int l = i + 1;
        int r = n - 1;
        while (l != r) {
            int mid = (l + r) / 2;
            if (num[i] + num[mid] == x) {
                return true;
            }
            if (num[i] + num[mid] > x) {
                r = mid;
            }
            else {
                l = mid + 1;
            }
        }
        if (num[i] + num[l] == x) {
            return true;
        }
    }
    return false;
}

int main() {
    std::cin >> m;
    for (int i = 0; i < m; ++i) {
        std::cin >> n >> x;
        for (int j = 0; j < n; ++j) {
            std::cin >> num[j];
        }
        std::cout << ((process() == true) ? "yes" : "no") << std::endl;
    }
}