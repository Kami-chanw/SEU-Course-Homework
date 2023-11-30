#include <iostream>

int n, a, b;

int main() {
    std::cin >> n;
    for (int i = 1; i <= n; ++i) {
        std::cin >> a >> b;
        std::cout << a + b << std::endl;
    }
    return 0;
}