#include <iostream>

int       n, m, k;
long long num[30];
long long dp[30][30][30];

long long fetch_fp(int l, int r, int t) {
    if (0 <= l && l < 30)
        if (0 <= r && r < 30)
            if (0 <= t && t < 30)
                return dp[l][r][t];
    return 0;
}

long long tri_max(long long a, long long b, long long c) {
    return std::max(std::max(a, b), c);
}

void solve() {
    for (int i = 0; i < 30; ++i)
        for (int j = 0; j < 30; ++j)
            for (int k = 0; k < 30; ++k)
                dp[i][j][k] = 0;

    for (int i = 0; i < n; ++i)
        dp[i][i][0] = num[i];

    for (int stride = 1; stride < n; ++stride) {
        for (int l = 0; l + stride < n; ++l) {
            int r = l + stride;
            for (int n_times = 0; n_times <= k; ++n_times) {
                for (int mid = l; mid < r; ++mid) {
                    for (int u = 0; u <= n_times; ++u) {
                        dp[l][r][n_times] = tri_max(
                            fetch_fp(l, r, n_times),
                            fetch_fp(l, mid, u) + fetch_fp(mid + 1, r, n_times - u),
                            fetch_fp(l, mid, u) * fetch_fp(mid + 1, r, n_times - u - 1));
                    }
                }
            }
        }
    }

    std::cout << dp[0][n - 1][k] << std::endl;
}

int main() {
    // std::cout << tri_max(1, 2, 3);
    std::cin >> m;
    for (int i = 0; i < m; ++i) {
        std::cin >> n >> k;
        for (int j = 0; j < n; ++j) {
            std::cin >> num[j];
        }
        solve();
    }
}