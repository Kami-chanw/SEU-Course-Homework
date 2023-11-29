#include <algorithm>
#include <iostream>
#include <vector>

int m, n;
int f[20010];

struct activity_t {
    int s, f, v;
} activity[10010];

void solve() {
    std::sort(activity, activity + n,
              [](activity_t a, activity_t b) { return a.f < b.f; });
    int last = 0, now;
    for (int i = 0; i < n; ++i) {
        now = activity[i].f;
        for (; last < now; ++last) {
            f[last + 1] = f[last];
        }
        f[activity[i].f] = std::max(f[activity[i].f], f[activity[i].s] + activity[i].v);
    }
    std::cout << f[activity[n - 1].f] << std::endl;
}

int main() {
    std::cin >> m;
    for (int i = 0; i < m; ++i) {
        std::cin >> n;
        for (int j = 0; j < n; ++j) {
            int s, f, v;
            std::cin >> s >> f >> v;
            activity[j].s = s;
            activity[j].f = f;
            activity[j].v = v;
        }
        solve();
    }
}