#include <iostream>

int m, n, k, li, pi;
int price[10010];

void solve() {
    for (int l = 1; l <= n; ++l) {
        for (int fst_len = 1; fst_len < l; ++fst_len) {
            price[l] = std::max(price[l], price[fst_len] + price[l - fst_len]);
        }
    }
    std::cout << price[n] << std::endl;
}

int main() {
    std::cin >> m;
    for (int i = 0; i < m; ++i) {
        std::cin >> n >> k;
        for (int j = 0; j < 10010; ++j) {
            price[j] = 0;
        }
        for (int j = 0; j < k; ++j) {
            std::cin >> li >> pi;
            price[li] = pi;
        }
        solve();
    }
}