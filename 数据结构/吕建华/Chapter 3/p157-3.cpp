//use c++20
#include <format>
#include <iostream>
#include <stack>
#include <string>
#include <vector>
using std::cin;
using std::cout;
using std::endl;
using std::stack;
using std::string;
using std::vector;
template <class Adapter>
typename Adapter::container_type& get_container(Adapter& a) {
    struct hack : Adapter {
        static typename Adapter::container_type& get(Adapter& a) { return a.* & hack::c; }
    };
    return hack::get(a);
}
struct offsets {
    int a, b;
};
offsets move[8] = { offsets(-1, 0), offsets(-1, 1), offsets(0, 1), offsets(1, 1), offsets(1, 0), offsets(1, -1), offsets(0, -1), offsets(-1, -1) };
enum directions { N, NE, E, SE, S, SW, W, NW };

template <>
struct std::formatter<directions> : std::formatter<std::string_view> {
    auto format(const directions& it, std::format_context& ctx) {
        string dir;
        switch (it) {
        case directions::N:
            dir = "N";
            break;
        case directions::E:
            dir = "E";
            break;
        case directions::NE:
            dir = "NE";
            break;
        case directions::NW:
            dir = "NW";
            break;
        case directions::S:
            dir = "S";
            break;
        case directions::SE:
            dir = "SE";
            break;
        case directions::SW:
            dir = "SW";
            break;
        case directions::W:
            dir = "W";
            break;
        }
        return formatter<string_view>::format(dir, ctx);
    }
};

struct Items {
    int x, y, dir;
};

stack<Items> path;

int m, p;

vector<vector<int> > maze, mark;

void Path(int x, int y) {
    if (path.empty()) {
    }
    else if (x == m && y == p) {  // success
        for (auto& it : get_container(path))
            cout << std::format("{},{},{}\n", it.x, it.y, static_cast<directions>(it.dir));
    }
    else {
        for (int i = 0; i < 8; ++i) {
            int next_x = x + move[i].a, next_y = y + move[i].b;
            if (maze[next_x][next_y] == 0 && mark[next_x][next_y] == 0) {
                mark[next_x][next_y] = 1;
                path.emplace(next_x, next_y, i);
                Path(next_x, next_y);
            }
        }
        path.pop();
        mark[x][y] = 0;
    }
}

int main() {
    static_assert(!(__cplusplus >= 201103L && __cplusplus < 202002L), "requires c++20 standard or newer");
    cout << "input m and p(row x col, not include border)" << endl;
    cin >> m >> p;
    // m = 5, p = 8; //test case

    // init maze
    maze.resize(m + 2);
    for (int i = 0; i < m + 2; i++) {
        maze[i].resize(p + 2);
        maze[i].front() = maze[i].back() = 1;
    }
    maze[0].assign(p + 2, 1);
    maze[m + 1].assign(p + 2, 1);
    mark = maze;
    /*maze[2].assign({ 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 }); //test case
    maze[4].assign({ 1, 0, 1, 1, 1, 1, 1, 1, 1, 1 });
    for (auto& i : maze) {
        for (auto& j : i)
            cout << j << " ";
        cout << endl;
    }*/
    for (int i = 0; i < m + 2; i++)
        for (int j = 0; j < p + 2; j++)
            cin >> maze[i][j];

    mark[1][1] = 1;
    path.emplace(1, 1, E);
    Path(1, 1);
    return 0;
}
