#include "adjacency_list.h"
#include <algorithm>
#include <ranges>
#include <queue>
using namespace std;
namespace rng = ranges;

template <class Tp, class Tag>
std::vector<typename adjacency_list<Tp, Tag>::vertex_type> adjacency_list<Tp, Tag>::bfs(const_reference vertex_a,
    const_reference vertex_b) {
    if (index_map.count(vertex_a) + index_map.count(vertex_b) < 2)
        return {};

    std::queue<pair<vertex_type, int>> q;
    std::vector<bool>                  visited(vertices.size(), false);
    std::vector<vertex_type>           parent(vertices.size());
    q.emplace(vertex_a, 0);
    visited[index_map[vertex_a]] = true;

    while (!q.empty()) {
        auto [vertex, path_weight] = q.front();
        if (vertex == vertex_b)
            break;
        q.pop();
        for (const auto& v : vertices[index_map[vertex]]) {
            auto i = index_map[v.vertex];
            if (!visited[i]) {
                q.emplace(v.vertex, 1 + path_weight);
                parent[i] = vertex;
                visited[i] = true;
            }
        }
    }

    vector<vertex_type> path;
    vertex_type         vertex = vertex_b;
    path.push_back(vertex_b);
    while (vertex != vertex_a)
        path.push_back(vertex = parent[index_map[vertex]]);

    return rng::to<vector<vertex_type>>(views::reverse(path));
}

int main() {
    adjacency_list<int> g;
    g.add_edge(0, 1);
    g.add_edge(0, 2);
    g.add_edge(1, 3);
    g.add_edge(1, 4);
    g.add_edge(2, 5);
    g.add_edge(2, 6);
    g.add_edge(7, 3);
    g.add_edge(7, 4);
    g.add_edge(7, 5);
    g.add_edge(7, 6);
    vector<int> path = g.bfs(0, 7);
    rng::copy(path, ostream_iterator<int>(cout, " --> "));
}