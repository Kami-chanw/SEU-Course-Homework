#include <iostream>
#include <string>

std::string line;
int         n, num_cnt;

int main() {
    std::cin >> n;
    std::getline(std::cin, line);
    for (int i = 0; i < n; ++i) {
        num_cnt = 0;
        std::getline(std::cin, line);
        for (int j = 0; j < line.length(); ++j) {
            if ('0' <= line[j] && line[j] <= '9') {
                num_cnt++;
            }
        }
        std::cout << num_cnt << std::endl;
    }
}