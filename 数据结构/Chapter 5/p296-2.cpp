#include <fstream>
#include <functional>
#include <iostream>
#include <random>
using namespace std;
template <class Tp>
struct TreeNode {
    TreeNode() = default;
    TreeNode(const Tp& value, TreeNode* left = {}, TreeNode* right = {}) : left(left), right(right), value(value) {}

    TreeNode* left{};
    TreeNode* right{};
    Tp        value{};
};

template <class Tp>
class Tree {
    using Nodeptr = TreeNode<Tp>*;
    using Self = Tree<Tp>;

public:
    using value_type = Tp;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using pointer = Tp*;
    using const_pointer = const Tp*;
    using reference = value_type&;
    using const_reference = const value_type&;

    void insert(const_reference value) {
        Nodeptr* insert_pos = &root, curr = root;
        while (curr) {
            if (curr->value > value) {
                insert_pos = &curr->left;
                curr = curr->left;
            }
            else if (curr->value < value) {
                insert_pos = &curr->right;
                curr = curr->right;
            }
            else
                return;
        }

        Nodeptr new_node = new TreeNode<value_type>(value);
        *insert_pos = new_node;
    }

    size_t height() const {
        std::function<size_type(Nodeptr)> get_depth = [&](Nodeptr node) { return node == nullptr ? 0 : (std::max)(get_depth(node->left), get_depth(node->right)) + 1; };
        return get_depth(root);
    }

    ~Tree() noexcept { destroy(root); }

private:
    void destroy(Nodeptr node) {
        while (node) {
            destroy(node->right);
            delete std::exchange(node, node->left);
        }
    }

    Nodeptr root = nullptr;  // root->parent == nullptr
};
int main() {

    random_device rd;
    mt19937       mt{ rd() };
    constexpr int n[] = { 100, 500, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 9000, 10000 };
    fstream       fs("data.csv", ios::out | ios::trunc);
    fs << "n,height\n";
    for (int times : n) {
        Tree<int> t;
        for (int i = 0; i < times; ++i)
            t.insert(mt());
        fs << times << "," << t.height() << endl;
    }
}