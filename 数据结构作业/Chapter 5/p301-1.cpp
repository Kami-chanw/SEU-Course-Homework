#include <algorithm>
#include <iostream>
#pragma warning(disable: 6385 6001)
using namespace std;

class winner_tree {
private:
    int* tree;
    int  size;

    void update(int i) {
        for (; i; i--)
            tree[i] = (tree[i * 2] < tree[i * 2 + 1]) ? tree[i * 2] : tree[i * 2 + 1];
    }

public:
    winner_tree(const int* values, const int size) : size(size) {
        tree = new int[size * 2];
        copy_n(values, size, tree + size);
        update(size - 1);
    }
    int  top() const noexcept { return tree[1]; }
    void adjust(int index, int new_value) {
        int value_idx   = index + size;
        tree[value_idx] = new_value;
        update(value_idx / 2);
    }

    ~winner_tree() { delete[] tree; }
};

class loser_tree {
private:
    int* tree;
    int  size;
    int* winners;

    void modify(int index) {
        int left  = 0;
        int right = 0;
        if (index < size / 2) {
            left  = winners[index * 2];
            right = winners[index * 2 + 1];
        }
        else {
            left  = tree[index * 2];
            right = tree[index * 2 + 1];
        }
        int loser      = (left > right) ? left : right;
        int winner     = (loser == left) ? right : left;
        tree[index]    = loser;
        winners[index] = winner;
    }

public:
    loser_tree(const int* values, const int size) : size(size) {
        tree    = new int[size * 2];
        winners = new int[size];
        copy_n(values, size, tree + size);

        for (int i = size - 1; i; i--)
            modify(i);
        tree[0] = winners[1];
    }
    int  top() const noexcept { return tree[0]; }
    void adjust(int index, int newValue) {
        int value_idx   = index + size;
        tree[value_idx] = newValue;

        for (int i = value_idx / 2; i != 0; i = i / 2)
            modify(i);
        tree[0] = winners[1];
    }

    ~loser_tree() {
        delete[] winners;
        delete[] tree;
    }
};

int main() {
    int values[] = { 10, 9, 20, 6, 8, 11, 90, 17 };
    cout << "current:";
    copy(values, values + size(values), ostream_iterator<int>(cout, " "));
    cout << endl;

    loser_tree tree(values, 8);
    cout << "minimum :" << tree.top() << endl;

    tree.adjust(3, 15);
    cout << "After change 6 to 15 the minimum :" << tree.top() << endl;

    tree.adjust(4, 16);
    cout << "After change 8 to 16 the minimum :" << tree.top() << endl;
}