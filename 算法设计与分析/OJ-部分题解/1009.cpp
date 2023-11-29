#include <algorithm>
#include <iostream>

int m, n, ans_max_number, ans_min_count;
int height[110];
int max_non_increase[110];
int max_non_decrease[110];

void calc() {
    ans_max_number = 1;
    ans_min_count  = 1;

    for (int i = 0; i < 110; ++i) {
        max_non_increase[i] = 1;
        max_non_decrease[i] = 1;
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (height[j] >= height[i]) {
                max_non_increase[i] = std::max(max_non_increase[i], max_non_increase[j] + 1);
                ans_max_number      = std::max(ans_max_number, max_non_increase[i]);
            }
            if (height[i] >= height[j]) {
                max_non_decrease[i] = std::max(max_non_decrease[i], max_non_decrease[j] + 1);
                ans_min_count       = std::max(ans_min_count, max_non_decrease[i]);
            }
        }
    }
    std::cout << ans_max_number << std::endl;
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