#include <algorithm>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>
using namespace std;
namespace fs = filesystem;

const int imove[] = { -1, 0, 1, 1, 1, 0, -1, -1 };
const int jmove[] = { 1, 1, 1, 0, -1, -1, -1, 0 };
bool      hasTraveledAll(const vector<vector<int> >& v) {
    for (int i = 0; i < v.size(); i++)
        for (int j = 0; j < v[i].size(); j++) {
            if (v[i][j] == 0)
                return false;
        }
    return true;
}

int main() {
    vector<vector<int> > count;
    int                  m, n;
    cout << "input m, n" << endl;
    cin >> m >> n;
    count.resize(m);
    for_each(count.begin(), count.end(), [&](auto& v) { v.resize(n, 0); });
    int x, y;
    cout << "input x, y" << endl;
    cin >> x >> y;
    random_device              rd;
    mt19937                    mt(rd());
    uniform_int_distribution<> ud(0, 7);

    int cnt = 0, i;
    for (i = 0; i < 5e4 && !hasTraveledAll(count); i++) {
        int index = ud(mt);
        int tempx = x + imove[index];
        int tempy = y + jmove[index];
        if (tempx >= 0 && tempx < m && tempy >= 0 && tempy < n) {
            count[x = tempx][y = tempy]++;
            cnt++;
        }
    }
    if (i == 5e4) {
        cout << "too many iterations" << endl;
        return 0;
    }
    cout << "number of motion: " << cnt << endl;

    fstream fs("data2.csv", ios::trunc | ios::out);
    if (!fs) {
        cout << "error" << endl;
        return 0;
    }

    fs << "row,col,count" << endl;
    for (int row = 0; row < count.size(); row++) {
        for (int col = 0; col < count[row].size(); col++) {
            cout << format("{:3}", count[row][col]);
            fs << format("{},{},{}\n", row, col, count[row][col]);
        }
        cout << endl;
    }

    return 0;
}