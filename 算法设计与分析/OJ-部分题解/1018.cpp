#include <iostream>

int n, m, c;
int s[510], v[510];
int dp[510][110], exist[510][110];

void process() {
    for (int i = 0; i < 510; ++i) {
        for (int j = 0; j < 110; ++j) {
            dp[i][j] = exist[i][j] = 0;
        }
    }
    exist[0][c] = 1;

    for (int item = 1; item <= n; ++item) {
        for (int space = 0; space <= c; ++space) {
            for (int k = 0; k < item; ++k) {
                if (exist[k][space]) {
                    if (space + s[item] <= c && exist[k][space + s[item]]) {
                        dp[item][space]    = std::max(dp[k][space], dp[k][space + s[item]] + v[item]);
                        exist[item][space] = 1;
                    }
                    else {
                        int tmp = std::max(dp[item][space], dp[k][space]);
                        if (tmp != dp[item][space]) {
                            dp[item][space]    = tmp;
                            exist[item][space] = 1;
                        }
                    }
                }
                if (space + s[item] <= c && exist[k][space + s[item]]) {
                    int tmp = std::max(dp[item][space], dp[k][space + s[item]] + v[item]);
                    if (tmp != dp[item][space]) {
                        dp[item][space]    = tmp;
                        exist[item][space] = 1;
                    }
                }
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
        std::cout << dp[n][0] << std::endl;
    }
}