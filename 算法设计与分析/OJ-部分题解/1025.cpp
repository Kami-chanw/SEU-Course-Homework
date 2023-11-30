#include <algorithm>
#include <iostream>

int m, n, ans_min_count;
int height[11000];
int max_non_decrease[11000];

void calc() {
    ans_min_count = 1;

    for (int i = 0; i < 11000; ++i) {
        max_non_decrease[i] = 1;
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (height[i] >= height[j]) {
                max_non_decrease[i] = std::max(max_non_decrease[i], max_non_decrease[j] + 1);
                ans_min_count       = std::max(ans_min_count, max_non_decrease[i]);
            }
        }
    }
    std::cout << ans_min_count << std::endl;
}

int main() {
    std::cin >> m;
    for (int i = 0; i < m; ++i) {
        std::cin >> n;
        for (int j = 0; j < n; ++j) {
            std::cin >> height[j];
        }
        calc();
    }
}