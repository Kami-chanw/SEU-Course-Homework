#include <algorithm>
#include <iostream>

int T, N;
struct task_t {
    long long d, p;
} tasks[100100];

void solve() {
    bool      occupy[100100];
    long long ans = 0;
    for (int i = 0; i < 100100; ++i) {
        occupy[i] = false;
    }
    for (int i = 0; i < N; ++i) {
        for (int j = tasks[i].d; j > 0; --j) {
            if (occupy[j] == false) {
                ans += tasks[i].p;
                occupy[j] = true;
                break;
            }
        }
    }
    std::cout << ans << std::endl;
}

int main() {
    std::cin >> T;
    while (T--) {
        std::cin >> N;
        if (N == 0)
            continue;
        for (int i = 0; i < N; ++i) {
            std::cin >> tasks[i].d >> tasks[i].p;
        }
        std::sort(tasks, tasks + N, [](const task_t& a, const task_t& b) {
            if (a.p != b.p) {
                return a.p > b.p;
            }
            return a.d > b.d;
        });
        solve();
    }
}