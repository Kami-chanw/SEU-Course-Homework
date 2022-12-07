#include "adjacency_list.h"
using namespace std;

int main() {
    adjacency_list<int, Directed> g;
    g.add_edge(0, 1, 5);
    g.add_edge(0, 2, 3);
    g.add_edge(1, 3, 6);
    g.add_edge(1, 2, 2);
    g.add_edge(2, 4, 4);
    g.add_edge(2, 5, 2);
    g.add_edge(2, 3, 7);
    g.print();

    for (auto i : g)
        cout << i << " ";
}