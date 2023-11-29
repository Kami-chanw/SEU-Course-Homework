#include <iostream>

int m, n;
int row[510], col[510];
int cost[510][510];

void solve() {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cost[i][j] = 0x7fffffff;
        }
    }

    for (int i = 0; i < n - 1; ++i) {
        cost[i][i + 1] = row[i] * col[i] * col[i + 1];
        cost[i][i]     = 0;
    }
    cost[n - 1][n - 1] = 0;

    for (int stride = 2; stride < n; ++stride) {
        for (int l = 0; l < n - stride; ++l) {
            int r = l + stride;
            for (int t = l; t < r; ++t) {
                cost[l][r] = std::min(cost[l][r], cost[l][t] + cost[t + 1][r] + row[l] * col[t] * col[r]);
            }
        }
    }

    std::cout << cost[0][n - 1] << std::endl;
}

int main() {
    std::cin >> m;
    for (int i = 0; i < m; ++i) {
        std::cin >> n;
        for (int j = 0; j < n; ++j) {
            std::cin >> row[j] >> col[j];
        }
        solve();
    }
}