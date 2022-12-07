#include "adjacency_list.h"
#include <algorithm>
#include <map>
#include <queue>
#include <ranges>
using namespace std;
namespace rng = ranges;

class disjoint_set {
private:
    vector<int> parent;

public:
    disjoint_set(int size = 10) : parent(size) {
        for (int i = 0; i < size; ++i)
            parent[i] = i;
    }

    void merge(int i, int j) { parent[find(i)] = find(j); }
    int  find(int i) {
        if (i == parent[i])
            return i;
        return parent[i] = find(parent[i]);
    }
};

template <class Tp, class Tag>
typename adjacency_list<Tp, Tag>::weight_type adjacency_list<Tp, Tag>::Kruskal(adjacency_list& tree) {
    priority_queue<edge_tuple, deque<edge_tuple>, greater<>> edges;

    for (auto& header : vertices)
        for (const auto& e : header.edges)
            edges.emplace(header.vertex.value, e.vertex.value, e.weight);

    disjoint_set set(vertices.size());
    weight_type  weight{};
    while (!edges.empty()) {
        edge_tuple min_edge = edges.top();
        edges.pop();
        int from_idx = index_map[min_edge.from], to_idx = index_map[min_edge.to];
        if (set.find(from_idx) != set.find(to_idx)) {
            set.merge(from_idx, to_idx);
            weight += min_edge.weight;
            tree.add_edge(min_edge.from, min_edge.to);
        }
    }
    return weight;
}

int main() {
    adjacency_list<int> g, min_tree;
    g.add_edge(0, 5, 10);
    g.add_edge(0, 1, 28);
    g.add_edge(6, 1, 14);
    g.add_edge(5, 4, 25);
    g.add_edge(6, 4, 24);
    g.add_edge(3, 6, 18);
    g.add_edge(3, 4, 22);
    g.add_edge(2, 3, 12);
    g.add_edge(1, 2, 16);

    g.Kruskal(min_tree);
    min_tree.print();
}