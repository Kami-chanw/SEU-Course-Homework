#include "adjacency_list.h"
#include <algorithm>
#include <queue>
#include <ranges>
using namespace std;
namespace rng = ranges;

template <class Tp, class Tag>
typename adjacency_list<Tp, Tag>::weight_type adjacency_list<Tp, Tag>::Dijkstra(const_reference src, adjacency_list& tree) {
    using pri_pair = pair<weight_wrapper, vertex_type>;
    priority_queue<pri_pair, vector<pri_pair>, function<bool(const pri_pair&, const pri_pair&)>> q(
        [](const pri_pair& l, const pri_pair& r) { return l.first.data > r.first.data; });
    vector<bool>           visited(vertices.size(), false);
    weight_type            weight{ 0 };
    vector<weight_wrapper> dist(vertices.size(), weight_wrapper::INF);
    dist[index_map[src]] = weight_type{ 0 };
    q.emplace(weight_type{ 0 }, vertex_type(src));

    while (!q.empty()) {
        auto [distance, vertex] = q.top();
        q.pop();
        size_type idx = index_map[vertex.value];
        if (!visited[idx] && dist[idx] >= distance) {
            visited[idx] = true;
            for (const auto& e : vertices[idx].edges)
                if (distance + e.weight < dist[index_map[e.vertex.value]]) {
                    dist[index_map[e.vertex.value]] = distance + e.weight;
                    q.emplace(distance + e.weight, e.vertex);
                    tree.add_edge(vertex.value, e.vertex.value);
                    weight += e.weight;
                }
        }
    }
    return weight;
}

int main() {
    adjacency_list<int, Directed> g, min_tree;
    g.add_edge(0, 1, 5);
    g.add_edge(0, 2, 3);
    g.add_edge(1, 3, 6);
    g.add_edge(1, 2, 2);
    g.add_edge(2, 4, 4);
    g.add_edge(2, 5, 2);
    g.add_edge(2, 3, 7);
    g.print();
    g.Dijkstra(0, min_tree);
    min_tree.print();
}