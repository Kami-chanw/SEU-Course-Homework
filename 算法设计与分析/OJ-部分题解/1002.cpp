#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>

std::string line;
int         n, num_cnt;

std::vector<int> split_to_int(std::string s) {
    std::vector<int> ret;

    char* tmp_s = new char[s.length() + 1];
    strcpy(tmp_s, s.c_str());
    char* tmp = strtok(tmp_s, " ");
    while (tmp) {
        ret.push_back(std::atoi(tmp));
        tmp = strtok(NULL, " ");
    }

    return ret;
}

int main() {
    std::cin >> n;
    std::getline(std::cin, line);
    for (int i = 0; i < n; ++i) {
        num_cnt = 0;
        std::getline(std::cin, line);
        auto nums = split_to_int(line);
        std::sort(nums.begin(), nums.end(), [](int a, int b) { return a < b; });
        std::cout << nums[1] << std::endl;
    }
}