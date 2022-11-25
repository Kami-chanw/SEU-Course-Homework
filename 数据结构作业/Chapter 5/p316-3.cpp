#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

using namespace std;
#define COUNT_TIME(OP, SUM)                                       \
    do {                                                          \
        auto start = chrono::high_resolution_clock::now();        \
        OP;                                                       \
        auto end = chrono::high_resolution_clock::now();          \
        SUM += chrono::duration_cast<decltype(SUM)>(end - start); \
    } while (0)  // 语言顶针：鉴定为c-lang糟粕

class Sets {
private:
    vector<int> parent;

public:
    Sets(int size = 10) : parent(size, -1) {}

    void SimpleUnion(int i, int j) {
        if (i == j)
            return;
        parent[i] = j;
    }
    int SimpleFind(int i) {
        while (parent[i] >= 0)
            i = parent[i];
        return i;
    }
    void WeightedUnion(int i, int j) {
        if (i == j)
            return;
        int temp = parent[i] + parent[j];
        if (parent[i] > parent[j]) {

            parent[i] = j;
            parent[j] = temp;
        }
        else {
            parent[j] = i;
            parent[j] = temp;
        }
    }
    int CollapsingFind(int i) {
        int r;
        for (r = i; parent[r] >= 0; r = parent[r])
            ;
        while (i != r) {
            int s     = parent[i];
            parent[i] = r;
            i         = s;
        }
        return r;
    }
};

enum Op { Union, Find };

int main() {
    constexpr int n[] = { 100, 250, 500, 1000, 2000, 4000 };
    fstream       fs("data.csv", ios::trunc | ios::out);
    fs << "n,aver time of Simple(ms), aver time of Weighted/Collapsing(ms)" << endl;
    vector<Op> seq;

    mt19937                 mt{ random_device{}() };
    binomial_distribution<> op_dis;

    for (int cnt : n) {
        generate_n(back_inserter(seq), cnt / 2, [&]() { return static_cast<Op>(op_dis(mt)); });
        chrono::duration<double, micro> simple_time{}, wc_time{};
        uniform_int_distribution<>      dis(1, cnt - 1);
        Sets                            a(cnt), b(cnt);
        for (Op op : seq) {
            if (op == Union) {
                while (true) {
                    int i = dis(mt), j = dis(mt);
                    if (i != a.CollapsingFind(j) && i != b.CollapsingFind(j)) {
                        COUNT_TIME(a.SimpleUnion(i, j), simple_time);
                        COUNT_TIME(b.WeightedUnion(i, j), wc_time);
                        break;
                    }
                }
            }
            else {
                int val = dis(mt);
                COUNT_TIME(b.CollapsingFind(val), wc_time);
                COUNT_TIME(a.SimpleFind(val), simple_time);
            }
        }
        fs << cnt << "," << wc_time / (cnt / 2) << "," << simple_time / (cnt / 2) << endl;
    }
}