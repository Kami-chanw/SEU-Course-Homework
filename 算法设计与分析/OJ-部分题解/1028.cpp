#include <iostream>
#include <queue>
#include <utility>

int T, n, E, s, t;

int head[510], vis[510], dis[510], tot;
struct edge_t {
    int to, next, weight;
} edge[20010];

void init() {
    tot = 0;
    for (int i = 0; i < 510; ++i) {
        head[i] = 0;
        vis[i]  = 0;
        dis[i]  = 0x7fffffff;
    }
    dis[s] = 0;
}

void add_edge(int f, int t, int v) {
    edge[++tot].next = head[f];
    edge[tot].to     = t;
    edge[tot].weight = v;
    head[f]          = tot;
}

void dijsktra() {
    std::priority_queue<std::pair<int, int>> q;
    q.push(std::make_pair(0, s));
    while (!q.empty()) {
        std::pair<int, int> it = q.top();
        q.pop();
        vis[it.second] = 1;
        for (int i = head[it.second]; i; i = edge[i].next) {
            if (dis[edge[i].to] > dis[it.second] + edge[i].weight) {
                dis[edge[i].to] = dis[it.second] + edge[i].weight;
                if (!vis[edge[i].to])
                    q.push(std::make_pair(-dis[edge[i].to], edge[i].to));
            }
        }
    }
    std::cout << (dis[t] == 0x7fffffff ? -1 : dis[t]) << std::endl;
}

int main() {
    std::cin >> T;
    for (int i = 0; i < T; ++i) {
        std::cin >> n >> E >> s >> t;
        init();
        for (int j = 0; j < E; ++j) {
            int u, v, w;
            std::cin >> u >> v >> w;
            add_edge(u, v, w);
            add_edge(v, u, w);
        }
        dijsktra();
    }
}