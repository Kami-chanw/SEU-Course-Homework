#include <iostream>

int    T, n;
double ans;

int main() {
    std::cin >> T;
    while (T--) {
        std::cin >> n;
        ans = 0;
        for (int i = 0; i < n - 1; ++i) {
            int x;
            std::cin >> x;
            ans += x;
        }
        printf("%.6lf\n", 1.00 + 0.01 * ans);
    }
}