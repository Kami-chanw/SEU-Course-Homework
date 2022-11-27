#include "adjacency_list.h"
#include <algorithm>
#include <deque>
using namespace std;
namespace rng = ranges;

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
    vector<int> path;
    g.bfs(0, 7, path);
    rng::copy(path, ostream_iterator<int>(cout, " --> "));
}