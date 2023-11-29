#include <iostream>
#include <stack>
#include <utility>

int m, n, ans;
int num[20010];

std::stack<std::pair<int, int>> stk;

int main() {
    std::cin >> m;
    for (int i = 0; i < m; ++i) {
        std::cin >> n;
        ans = 0;
        for (int j = 0; j < 2 * n; ++j) {
            std::cin >> num[j];
        }
        for (int j = 0; j < 2 * n; ++j) {
            std::pair<int, int> cur = std::make_pair(j, num[j]);
            if (!stk.empty() && stk.top().second != cur.second) {
                ans += (cur.first - stk.top().first);
                stk.pop();
            }
            else {
                stk.push(cur);
            }
        }
        std::cout << ans << std::endl;
    }
}