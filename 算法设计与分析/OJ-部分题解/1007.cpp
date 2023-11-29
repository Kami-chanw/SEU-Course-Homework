#include <cstring>
#include <iostream>
#include <string>

int         m, n, k;
std::string str;
long long   sum[7][41];

long long calc() {
    // init
    long long ans = 1;
    for (int i = 0; i < 7; ++i)
        for (int j = 0; j < 41; ++j)
            sum[i][j] = 1;

    for (int j = 1; j < n; ++j) {
        sum[0][j] = std::stoi(str.substr(0, j));
    }

    for (int i = 1; i <= k; ++i) {
        for (int j = i; j < n; ++j) {
            for (int p = i - 1; p < j; ++p) {  // Perform DP
                sum[i][j] = std::max(sum[i][j], sum[i - 1][p] * std::stoi(str.substr(p, j - p)));
            }
        }
    }

    for (int j = k; j < n; ++j) {  // Getting answer
        ans = std::max(ans, sum[k][j] * std::stoi(str.substr(j, n - j)));
    }
    return ans;
}
int main() {
    std::cin >> m;
    for (int i = 0; i < m; ++i) {
        std::cin >> n >> k;
        std::cin >> str;
        std::cout << calc() << std::endl;
    }
}