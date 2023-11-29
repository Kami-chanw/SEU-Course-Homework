#include <iostream>

int m, n, x;
int num[20100];

void binary_search() {
    int l = 0, r = n - 1, father = 0;
    int mid;
    while (l != r) {
        mid = (l + r) / 2;
        if (num[mid] == x) {
            std::cout << "success, father is " << father << std::endl;
            return;
        }
        father = num[mid];
        if (num[mid] < x)
            l = mid + 1;
        else
            r = mid - 1;
    }
    if (num[l] == x) {
        std::cout << "success, father is " << father << std::endl;
        return;
    }
    std::cout << "not found, father is " << num[l] << std::endl;
}

int main() {
    std::cin >> m;
    for (int i = 0; i < m; ++i) {
        std::cin >> n >> x;
        for (int j = 0; j < n; ++j) {
            std::cin >> num[j];
        }
        binary_search();
    }
}