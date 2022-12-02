#include "adjacency_list.h"
#include <algorithm>
#include <ranges>
using namespace std;
namespace rng = ranges;

template <class Tp, class Tag>
std::vector<typename adjacency_list<Tp, Tag>::vertex_type> adjacency_list<Tp, Tag>::dfs(const_reference vertex_a,
    const_reference vertex_b) {
    if (index_map.count(vertex_a) + index_map.count(vertex_b) < 2)
        return {};
    std::vector<bool>        visited(vertices.size(), false);
    std::vector<vertex_type> parent(vertices.size());
    visited[index_map[vertex_a]] = true;

#ifdef __cpp_explicit_this_parameter
    auto dfs_visit = [&](this auto self,
#else
    std::function<void(const vertex_type&)> dfs_visit = [&](
#endif
        const vertex_type& vtx) {
            size_type idx = index_map[vtx];
            visited[idx] = true;
            for (const auto& v : vertices[idx]) {
                size_type i = index_map[v.vertex];
                if (vtx == vertex_b)
                    return;
                if (!visited[i]) {
                    parent[i] = vtx;
#ifdef __cpp_explicit_this_parameter
                    self(v.vertex);
#else
                    dfs_visit(v.vertex);
#endif
                }
            }
    };

    dfs_visit(vertex_a);
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
    vector<int> path = g.dfs(0, 7);
    rng::copy(path, ostream_iterator<int>(cout, " --> "));
}