#pragma once
#include <compare>
#include <format>
#include <functional>
#include <iostream>
#include <map>
#include <stack>
#include <vector>

struct Directed {};
struct Undirected {};

namespace {
    template <class Adapter>
    typename Adapter::container_type& get_container(Adapter& cont) {
        struct hack : Adapter {
            static auto& get(Adapter& a) noexcept { return static_cast<hack&>(a).c; }
        };
        return hack::get(cont);
    };
}  // namespace

template <class Tp, class Tag = Undirected>
class adjacency_list {
public:
    using weight_type = int;
    using value_type = Tp;
    using size_type = size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = Tp*;
    using const_pointer = const Tp*;
    using difference_type = std::ptrdiff_t;
    using graph_type = Tag;

private:
    struct vertex_type {
        vertex_type() = default;
        vertex_type(value_type&& v) : value(std::move(v)) {}
        vertex_type(const_reference v) : value(v) {}
        bool operator==(const vertex_type& v) const noexcept { return value == v.value; }
        bool operator!=(const vertex_type& v) const noexcept { return !(*this == v); }

        value_type value;
    };

public:
    class const_iterator {
        using Self = const_iterator;
        using Adj = adjacency_list<Tp, Tag>;
        using vertex_type = typename Adj::vertex_type;

    public:
        using value_type = typename Adj::value_type;
        using pointer = typename Adj::const_pointer;
        using reference = typename Adj::const_reference;
        using difference_type = typename Adj::difference_type;
        using iterator_category = std::forward_iterator_tag;

        constexpr const_iterator() = default;
        constexpr const_iterator(const std::stack<vertex_type>& graph) : graph(graph) {}

        constexpr reference operator*() const noexcept { return graph.top().value; }
        constexpr pointer   operator->() const noexcept { return std::addressof(operator*()); }

        [[nodiscard]] constexpr bool operator==(const Self& rhs) const {
            return graph.empty() && rhs.graph.empty() || std::addressof(graph) == std::addressof(rhs.graph);
        }
        [[nodiscard]] constexpr bool operator!=(const Self& rhs) const noexcept(noexcept(*this == rhs)) {
            return !(*this == rhs);
        }

        constexpr Self& operator++() {
            if (!graph.empty())
                graph.pop();
            return *this;
        }

        constexpr Self operator++(int) {
            Self tmp = *this;
            ++* this;
            return tmp;
        }

    protected:
        std::stack<vertex_type> graph;
    };

    class iterator : public const_iterator {
        using Self = iterator;
        using Base = const_iterator;
        using Adj = adjacency_list<Tp, Tag>;
        using vertex_type = typename Adj::vertex_type;

    public:
        using value_type = typename Adj::value_type;
        using pointer = typename Adj::pointer;
        using reference = typename Adj::reference;

        constexpr iterator() = default;
        constexpr iterator(const std::stack<vertex_type>& graph) : Base(graph) {}

        constexpr reference operator*() const noexcept { return const_cast<reference>(Base::operator*()); }
        constexpr pointer   operator->() const noexcept { return std::addressof(operator*()); }

        constexpr Self& operator++() {
            ++static_cast<Base&>(*this);
            return *this;
        }
        constexpr Self operator++(int) {
            Self tmp = *this;
            ++* this;
            return tmp;
        }
    };

    [[nodiscard]] iterator       begin() noexcept { return { topo_logical_sort() }; }
    [[nodiscard]] const_iterator begin() const noexcept { return cbegin(); }
    [[nodiscard]] const_iterator cbegin() const noexcept { return { topo_logical_sort() }; }
    [[nodiscard]] iterator       end() noexcept { return {}; }
    [[nodiscard]] const_iterator end() const noexcept { return cend(); }
    [[nodiscard]] const_iterator cend() const noexcept { return {}; }

    bool add_edge(const_reference vertex_a, const_reference vertex_b, weight_type w = {}) {
        vertex_type vtx_a(vertex_a), vtx_b(vertex_b);
        bool        result = add_directed_edge(vtx_a, vtx_b, w);
        if constexpr (is_same_v<Tag, Undirected>)
            result = add_directed_edge(vtx_b, vtx_a, w);
        return result;
    }

    std::vector<value_type> bfs(const_reference vertex_a, const_reference vertex_b);  // p352-5
    std::vector<value_type> dfs(const_reference vertex_a, const_reference vertex_b);  // p352-6

    weight_type Kruskal(adjacency_list& tree);  // p359-1

    std::vector<std::pair<value_type, weight_type>> shortest_path(const_reference src);  // p373-2

    weight_type Dijkstra(const_reference src, adjacency_list& tree);  // p373-5

    adjacency_list topo_sort() {
        std::stack<vertex_type> stk;
        adjacency_list          topo_graph;
        topo_logical_sort(stk);
        if (vertex_type curr; !stk.empty()) {
            curr = std::move(stk.top());
            stk.pop();
            for (const auto& ele : std::views::reverse(get_container(stk))) {
                topo_graph.add_directed_edge<false>(curr, ele);
                curr = ele;
            }
        }
        return topo_graph;
    }

    bool add_vectex(const_reference new_vertex) {
        auto [_, success] = index_map.try_emplace(new_vertex, vertices.size());
        if (success)
            vertices.emplace_back(std::move(vertex_type(new_vertex)), std::vector<edge>{});
        return success;
    }

    void print() const {
        for (auto& p : index_map) {
            std::cout << "vertex " << p.first;
            for (auto& es : vertices[p.second].edges) {
                std::cout << std::format(" --> {}", es.vertex.value);
            }
            std::cout << std::endl;
        }
    }

private:
    std::stack<vertex_type> topo_logical_sort() const {
        std::vector<bool>       visit(vertices.size(), false);
        std::stack<vertex_type> res;
#ifdef __cpp_explicit_this_parameter
        auto impl = [&](this auto self,
#else
        std::function<void(const vertex_type&)> impl = [&](
#endif
            const vertex_type& v) {
                size_type idx = index_map.find(v.value)->second;
                visit[idx] = true;
                for (const auto& e : vertices[idx].edges) {
                    if (!visit[index_map.find(e.vertex.value)->second]) {
#ifdef __cpp_explicit_this_parameter
                        self(e.vertex);
#else
                        impl(e.vertex);
#endif
                    }
                }
                res.push(v);
        };

        for (auto i : std::views::iota(decltype(vertices)::size_type(0), vertices.size()))
            if (!visit[i])
                impl(vertices[i].vertex);
        return res;
    }

    struct edge {
        edge(const vertex_type& _vertex, const weight_type weight) : vertex(_vertex), weight(weight) {}
        vertex_type vertex;
        weight_type weight;
    };

    struct header {
        vertex_type       vertex;
        std::vector<edge> edges;
    };

    struct edge_tuple {
        std::strong_ordering operator<=>(const edge_tuple& e2) const noexcept { return weight <=> e2.weight; }

        value_type  from, to;
        weight_type weight;
    };

    struct weight_wrapper {
        inline static constexpr weight_type INF = std::numeric_limits<weight_type>::max();

        weight_wrapper(const weight_type& w) : data(w) {}

        weight_wrapper() = default;
        weight_wrapper(const weight_wrapper&) = default;
        weight_wrapper(weight_wrapper&&) = default;

        bool operator==(const weight_type& w) const noexcept { return data == w; }
        // std::strong_ordering operator<=>(const weight_type& w) const noexcept { return data <=> w; }

        weight_wrapper& operator=(const weight_wrapper&) = default;
        weight_wrapper& operator=(weight_wrapper&&) = default;
        weight_wrapper& operator=(const weight_type& w) {
            data = w;
            return *this;
        }

        weight_type data;

        operator weight_type() noexcept { return data; }
    };

    template <bool Check = true>
    bool add_directed_edge(const vertex_type& vertex_a, const vertex_type& vertex_b, weight_type w = {}) {
        if (vertex_a == vertex_b)
            return false;
        add_vectex(vertex_a.value);
        add_vectex(vertex_b.value);
        auto& edges = vertices[index_map[vertex_a.value]].edges;

        if constexpr (Check)
            if (std::find_if(edges.begin(), edges.end(), [&](const edge& e) { return e.vertex == vertex_b; }) != edges.end())
                return false;

        edges.emplace_back(vertex_b, w);
        return true;
    }

    std::vector<header> vertices;

    std::map<value_type, size_type> index_map;
};