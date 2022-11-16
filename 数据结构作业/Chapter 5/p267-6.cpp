#include <iostream>
#include <stack>
using namespace std;

template <class Tp>
struct TreeNode {
    Tp        value{};
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
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

    Tree(const_reference ender) { create(ender, root); }

    void Visit(Nodeptr n) { cout << n->value << " "; }

    void Preorder() {
        stack<Nodeptr> s;
        for (Nodeptr curr = root; curr || !s.empty();) {
            while (curr) {
                Visit(curr);
                s.push(curr);
                curr = curr->left;
            }
            if (!s.empty()) {
                curr = s.top();
                s.pop();
                curr = curr->right;
            }
        }
    }

    ~Tree() noexcept { destroy(root); }

private:
    void destroy(Nodeptr node) {
        while (node) {
            destroy(node->right);
            delete std::exchange(node, node->left);
        }
    }

    void create(const_reference ender, Nodeptr& node) {
        Tp v;
        cin >> v;
        if (v == ender)
            return;
        node = reinterpret_cast<Nodeptr>(::operator new(sizeof TreeNode<Tp>));
        construct_at(addressof(node->left), Nodeptr{});
        construct_at(addressof(node->right), Nodeptr{});
        construct_at(addressof(node->value), move(v));
        create(ender, node->left);
        create(ender, node->right);
    }

    Nodeptr root = nullptr;
};
int main() {
    // input E A B z z D C z z z z
    Tree<char> t('z');
    t.Preorder();
}