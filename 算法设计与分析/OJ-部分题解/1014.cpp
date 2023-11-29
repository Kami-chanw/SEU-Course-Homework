#include <iostream>

int m, n, ans, cur;
int num[50100];

void process() {
    cur = ans = num[0];

    for (int i = 1; i < n; ++i) {
        if (cur < 0) {
            cur = 0;
        }
        cur += num[i];
        ans = std::max(ans, cur);
    }
}

int main() {
    std::cin >> m;
    for (int i = 0; i < m; ++i) {
        std::cin >> n;
        for (int j = 0; j < n; ++j) {
            std::cin >> num[j];
        }
        process();
        std::cout << ans << std::endl;
    }
}