#pragma once
#include <format>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <vector>
template <class Tp>
class adjacency_list {
public:
    using vertex_type = Tp;
    using weight_type = int;
    using value_type = Tp;
    using size_type = size_t;
    using reference = value_type&;
    using const_reference = const value_type&;

    void add_vectex(const_reference new_vertex) {
        index_map[new_vertex] = vertices.size();
        vertices.push_back({});
    }

    void add_edge(const_reference vertex_a, const_reference vertex_b) {
        add_directed_edge(vertex_a, vertex_b);
        add_directed_edge(vertex_b, vertex_a);
    }
    void add_directed_edge(const_reference vertex_a, const_reference vertex_b) {
        if (vertex_a == vertex_b)
            return;
        if (!index_map.count(vertex_a))
            add_vectex(vertex_a);
        if (!index_map.count(vertex_b))
            add_vectex(vertex_b);

        vertices[index_map[vertex_a]].emplace_back(vertex_b, 0);
    }

    int bfs(const_reference vertex_a, const_reference vertex_b) {
        if (index_map.count(vertex_a) + index_map.count(vertex_b) < 2)
            return -1;
        std::queue<std::pair<vertex_type, int>> q;
        std::vector<bool>                       visited(vertices.size(), false);
        q.emplace(vertex_a, 0);
        visited[index_map[vertex_a]] = true;

        while (!q.empty()) {
            auto [vertex, path_weight] = q.front();
            if (vertex == vertex_b)
                return path_weight;
            q.pop();
            for (const auto& v : vertices[index_map[vertex]]) {
                auto i = index_map[v.vertex];
                if (!visited[i]) {
                    q.emplace(v.vertex, 1 + path_weight);
                    visited[i] = true;
                }
            }
        }
        return -1;
    }

    int bfs(const_reference vertex_a, const_reference vertex_b, std::vector<value_type>& path) {
        if (index_map.count(vertex_a) + index_map.count(vertex_b) < 2)
            return -1;

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

        vertex_type vertex = vertex_b;
        path.push_back(vertex_b);
        while (vertex != vertex_a)
            path.push_back(vertex = parent[index_map[vertex]]);

        std::reverse(path.begin(), path.end());

        return q.front().second;
    }

    int dfs(const_reference vertex_a, const_reference vertex_b) {
        if (index_map.count(vertex_a) + index_map.count(vertex_b) < 2)
            return -1;
        std::vector<bool> visited(vertices.size(), false);
        visited[index_map[vertex_a]] = true;

#ifdef __cpp_explicit_this_parameter
        auto dfs_visit = [&](this auto self,
#else
        std::function<weight_type(const vertex_type&, weight_type)> dfs_visit = [&](
#endif
            const vertex_type& vtx, weight_type w) {
                size_type idx = index_map[vtx];
                visited[idx] = true;
                for (const auto& v : vertices[idx]) {
                    if (v.vertex == vertex_b)
                        return w;
                    weight_type res;
#ifdef __cpp_explicit_this_parameter
                    res = self(v.vertex, w + 1);
#else
                    res = dfs_visit(v.vertex, w + 1);
#endif
                    if (res > 0)
                        return res;
                }
                return -1;
        };
        return dfs_visit(vertex_a, 0);
    }

    int dfs(const_reference vertex_a, const_reference vertex_b, std::vector<value_type>& path) {
        if (index_map.count(vertex_a) + index_map.count(vertex_b) < 2)
            return -1;
        std::vector<bool>        visited(vertices.size(), false);
        std::vector<vertex_type> parent(vertices.size());
        visited[index_map[vertex_a]] = true;

#ifdef __cpp_explicit_this_parameter
        auto dfs_visit = [&](this auto self,
#else
        std::function<weight_type(const vertex_type&, weight_type)> dfs_visit =
            [&](
#endif
                const vertex_type& vtx, weight_type w) -> weight_type {
                    size_type idx = index_map[vtx];
                    visited[idx] = true;
                    for (const auto& v : vertices[idx]) {
                        size_type i = index_map[v.vertex];
                        if (vtx == vertex_b)
                            return w;
                        if (!visited[i]) {
                            parent[i] = vtx;
                            weight_type res;
#ifdef __cpp_explicit_this_parameter
                            res = self(v.vertex, w + 1);
#else
                            res = dfs_visit(v.vertex, w + 1);
#endif
                            if (res > 0)
                                return res;
                        }
                    }

                    return -1;
        };

        weight_type w = dfs_visit(vertex_a, 0);
        vertex_type vertex = vertex_b;
        path.push_back(vertex_b);
        while (vertex != vertex_a)
            path.push_back(vertex = parent[index_map[vertex]]);

        std::reverse(path.begin(), path.end());
        return w;
    }

    void print() const {
        for (auto& p : index_map) {
            std::cout << "vertex " << p.first;
            for (auto& es : vertices[p.second]) {
                std::cout << std::format(" --> {}", es.vertex);
            }
            std::cout << std::endl;
        }
    }

private:
    struct edge {
        edge(const vertex_type& _vertex, const int weight) : vertex(_vertex), weight(weight) {}
        vertex_type vertex;
        int         weight;
    };

    std::vector<std::vector<edge>> vertices;

    std::map<vertex_type, int> index_map;
};