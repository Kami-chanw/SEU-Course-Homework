#include "tree.h"
#include <iostream>
using namespace std;
int main() {
    bs_set<int> b{ 5, 3, 6, 2, 4, 7 };
    for (auto& i : b)
        cout << i << " ";
    cout << endl;
    cout << "root data is " << b.root_data() << endl;
    bs_set<int> l, r;
    l = b.left_subtree();
    r = b.right_subtree();
    for (auto& i : l)
        cout << i << " ";
    cout << endl;
    for (auto& i : r)
        cout << i << " ";
    cout << endl;
    bs_set<int> a(l, 5, r);
    for (auto& i : a)
        cout << i << " ";
}