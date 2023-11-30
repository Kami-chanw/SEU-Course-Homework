#include <iostream>
#include <string>

int         m;
std::string str1, str2;

int lcs(std::string s1, std::string s2) {
    int lcm[510][510];
    for (int i = 0; i < 510; ++i)
        for (int j = 0; j < 510; ++j)
            lcm[i][j] = 0;

    for (int s_1 = 1; s_1 <= s1.length(); ++s_1) {
        for (int s_2 = 1; s_2 <= s2.length(); ++s_2) {
            lcm[s_1][s_2] = std::max(lcm[s_1 - 1][s_2], lcm[s_1][s_2 - 1]);
            if (s1[s_1 - 1] == s2[s_2 - 1]) {
                lcm[s_1][s_2] = std::max(lcm[s_1][s_2], lcm[s_1 - 1][s_2 - 1] + 1);
            }
        }
    }

    return lcm[s1.length()][s2.length()];
}

int main() {
    std::cin >> m;
    for (int i = 0; i < m; ++i) {
        std::cin >> str1 >> str2;
        std::cout << lcs(str1, str2) << std::endl;
    }
}