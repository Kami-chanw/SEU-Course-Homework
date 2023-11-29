#include <iostream>

int n, m, c, ans;
int s[510], v[510];
int dp[510][110];

void process() {
    for (int i = 0; i < 510; ++i) {
        for (int j = 0; j < 110; ++j) {
            dp[i][j] = 0;
        }
    }
    ans = 0;

    for (int item = 1; item <= n; ++item) {
        for (int space = 0; space <= c; ++space) {
            for (int k = 0; k < item; ++k) {
                if (space + s[item] <= c)
                    dp[item][space] = std::max(dp[k][space], dp[k][space + s[item]] + v[item]);
                else
                    dp[item][space] = dp[k][space];

                ans = std::max(ans, dp[item][space]);
            }
        }
    }
}

int main() {
    std::cin >> m;
    for (int i = 0; i < m; ++i) {
        std::cin >> n >> c;
        for (int j = 1; j <= n; ++j) {
            std::cin >> s[j] >> v[j];
        }
        process();
        std::cout << ans << std::endl;
    }
}