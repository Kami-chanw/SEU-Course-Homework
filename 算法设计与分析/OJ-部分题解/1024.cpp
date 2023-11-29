#include <iostream>
#include <stdio.h>

int    m, n;
double P_k[510], P_nk[510];
double sum[510][510];
double dp[510][510];

void solve() {
    for (int i = 0; i < 510; ++i) {
        for (int j = 0; j < 510; ++j) {
            dp[i][j] = 999999999;
        }
    }

    sum[0][0] = P_k[0] + P_nk[0] + P_nk[1];
    for (int i = 1; i < n; ++i) {
        sum[0][i] = sum[0][i - 1] + P_k[i] + P_nk[i + 1];
    }

    for (int i = 1; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            sum[i][j] = sum[i - 1][j] - sum[i - 1][i - 1] + P_nk[i];
        }
    }

    for (int i = 0; i < n; ++i) {
        dp[i][i] = P_k[i] + P_nk[i] + P_nk[i + 1];
    }

    for (int stride = 1; stride < n; ++stride) {
        for (int l = 0; l < n - stride; ++l) {
            int r = l + stride;
            for (int k = l + 1; k < r; ++k) {
                dp[l][r] = std::min(dp[l][r], dp[l][k - 1] + P_k[k] + dp[k + 1][r] + (sum[l][k - 1] + sum[k + 1][r]));
            }
            dp[l][r] = std::min(dp[l][r], dp[l][r - 1] + P_k[r] + P_nk[r + 1] + sum[l][r - 1]);
            dp[l][r] = std::min(dp[l][r], P_k[l] + P_nk[l] + sum[l + 1][r] + dp[l + 1][r]);
        }
    }

    printf("%.6lf\n", dp[0][n - 1]);
}

int main() {
    std::cin >> m;
    for (int i = 0; i < m; ++i) {
        std::cin >> n;
        int tmp;
        for (int j = 0; j < n; ++j)
            std::cin >> tmp;
        for (int j = 0; j < n; ++j)
            std::cin >> P_k[j];
        for (int j = 0; j < n + 1; ++j)
            std::cin >> P_nk[j];
        solve();
    }
}