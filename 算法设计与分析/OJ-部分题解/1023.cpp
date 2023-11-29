#include <iostream>

int T, n, E, s, t, M, ans;
int B[510];
int edg_len[510][510];
int dp[510][110];

void init() {
    for (int i = 0; i < 510; ++i) {
        for (int j = 0; j < 110; ++j) {
            dp[i][j] = 0x7fffffff;
        }

        for (int j = 0; j < 510; ++j) {
            edg_len[i][j] = 0x7fffffff;
        }
    }
    ans = 0x7fffffff;
}

void dp_solve() {
    for (int k = 1; k <= n; ++k) {
        for (int j = 1; j <= n; ++j) {
            for (int c = 0; c <= M; ++c) {
                if (edg_len[k][j] != 0x7fffffff && dp[k][c - B[j]] != 0x7fffffff && c - B[j] >= 0)
                    dp[j][c] = std::min(dp[k][c - B[j]] + edg_len[k][j], dp[j][c]);
            }
        }
    }
    for (int k = 1; k <= n; ++k) {
        for (int j = 1; j <= n; ++j) {
            for (int c = 0; c <= M; ++c) {
                if (edg_len[k][j] != 0x7fffffff && dp[k][c - B[j]] != 0x7fffffff && c - B[j] >= 0)
                    dp[j][c] = std::min(dp[k][c - B[j]] + edg_len[k][j], dp[j][c]);
            }
        }
    }
}

int main() {
    std::cin >> T;
    for (int i = 0; i < T; ++i) {
        init();
        std::cin >> n >> E >> s >> t >> M;
        for (int j = 1; j <= n; ++j) {
            std::cin >> B[j];
            if (j == s) {
                B[j] = 0;
            }
        }
        for (int j = 1; j <= E; ++j) {
            int u, v, w;
            std::cin >> u >> v >> w;
            edg_len[u][v] = std::min(edg_len[u][v], w);
            edg_len[v][u] = std::min(edg_len[v][u], w);
        }
        for (int j = 0; j <= M; ++j) {
            dp[s][j] = 0;
        }
        dp_solve();
        for (int j = 0; j <= M; ++j) {
            ans = std::min(ans, dp[t][j]);
        }

        std::cout << (ans == 0x7fffffff ? -1 : ans) << std::endl;
    }
}