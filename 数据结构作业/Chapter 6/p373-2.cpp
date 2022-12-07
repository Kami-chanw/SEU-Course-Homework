#include "adjacency_list.h"
#include <algorithm>
#include <ranges>
using namespace std;
namespace rng = ranges;

template <class Tp, class Tag>
std::vector<pair<typename adjacency_list<Tp, Tag>::value_type, typename adjacency_list<Tp, Tag>::weight_type>>
adjacency_list<Tp, Tag>::shortest_path(const_reference src) {
	static_assert(is_same_v<Tag, Directed>, "shortest_paht: topo logic sort requires directed graph");
	vector<weight_wrapper> dist(vertices.size(), weight_wrapper::INF);
	dist[index_map[src]] = weight_wrapper{ 0 };
	stack<vertex_type>     stk = topo_logical_sort();
	while (!stk.empty()) {
		size_type idx = index_map[stk.top().value];
		stk.pop();
		if (dist[idx] != weight_wrapper::INF) {
			for (const auto& e : vertices[idx].edges) {
				size_type curr_idx = index_map[e.vertex.value];
				if (dist[curr_idx] > dist[idx] + e.weight) {
					dist[curr_idx] = dist[idx] + e.weight;
				}
			}
		}
	}
	std::vector<pair<value_type, weight_type>> res;
	for (auto i : std::views::iota(decltype(vertices)::size_type(0), vertices.size()))
		res.emplace_back(vertices[i].vertex.value, dist[i]);

	return res;
}

int main() {
	adjacency_list<int, Directed> g;
	g.add_edge(0, 1, 5);
	g.add_edge(0, 2, 3);
	g.add_edge(1, 3, 6);
	g.add_edge(1, 2, 2);
	g.add_edge(2, 4, 4);
	g.add_edge(2, 5, 2);
	g.add_edge(2, 3, 7);
	g.add_edge(3, 4, -1);
	g.add_edge(4, 5, -2);
	g.print();
	for (auto [v, w] : g.shortest_path(1))
		cout << format("vertex: {}  weight: {}", v, w) << endl;
}