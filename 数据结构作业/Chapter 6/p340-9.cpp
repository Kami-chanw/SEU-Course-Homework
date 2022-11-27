#include <algorithm>
#include <chrono>
#include <fstream>
#include <random>
#include "adjacency_list.h"
using namespace std;
namespace rng = ranges;
#define COUNT_TIME(OP, SUM)                                       \
    do {                                                          \
        auto start = chrono::high_resolution_clock::now();        \
        OP;                                                       \
        auto end = chrono::high_resolution_clock::now();          \
        SUM += chrono::duration_cast<decltype(SUM)>(end - start); \
    } while (0)  // 语言顶针：鉴定为c-lang糟粕

template <class Tp>
void fn(adjacency_list<Tp>& g, const vector<pair<Tp, Tp>>& edges) {
    rng::for_each(edges, [&](const auto& e) { g.add_directed_edge(e.first, e.second); });
}

int main() {
    vector<pair<char, char>>   edges;
    constexpr int              n[] = { 100, 250, 500, 1000, 2000 };
    mt19937                    mt{ random_device{}() };
    fstream                    fs("data.csv", ios::trunc | ios::out);
    fs << "n,time" << endl;
    for (auto i : n) {
        uniform_int_distribution<>      dis(0, i);
        chrono::duration<double, micro> time{};
        while (edges.size() < i)
            edges.emplace_back(dis(mt), dis(mt));
        adjacency_list<char> g;

        COUNT_TIME(fn(g, edges), time);

        fs << format("{},{}\n", i, time);
    }
}