#include <iostream>
#include <stack>
using namespace std;
template <class Tp>
struct TreeNode {
    TreeNode() = default;
    TreeNode(const Tp& value, TreeNode* left = {}, TreeNode* right = {}, TreeNode* parent = {}) : left(left), right(right), parent(parent), value(value) {}

    TreeNode* left{};
    TreeNode* right{};
    TreeNode* parent{};
    Tp        value{};
};

template <class Key, class Value>
class Tree {
    using Nodeptr = TreeNode<pair<Key, Value>>*;
    using Self = Tree<Key, Value>;

public:
    using value_type = pair<Key, Value>;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using pointer = pair<Key, Value>*;
    using const_pointer = const pair<Key, Value>*;
    using reference = value_type&;
    using const_reference = const value_type&;

    void insert(initializer_list<value_type> l) {
        for (auto& p : l)
            insert(p.first, p.second);
    }

    void insert(const Key& key, const Value& value) {
        Nodeptr* insert_pos = &root, curr = root, par = nullptr;
        while (curr) {
            par = curr;
            if (curr->value.first > key) {
                insert_pos = &curr->left;
                curr = curr->left;
            }
            else if (curr->value.first < key) {
                insert_pos = &curr->right;
                curr = curr->right;
            }
            else
                return;
        }

        Nodeptr new_node = new TreeNode<value_type>(value_type(key, value));
        *insert_pos = new_node;
        new_node->parent = par;
    }

    void erase(const Key& key) {
        Nodeptr node = root;
        while (node) {
            if (node->value.first > key)
                node = node->left;
            else if (node->value.first < key)
                node = node->right;
            else
                break;
        }
        if (node) {
            Nodeptr parent;
            if (node->left == nullptr)
                parent = take_node(node, node->right);
            else if (node->right == nullptr)
                parent = take_node(node, node->left);
            else {
                Nodeptr suc = leftmost(node->right);
                if (suc->parent != node) {
                    parent = take_node(suc, suc->right);
                    suc->right = node->right;
                    suc->right->parent = suc;
                }
                parent = take_node(node, suc);
                suc->left = node->left;
                suc->left->parent = suc;
            }
            delete node;
        }
    }

    void visit(Nodeptr n) { cout << n->value.first << " " << n->value.second << " "; }

    void inorder() {
        stack<Nodeptr> s;
        for (Nodeptr curr = root; curr || !s.empty();) {
            while (curr) {
                s.push(curr);
                curr = curr->left;
            }
            if (!s.empty()) {
                curr = s.top();
                visit(curr);
                s.pop();
                curr = curr->right;
            }
        }
    }

    ~Tree() noexcept { destroy(root); }

private:
    Nodeptr take_node(Nodeptr node, Nodeptr suc) {
        if (is_root(node))
            root = suc;
        else if (node == node->parent->left)
            node->parent->left = suc;
        else if (node == node->parent->right)
            node->parent->right = suc;
        if (suc)
            suc->parent = node->parent;
        return node->parent;
    }

    Nodeptr leftmost(Nodeptr node) noexcept {
        while (node->left)
            node = node->left;
        return node;
    }

    bool is_root(Nodeptr n) const noexcept { return n->parent == nullptr; }

    void destroy(Nodeptr node) {
        while (node) {
            destroy(node->right);
            delete std::exchange(node, node->left);
        }
    }

    Nodeptr root = nullptr;  // root->parent == nullptr
};
int main() {
    Tree<int, char> t;
    t.insert({ { 1, 'a' }, { 2, 'b' }, { 0, 'c' }, { 6, 'd' }, { 4,'f'} });
    t.erase(2);
    t.inorder();
}