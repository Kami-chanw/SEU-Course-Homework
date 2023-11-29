#include <algorithm>
#include <iostream>
#include <vector>

int T, n, E, ans;
int fa[5510];
struct edge_t {
    edge_t(int u, int v, int w) : from(u), to(v), weight(w) {}
    int from, to, weight;
};
std::vector<edge_t> edge;

void init() {
    ans = 0;
    for (int i = 0; i < 5510; ++i)
        fa[i] = i;
    edge.clear();
}

int find(int i) {
    if (fa[i] == i) {
        return i;
    }
    return fa[i] = find(fa[i]);
}
void combine(int i, int j) {
    fa[find(i)] = find(j);
}

void solve() {
    for (auto it = edge.begin(); it != edge.end(); ++it) {
        if (find(it->from) != find(it->to)) {
            combine(it->from, it->to);
            ans += it->weight;
        }
    }
    for (int i = 1; i < n; ++i) {
        if (find(i) != find(i + 1)) {
            std::cout << -1 << std::endl;
            return;
        }
    }
    std::cout << ans << std::endl;
}

int main() {
    std::cin >> T;
    // T = 1;
    for (int i = 0; i < T; ++i) {
        init();
        std::cin >> n >> E;
        for (int j = 0; j < E; ++j) {
            int u, v, w;
            std::cin >> u >> v >> w;
            edge.push_back(edge_t(u, v, w));
        }
        std::sort(edge.begin(), edge.end(),
                  [](edge_t a, edge_t b) { return a.weight < b.weight; });
        solve();
    }
}