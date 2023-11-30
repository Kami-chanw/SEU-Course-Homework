#include <iterator>
#include <type_traits>
template <class Tp>
struct TreeNode {
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;
    Tp        value;
};

template <class Tp>
class tree_const_iterator {
    using Self    = tree_const_iterator<Tp>;
    using Nodeptr = TreeNode<Tp>*;

public:
    using value_type        = Tp;
    using pointer           = const Tp*;
    using reference         = const Tp&;
    using difference_type   = ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    constexpr tree_const_iterator() = default;
    constexpr tree_const_iterator(Nodeptr node) : node(node) {}

    constexpr reference          operator*() const noexcept { return node->value; }
    constexpr pointer            operator->() const noexcept { return std::addressof(operator*()); }
    [[nodiscard]] constexpr bool operator==(const Self& rhs) const noexcept(noexcept(std::declval<Nodeptr>() == std::declval<Nodeptr>())) { return rhs.node == this->node; }
    [[nodiscard]] constexpr bool operator!=(const Self& rhs) const noexcept(noexcept(*this == rhs)) { return !(*this == rhs); }

    constexpr Self& operator++() {
        if (node->right)
            while (node->left)
                node = node->left;
        else {
            while (node->parent && node == node->parent->right)
                node = node->parent;
            node = node->parent;
        }
        return *this;
    }

    constexpr Self operator++(int) {
        Self tmp = *this;
        ++*this;
        return tmp;
    }

protected:
    Nodeptr node{};
};

template <class Tp>
class tree_iterator : public tree_const_iterator<Tp> {
    using Self    = tree_iterator<Tp>;
    using Base    = tree_const_iterator<Tp>;
    using Nodeptr = typename Base::Nodeptr;

public:
    using value_type        = Tp;
    using pointer           = Tp*;
    using reference         = Tp&;
    using difference_type   = ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    constexpr tree_iterator() = default;
    constexpr tree_iterator(Nodeptr node) : Base(node) {}

    constexpr reference operator*() const noexcept { return this->node->value; }
    constexpr pointer   operator->() const noexcept { return std::addressof(operator*()); }

    constexpr Self& operator++() {
        ++static_cast<Base&>(*this);
        return *this;
    }
    constexpr Self operator++(int) {
        Self tmp = *this;
        ++*this;
        return tmp;
    }
};