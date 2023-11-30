#include <iostream>
#include <vector>

int m, n, ans;
int num[50100];

std::vector<int> merge(std::vector<int> a, std::vector<int> b) {
    auto a_ptr = a.begin();
    auto b_ptr = b.begin();

    std::vector<int> ret;
    while (a_ptr != a.end() && b_ptr != b.end()) {
        if (*a_ptr > *b_ptr) {
            ans += (a.end() - a_ptr);
            ret.push_back(*b_ptr);
            b_ptr++;
        }
        else {
            ret.push_back(*a_ptr);
            a_ptr++;
        }
    }
    while (a_ptr != a.end()) {
        ret.push_back(*a_ptr);
        a_ptr++;
    }
    while (b_ptr != b.end()) {
        ret.push_back(*b_ptr);
        b_ptr++;
    }
    return ret;
}

std::vector<int> merge_sort(std::vector<int> v) {
    while (v.size() > 1) {
        auto lt  = v.begin();
        auto rt  = v.end();
        auto mid = lt + (rt - lt) / 2;

        std::vector<int> l_half = merge_sort(std::vector<int>(lt, mid));
        std::vector<int> r_half = merge_sort(std::vector<int>(mid, rt));
        return merge(l_half, r_half);
    }
    return v;
}

int main() {
    std::cin >> m;
    for (int i = 0; i < m; ++i) {
        std::cin >> n;
        for (int j = 0; j < n; ++j) {
            std::cin >> num[j];
        }
        merge_sort(std::vector<int>(num, num + n));
        std::cout << ans << std::endl;
        ans = 0;
    }
}