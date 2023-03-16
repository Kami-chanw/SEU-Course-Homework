#pragma once

#include "iter.h"
#include <algorithm>
#include <atomic>
#include <stdexcept>
#undef KFN
#define KFN(NODE) Traits::kfn((NODE)->value)

namespace {

    template <class Tp>
    struct TreeNode {
        using Node    = TreeNode<Tp>;
        using Nodeptr = Node*;

        int prop{};  // for balanced tree

        bool    is_nil;
        Nodeptr left;
        Nodeptr right;
        Nodeptr parent;
        Tp      value;

        inline bool is_real_root() const noexcept { return this == parent->parent; }
        inline bool is_left() const noexcept { return this == parent->left; }
        inline bool is_right() const noexcept { return this == parent->right; }

        template <class Alnode>
        inline static Nodeptr create_root(Alnode& alloc) {
            Nodeptr root = TreeTempNode<Alnode>(alloc).release();
            init_node(root, root, root, root, {}, true);
            return root;
        }

        inline static void init_node(Nodeptr node, Nodeptr left, Nodeptr right, Nodeptr parent, const int& attr, bool is_nil) {
            node->left   = left;
            node->right  = right;
            node->parent = parent;
            node->prop   = attr;
            node->is_nil = is_nil;
        }

        template <class Alnode>
        void static destroy_node(Alnode& alloc, Nodeptr node) noexcept {
            static_assert(std::is_same_v<typename Alnode::value_type, Node>, "Allocator's value_type is not consist with node");
            std::allocator_traits<Alnode>::destroy(alloc, std::addressof(node->value));
            std::allocator_traits<Alnode>::deallocate(alloc, node, 1);
        }

        inline static Nodeptr leftmost(Nodeptr node) noexcept {
            while (!node->left->is_nil)
                node = node->left;
            return node;
        }

        inline static Nodeptr rightmost(Nodeptr node) noexcept {
            while (!node->right->is_nil)
                node = node->right;
            return node;
        }

        inline static void find_inorder_predecessor(Nodeptr& node) noexcept {
            if (!node->left->is_nil)
                node = rightmost(node->left);
            else {
                Nodeptr parent{};
                while (!(parent = node->parent)->is_nil && node == parent->left)
                    node = parent;
                if (!node->is_nil)
                    node = parent;
            }
        }

        inline static void find_inorder_successor(Nodeptr& node) noexcept {
            if (!node->right->is_nil)
                node = leftmost(node->right);
            else {
                while (!node->parent->is_nil && node == node->parent->right)
                    node = node->parent;
                node = node->parent;
            }
        }
    };

    template <class Alnode>
    struct TreeTempNode {  // for exception safety
        using AlnodeTraits = std::allocator_traits<Alnode>;
        using Nodeptr      = typename AlnodeTraits::pointer;
        using Node         = typename std::remove_pointer_t<Nodeptr>;

        TreeTempNode(Alnode& alloc) : alnode(alloc), node(alloc.allocate(1)) {}

        template <class... Args>
        explicit TreeTempNode(Alnode& alloc, Nodeptr root, Args&&... values) : TreeTempNode(alloc) {
            AlnodeTraits::construct(alnode, std::addressof(node->value), std::forward<Args>(values)...);
            Node::init_node(node, root, root, root, {}, false);
        }

        TreeTempNode(const TreeTempNode&) = delete;
        TreeTempNode& operator=(const TreeTempNode&) = delete;

        [[nodiscard]] Nodeptr release() noexcept { return std::exchange(node, nullptr); }

        ~TreeTempNode() {
            if (node)
                Node::destroy_node(alnode, node);
        }

        Nodeptr node = nullptr;
        Alnode& alnode;
    };

    /**
     *	@class ScaryTree
     *   @brief for scary iterator
     */
    template <class ValTypes>
    struct ScaryTree {
        using Self            = ScaryTree<ValTypes>;
        using Nodeptr         = typename ValTypes::Nodeptr;
        using Node            = typename ValTypes::Node;
        using value_type      = typename ValTypes::value_type;
        using size_type       = typename ValTypes::size_type;
        using difference_type = typename ValTypes::difference_type;
        using pointer         = typename ValTypes::pointer;
        using const_pointer   = typename ValTypes::const_pointer;
        using reference       = value_type&;
        using const_reference = const value_type&;

        // clang-format off
#ifdef _PREORDER_ITERATOR_
            static void incr(Nodeptr& node) noexcept {
                if (!node->left->is_nil)
                    node = node->left;
                else if (!node->right->is_nil)
                    node = node->right;
                else {
                    Nodeptr suc = node->parent;
                    while (node != suc->parent) {
                        if (node == suc->left && !suc->right->is_nil) {
                            node = suc->right;
                            return;
                        }
                        node = suc, suc = suc->parent;
                    }
                    node = suc;
                }
            }


            static void Decr(Nodeptr& node) noexcept {
                Nodeptr pre = node->parent;
                if (node == pre->right && !pre->left->is_nil && !node->parent->is_nil) {
                    pre = pre->left;
                    while (!pre->right->is_nil || !pre->left->is_nil) {
                        for (; !pre->right->is_nil; pre = pre->right);
                        if (!pre->left->is_nil)
                            pre = pre->left;
                    }
                }
                node = pre;
            }
#elif defined _POSTORDER_ITERATOR_
            static void incr(Nodeptr& node) noexcept {
                Nodeptr suc = node->parent;
                if (node == suc->left && !suc->right->is_nil && !node->parent->is_nil) {
                    suc = suc->right;
                    while (!suc->left->is_nil || !suc->right->is_nil) {
                        for (; !suc->left->is_nil; suc = suc->left);
                        if (!suc->right->is_nil)
                            suc = suc->right;
                    }
                }
                node = suc;
            }

            static void Decr(Nodeptr& node) noexcept {
                if (node->is_nil)
                    node = node->parent;
                else if (!node->right->is_nil)
                    node = node->right;
                else if (!node->left->is_nil)
                    node = node->left;
                else {
                    for (Nodeptr pre = node->parent;; pre = pre->parent) {
                        if (node == pre->right && !pre->left->is_nil) {
                            node = pre->left;
                            return;
                        }
                        node = pre;
                    }
                }
            }
#elif defined _LEVEL_ORDER_ITERATOR_
        // clang-format on
        static Nodeptr FwdExplore(Nodeptr node, size_type height) noexcept {
            if (height == 0 || node->is_nil)
                return node;
            Nodeptr res = FwdExplore(node->left, height - 1);
            if (!res->is_nil)
                return res;
            return FwdExplore(node->right, height - 1);
        }

        static Nodeptr Incr(Nodeptr node, size_type height = 0) noexcept {
            if (node->is_real_root())
                return FwdExplore(node, height + 1);
            if (node->is_left()) {
                Nodeptr res = FwdExplore(node->parent->right, height);
                if (!res->is_nil)
                    return res;
            }
            return Incr(node->parent, height + 1);
        }

        static void incr(Nodeptr& node) noexcept { node = Incr(node); }

        static Nodeptr BackwdExplore(Nodeptr node, int height) noexcept {
            if (height == 0 || node->is_nil)
                return node;
            Nodeptr res = BackwdExplore(node->right, height - 1);
            if (!res->is_nil)
                return res;
            return BackwdExplore(node->left, height - 1);
        }

        static Nodeptr Decr(Nodeptr node, int height) noexcept {
            if (node->is_real_root())
                return BackwdExplore(node, height - 1);
            if (node->is_right()) {
                Nodeptr res = BackwdExplore(node->parent->left, height);
                if (!res->is_nil)
                    return res;
            }
            return Decr(node->parent, height + 1);
        }

        static void Decr(Nodeptr& node) noexcept { node = Decr(node, 0); }
#else  //_INORDER_ITERATOR_
       // clang-format on
        static void incr(Nodeptr& node) noexcept { Node::find_inorder_successor(node); }

        static void Decr(Nodeptr& node) noexcept {
            if (node->is_nil)
                node = node->right;
            else
                Node::find_inorder_predecessor(node);
        }
#endif

        static void decr(Nodeptr& node) noexcept { Decr(node); }

        static value_type& extract(Nodeptr node) noexcept { return node->value; }

        static bool dereferable(const Self* tree, Nodeptr node) noexcept { return node != tree->root; }

        static bool incrable(const Self*, Nodeptr node) noexcept { return !node->is_nil; }

        static bool decrable(const Self* tree, Nodeptr node) {
            return true;  // always true because verification should be after decreasing
        }

        void swap(Self& x) {
            if (this != std::addressof(x)) {
                using std::swap;
                swap(root, x.root);
            }
        }

        void init() noexcept { Node::init_node(root, root, root, root, {}, true); }

        Nodeptr root{};
    };
}  // namespace

template <class Traits, template <class, class> class... MixIn>
class BinaryTree : public MixIn<Traits, BinaryTree<Traits, MixIn...>>... {
    using Self         = BinaryTree<Traits, MixIn...>;
    using ScaryVal     = typename Traits::ScaryVal;
    using Node         = typename Traits::ScaryVal::Node;
    using _Altp_traits = typename Traits::_Altp_traits;
    using _Alnode_type = typename _Altp_traits::template rebind_alloc<Node>;
    using AlnodeTraits = std::allocator_traits<_Alnode_type>;

public:
    friend struct TreeAccessor;
    using Nodeptr         = typename Traits::Nodeptr;
    using allocator_type  = typename Traits::allocator_type;
    using value_type      = typename Traits::value_type;
    using key_type        = typename Traits::key_type;
    using key_compare     = typename Traits::key_compare;
    using value_compare   = typename Traits::value_compare;
    using size_type       = typename Traits::size_type;
    using difference_type = typename Traits::difference_type;
    using pointer         = typename Traits::pointer;
    using const_pointer   = typename Traits::const_pointer;
    using reference       = value_type&;
    using const_reference = const value_type&;

    using const_iterator = typename Traits::const_iterator;
    using iterator       = typename Traits::iterator;
#if !defined(_LEVEL_ORDER_ITERATOR_) && !defined(_PREORDER_ITERATOR_) && !defined(_POSTORDER_ITERATOR_)
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
#endif

    static constexpr bool Multi = Traits::Multi;

public:
    [[nodiscard]] iterator       begin() noexcept { return MakeIter(cbegin().base()); }
    [[nodiscard]] const_iterator begin() const noexcept { return cbegin(); }
    [[nodiscard]] const_iterator cbegin() const noexcept {
#if defined(_PREORDER_ITERATOR_) || defined(_LEVEL_ORDER_ITERATOR_)
        return MakeCIter(Getroot()->parent);
#elif defined _POSTORDER_ITERATOR_
        if (beg.load()->is_nil)
            SetBegin();
        return MakeCIter(beg);
#else
        return MakeCIter(Getroot()->left);
#endif
    }
    [[nodiscard]] iterator       end() noexcept { return MakeIter(Getroot()); }
    [[nodiscard]] const_iterator end() const noexcept { return cend(); }
    [[nodiscard]] const_iterator cend() const noexcept { return MakeCIter(const_cast<Nodeptr>(Getroot())); }

#if !defined(_LEVEL_ORDER_ITERATOR_) && !defined(_PREORDER_ITERATOR_) && !defined(_POSTORDER_ITERATOR_)
    [[nodiscard]] reverse_iterator       rbegin() noexcept { return reverse_iterator(MakeIter(crbegin().base().base())); }
    [[nodiscard]] const_reverse_iterator rbegin() const noexcept { return crbegin(); }

    [[nodiscard]] const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    [[nodiscard]] reverse_iterator       rend() noexcept { return reverse_iterator(MakeIter(crend().base().base())); }
    [[nodiscard]] const_reverse_iterator rend() const noexcept { return crend(); }

    [[nodiscard]] const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }
#endif

    BinaryTree() {
        InitRoot();
#if defined _POSTORDER_ITERATOR_ || defined _PREORDER_ITERATOR_
        beg = Getroot();
#endif
    }

    BinaryTree(const BinaryTree& other) : sz(other.sz) {
        InitRoot();
        Getroot()->parent = Copy(other.Getroot()->parent, Getroot());
        Getroot()->left   = Node::leftmost(Getroot()->parent);
        Getroot()->right  = Node::rightmost(Getroot()->parent);
#if defined _POSTORDER_ITERATOR_ || defined _PREORDER_ITERATOR_
        beg = Getroot();
#endif
        alloc_pocca(alloc, AlnodeTraits::select_on_container_copy_construction(other.alloc));
    }

    BinaryTree(BinaryTree&& other) noexcept(std::is_nothrow_move_constructible_v<key_compare>) : sz(std::exchange(other.sz, size_type{ 0 })) {
        InitRoot();
        val.swap(other.val);
#if defined _POSTORDER_ITERATOR_ || defined _PREORDER_ITERATOR_
        beg = Getroot();
#endif
        alloc_pocma(alloc, other.alloc);
    }

    BinaryTree(BinaryTree& bt1, const_reference item, BinaryTree& bt2) : sz(1 + std::exchange(bt1.sz, size_type{ 0 }) + std::exchange(bt2.sz, size_type{ 0 })) {
        if (bt1.root_data() >= item || bt2.root_data() <= item) {
            sz = 0;
            return;
        }
        InitRoot();
        Nodeptr root = TreeTempNode<_Alnode_type>(alloc).release();

        root->is_nil                  = false;
        root->value                   = item;
        root->left                    = bt1.Getroot()->parent;
        bt1.Getroot()->parent->parent = root;
        root->right                   = bt2.Getroot()->parent;
        bt2.Getroot()->parent->parent = root;
        root->parent                  = Getroot();

        Getroot()->parent = root;
        Getroot()->left   = bt1.Getroot()->left;
        Getroot()->right  = bt2.Getroot()->right;

        bt1.val.init();
        bt2.val.init();
#if defined _POSTORDER_ITERATOR_ || defined _PREORDER_ITERATOR_
        beg = Getroot();
#endif
    }

    template <std::input_iterator Iter>
    BinaryTree(Iter first, Iter last) {
        InitRoot();
#if defined _POSTORDER_ITERATOR_ || defined _PREORDER_ITERATOR_
        beg = Getroot();
#endif
        insert(first, last);
    }

    BinaryTree(std::initializer_list<value_type> l) : BinaryTree(l.begin(), l.end()) {}

    template <bool _IsMulti = Multi, std::enable_if_t<_IsMulti, int> = 0>
    iterator insert(const_reference value) {
        return Emplace(value).first;
    }
    template <bool _IsMulti = Multi, std::enable_if_t<_IsMulti, int> = 0>
    iterator insert(value_type&& value) {
        return Emplace(std::move(value)).first;
    }

    template <bool _IsMulti = Multi, std::enable_if_t<!_IsMulti, int> = 0>
    std::pair<iterator, bool> insert(const_reference value) {
        return Emplace(value);
    }
    template <bool _IsMulti = Multi, std::enable_if_t<!_IsMulti, int> = 0>
    std::pair<iterator, bool> insert(value_type&& value) {
        return Emplace(std::move(value));
    }

    void insert(std::initializer_list<value_type> l) { insert(l.begin(), l.end()); }
    template <std::input_iterator Iter>
    void insert(Iter first, Iter last) {
        for (; first != last; ++first)
            Emplace(*first);
    }

    [[nodiscard]] iterator       lower_bound(const key_type& key);
    [[nodiscard]] const_iterator lower_bound(const key_type& key) const;
    template <class Key, class Cmpr = key_compare, class = typename Cmpr::is_transparent>
    [[nodiscard]] iterator lower_bound(const Key& key);
    template <class Key, class Cmpr = key_compare, class = typename Cmpr::is_transparent>
    [[nodiscard]] const_iterator lower_bound(const Key& key) const;

    [[nodiscard]] iterator       upper_bound(const key_type& key);
    [[nodiscard]] const_iterator upper_bound(const key_type& key) const;
    template <class Key, class Cmpr = key_compare, class = typename Cmpr::is_transparent>
    [[nodiscard]] iterator upper_bound(const Key& key);
    template <class Key, class Cmpr = key_compare, class = typename Cmpr::is_transparent>
    [[nodiscard]] const_iterator upper_bound(const Key& key) const;

    [[nodiscard]] std::pair<iterator, iterator>             equal_range(const key_type& key);
    [[nodiscard]] std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const;
    template <class Key, class Cmpr = key_compare, class = typename Cmpr::is_transparent>
    [[nodiscard]] std::pair<iterator, iterator> equal_range(const Key& key);
    template <class Key, class Cmpr = key_compare, class = typename Cmpr::is_transparent>
    [[nodiscard]] std::pair<const_iterator, const_iterator> equal_range(const Key& key) const;

    iterator  erase(const_iterator position) noexcept;
    size_type erase(const key_type& value) noexcept(noexcept(declval<Self>().EqualRange(value)));
    iterator  erase(const_iterator first, const_iterator last) noexcept;

    [[nodiscard]] size_type size() const noexcept { return sz; }

    [[nodiscard]] bool empty() const noexcept;

    Self left_subtree() const {
        Self ret;
        ret.Getroot()->parent = ret.Copy(Getroot()->parent->left, ret.Getroot());
        ret.Getroot()->left   = Node::leftmost(ret.Getroot()->parent);
        ret.Getroot()->right  = Node::rightmost(ret.Getroot()->parent);
        ret.sz                = ret.GetSize(ret.Getroot()->parent);
        return ret;
    }
    Self right_subtree() const {
        Self ret;
        ret.Getroot()->parent = ret.Copy(Getroot()->parent->right, ret.Getroot());
        ret.Getroot()->left   = Node::leftmost(ret.Getroot()->parent);
        ret.Getroot()->right  = Node::rightmost(ret.Getroot()->parent);
        ret.sz                = ret.GetSize(ret.Getroot()->parent);
        return ret;
    }
    value_type root_data() const noexcept { return Getroot()->parent->value; }

    void swap(BinaryTree& tree) noexcept(std::is_nothrow_swappable_v<key_compare>);

    [[nodiscard]] key_compare   key_comp() const { return key_compare(); }
    [[nodiscard]] value_compare value_comp() const { return value_compare(); }

    void clear() noexcept;

    ~BinaryTree() {
        clear();
        Node::destroy_node(alloc, Getroot());
    }

    BinaryTree& operator=(const BinaryTree&);
    BinaryTree& operator=(BinaryTree&&) noexcept(AlnodeTraits::is_always_equal::value&& std::is_nothrow_move_assignable_v<key_compare>);

private:
    enum class Inspos { LEFT, RIGHT };

    struct Inspack {
        Nodeptr parent;
        Inspos  pos;
    };

    struct FindResult {
        Inspack pack;
        Nodeptr curr;
    };

    void Destroy(Nodeptr node) noexcept {
        while (!node->is_nil) {
            Destroy(node->right);
            Node::destroy_node(alloc, std::exchange(node, node->left));
        }
    }
    void      InitRoot() { val.root = Node::create_root(alloc); }
    size_type GetSize(Nodeptr node) {
        if (!node->is_nil) {
            return GetSize(node->left) + GetSize(node->right) + 1;
        }
        return 0;
    }
    template <class Key>
    FindResult LowerBound(const Key& value) const;
    template <class Key>
    FindResult UpperBound(const Key& value) const;
    template <class Key>
    std::pair<Nodeptr, Nodeptr> EqualRange(const Key& value) const noexcept(is_nothrow_comparable_v<key_compare, value_type, Key>&& is_nothrow_comparable_v<key_compare, Key, value_type>);

    inline Nodeptr InsertAt(const Inspack&, Nodeptr);
    template <class... Args>
    std::pair<iterator, bool> Emplace(Args&&...);

    Nodeptr Copy(Nodeptr, Nodeptr);
    Nodeptr Erase(Nodeptr);

    void                  SetBegin() const;
    inline iterator       MakeIter(Nodeptr node) noexcept { return iterator(node); }
    inline const_iterator MakeIter(Nodeptr node) const noexcept { return const_iterator(node); }
    inline const_iterator MakeCIter(Nodeptr node) noexcept { return const_iterator(node); }
    inline const_iterator MakeCIter(Nodeptr node) const noexcept { return const_iterator(node); }

    inline Nodeptr       Getroot() noexcept { return val.root; }
    inline const Nodeptr Getroot() const noexcept { return val.root; }

    key_compare  cmpr;
    _Alnode_type alloc;
    ScaryVal     val;
    size_type    sz = 0;

#if defined _POSTORDER_ITERATOR_ || defined _PREORDER_ITERATOR_
    mutable std::atomic<Nodeptr> beg;
#endif
};

template <class Traits, template <class, class> class... MixIn>
typename BinaryTree<Traits, MixIn...>::Nodeptr BinaryTree<Traits, MixIn...>::Copy(Nodeptr src, Nodeptr dst) {
    Nodeptr subroot = Getroot();
    if (!src->is_nil) {
        Nodeptr node = TreeTempNode<_Alnode_type>(alloc, subroot, src->value).release();
        node->parent = dst;
        node->prop   = src->prop;
        subroot      = node;
        try {
            node->left  = Copy(src->left, node);
            node->right = Copy(src->right, node);
        } catch (...) {
            Destroy(node);
        }
    }
    return subroot;
}

template <class Traits, template <class, class> class... MixIn>
typename BinaryTree<Traits, MixIn...>::Nodeptr BinaryTree<Traits, MixIn...>::Erase(Nodeptr node) {
    if (Getroot()->left == node)
        Getroot()->left = node->right->is_nil ? node->parent : Node::leftmost(node->right);
    if (Getroot()->right == node)
        Getroot()->right = node->left->is_nil ? node->parent : Node::rightmost(node->left);
    Nodeptr parent = Traits::extract_node(this, node);
    Node::destroy_node(alloc, node);
    if (Getroot()->parent->is_nil)
        val.init();
    return parent;
}

template <class Traits, template <class, class> class... MixIn>
typename BinaryTree<Traits, MixIn...>::Nodeptr BinaryTree<Traits, MixIn...>::InsertAt(const Inspack& pack, Nodeptr new_node) {
    new_node->parent = pack.parent;
    if (pack.parent == Getroot())
        Getroot()->parent = Getroot()->left = Getroot()->right = new_node;
    else {
        if (pack.pos == Inspos::LEFT) {
            pack.parent->left = new_node;
            if (pack.parent == Getroot()->left)
                Getroot()->left = new_node;
        }
        else {
            pack.parent->right = new_node;
            if (pack.parent == Getroot()->right)
                Getroot()->right = new_node;
        }
#if defined _POSTORDER_ITERATOR_ || defined _PREORDER_ITERATOR_
        if (beg == pack.parent)
            beg = new_node;
#endif
    }
    Traits::insert_fixup(this, new_node);
    ++sz;
    return new_node;
}

template <class Traits, template <class, class> class... MixIn>
template <class... Args>
std::pair<typename BinaryTree<Traits, MixIn...>::iterator, bool> BinaryTree<Traits, MixIn...>::Emplace(Args&&... values) {
    using InPlaceKeyExtractor = typename Traits::template InPlaceKeyExtractor<std::remove_cvref_t<Args>...>;
    Nodeptr    new_node;
    FindResult res;
    if constexpr (InPlaceKeyExtractor::extractable && !Multi) {
        const auto& key = InPlaceKeyExtractor::extract(values...);
        res             = LowerBound(key);
        if (!res.curr->is_nil && !cmpr(key, KFN(res.curr)))  // key has existed in the tree
            return { MakeIter(res.curr), false };

        new_node = TreeTempNode<_Alnode_type>(alloc, Getroot(), std::forward<Args>(values)...).release();
    }
    else {
        TreeTempNode<_Alnode_type> tmp_node(alloc, Getroot(), std::forward<Args>(values)...);
        const key_type&            key = Traits::kfn(tmp_node.node->value);
        if constexpr (Multi)
            res = UpperBound(key);
        else {
            res = LowerBound(key);
            if (!res.curr->is_nil && !cmpr(key, KFN(res.curr)))  // key has existed in the tree
                return { MakeIter(res.curr), false };
        }
        new_node = tmp_node.release();
    }
    return { MakeIter(InsertAt(res.pack, new_node)), true };
}

template <class Traits, template <class, class> class... MixIn>
void BinaryTree<Traits, MixIn...>::SetBegin() const {
#ifdef _PREORDER_ITERATOR_
    Nodeptr curr = Getroot()->right;
    while (!curr->left->is_nil || !curr->right->is_nil) {
        for (; !curr->right->is_nil; curr = curr->right)
            ;
        if (!curr->left->is_nil)
            curr = curr->left;
    }
    beg = curr;
#elif defined _POSTORDER_ITERATOR_
    Nodeptr curr = Getroot()->left != Getroot()->parent ? Getroot()->left : Getroot()->right;
    while (!curr->left->is_nil || !curr->right->is_nil) {
        for (; !curr->left->is_nil; curr = curr->left)
            ;
        if (!curr->right->is_nil)
            curr = curr->right;
    }
    beg = curr;
#endif
}

template <class Traits, template <class, class> class... MixIn>
typename BinaryTree<Traits, MixIn...>::iterator BinaryTree<Traits, MixIn...>::erase(const_iterator position) noexcept {
    if (position.base()->is_nil)
        return end();
    Nodeptr curr = (position++).base();
#if defined _POSTORDER_ITERATOR_ || defined _PREORDER_ITERATOR_
    if (beg == curr)
        beg = Getroot();
#endif
    Traits::erase_fixup(this, Erase(curr));
    --sz;
    return MakeIter(position.base());
}

template <class Traits, template <class, class> class... MixIn>
typename BinaryTree<Traits, MixIn...>::iterator BinaryTree<Traits, MixIn...>::erase(const_iterator first, const_iterator last) noexcept {
    if (first == begin() && last == end())
        clear();
    else {
        while (first != last)
            erase(first++);
        return MakeIter(first.base());
    }
    return begin();
}

template <class Traits, template <class, class> class... MixIn>
typename BinaryTree<Traits, MixIn...>::size_type BinaryTree<Traits, MixIn...>::erase(const key_type& value) noexcept(noexcept(declval<Self>().EqualRange(value))) {
    auto      res   = EqualRange(value);
    auto      first = MakeCIter(res.first), second = MakeCIter(res.second);
    size_type n = std::distance(first, second);
    erase(MakeCIter(first.base()), MakeCIter(second.base()));
    return n;
}

template <class Traits, template <class, class> class... MixIn>
template <class Key>
typename BinaryTree<Traits, MixIn...>::FindResult BinaryTree<Traits, MixIn...>::LowerBound(const Key& key) const {
    Nodeptr    curr = Getroot()->parent;
    FindResult res{ { Getroot()->parent }, const_cast<Nodeptr>(Getroot()) };
    while (!curr->is_nil) {
        res.pack.parent = curr;
        if (!cmpr(KFN(curr), key)) {  // curr.key >= key
            res.pack.pos = Inspos::LEFT;
            res.curr     = curr;
            curr         = curr->left;
        }
        else {
            res.pack.pos = Inspos::RIGHT;
            curr         = curr->right;
        }
    }
    return res;
}

template <class Traits, template <class, class> class... MixIn>
typename BinaryTree<Traits, MixIn...>::iterator BinaryTree<Traits, MixIn...>::lower_bound(const key_type& key) {
    return MakeIter(LowerBound(key).curr);
}

template <class Traits, template <class, class> class... MixIn>
typename BinaryTree<Traits, MixIn...>::const_iterator BinaryTree<Traits, MixIn...>::lower_bound(const key_type& key) const {
    return MakeCIter(LowerBound(key).curr);
}

template <class Traits, template <class, class> class... MixIn>
template <class Key, class, class>
typename BinaryTree<Traits, MixIn...>::iterator BinaryTree<Traits, MixIn...>::lower_bound(const Key& key) {
    return MakeIter(LowerBound(key).curr);
}

template <class Traits, template <class, class> class... MixIn>
template <class Key, class, class>
typename BinaryTree<Traits, MixIn...>::const_iterator BinaryTree<Traits, MixIn...>::lower_bound(const Key& key) const {
    return MakeCIter(LowerBound(key).curr);
}

template <class Traits, template <class, class> class... MixIn>
template <class Key>
typename BinaryTree<Traits, MixIn...>::FindResult BinaryTree<Traits, MixIn...>::UpperBound(const Key& key) const {
    Nodeptr    curr = Getroot()->parent;
    FindResult res{ { Getroot()->parent }, const_cast<Nodeptr>(Getroot()) };
    while (!curr->is_nil) {
        res.pack.parent = curr;
        if (cmpr(key, KFN(curr))) {  // curr.key > key
            res.pack.pos = Inspos::LEFT;
            res.curr     = curr;
            curr         = curr->left;
        }
        else {
            res.pack.pos = Inspos::RIGHT;
            curr         = curr->right;
        }
    }
    return res;
}

template <class Traits, template <class, class> class... MixIn>
typename BinaryTree<Traits, MixIn...>::iterator BinaryTree<Traits, MixIn...>::upper_bound(const key_type& key) {
    return MakeIter(UpperBound(key).curr);
}

template <class Traits, template <class, class> class... MixIn>
typename BinaryTree<Traits, MixIn...>::const_iterator BinaryTree<Traits, MixIn...>::upper_bound(const key_type& key) const {
    return MakeCIter(UpperBound(key).curr);
}

template <class Traits, template <class, class> class... MixIn>
template <class Key, class, class>
typename BinaryTree<Traits, MixIn...>::iterator BinaryTree<Traits, MixIn...>::upper_bound(const Key& key) {
    return MakeIter(UpperBound(key).curr);
}

template <class Traits, template <class, class> class... MixIn>
template <class Key, class, class>
typename BinaryTree<Traits, MixIn...>::const_iterator BinaryTree<Traits, MixIn...>::upper_bound(const Key& key) const {
    return MakeCIter(UpperBound(key).curr);
}

template <class Traits, template <class, class> class... MixIn>
template <class Key>
std::pair<typename BinaryTree<Traits, MixIn...>::Nodeptr, typename BinaryTree<Traits, MixIn...>::Nodeptr> BinaryTree<Traits, MixIn...>::EqualRange(const Key& key) const
    noexcept(is_nothrow_comparable_v<key_compare, value_type, Key>&& is_nothrow_comparable_v<key_compare, Key, value_type>) {
    auto first = Getroot(), second = Getroot();
    auto curr = Getroot()->parent;
    while (!curr->is_nil)
        if (cmpr(KFN(curr), key))
            curr = curr->right;
        else {
            if (second->is_nil && cmpr(key, KFN(curr)))
                second = curr;
            first = curr;
            curr  = curr->left;
        }
    curr = second->is_nil ? Getroot()->parent : second->left;
    while (!curr->is_nil)
        if (cmpr(key, KFN(curr)))
            second = curr, curr = curr->left;
        else
            curr = curr->right;
    return { const_cast<Nodeptr>(first), const_cast<Nodeptr>(second) };
}

template <class Traits, template <class, class> class... MixIn>
std::pair<typename BinaryTree<Traits, MixIn...>::iterator, typename BinaryTree<Traits, MixIn...>::iterator> BinaryTree<Traits, MixIn...>::equal_range(const key_type& key) {
    const auto res = EqualRange(key);
    return { MakeIter(res.first), MakeIter(res.second) };
}

template <class Traits, template <class, class> class... MixIn>
std::pair<typename BinaryTree<Traits, MixIn...>::const_iterator, typename BinaryTree<Traits, MixIn...>::const_iterator> BinaryTree<Traits, MixIn...>::equal_range(const key_type& key) const {
    const auto res = EqualRange(key);
    return { MakeCIter(res.first), MakeCIter(res.second) };
}

template <class Traits, template <class, class> class... MixIn>
template <class Key, class, class>
std::pair<typename BinaryTree<Traits, MixIn...>::iterator, typename BinaryTree<Traits, MixIn...>::iterator> BinaryTree<Traits, MixIn...>::equal_range(const Key& key) {
    const auto res = EqualRange(key);
    return { MakeIter(res.first), MakeIter(res.second) };
}

template <class Traits, template <class, class> class... MixIn>
template <class Key, class, class>
std::pair<typename BinaryTree<Traits, MixIn...>::const_iterator, typename BinaryTree<Traits, MixIn...>::const_iterator> BinaryTree<Traits, MixIn...>::equal_range(const Key& key) const {
    const auto res = EqualRange(key);
    return { MakeCIter(res.first), MakeCIter(res.second) };
}

template <class Traits, template <class, class> class... MixIn>
void BinaryTree<Traits, MixIn...>::swap(BinaryTree& x) noexcept(std::is_nothrow_swappable_v<key_compare>) {
    if (this != std::addressof(x)) {
        using std::swap;
        swap(sz, x.sz);
        swap(cmpr, x.cmpr);
        val.swap(x.val);
        alloc_pocs(alloc, x.alloc);
    }
}

template <class Traits, template <class, class> class... MixIn>
void BinaryTree<Traits, MixIn...>::clear() noexcept {
    Destroy(Getroot()->parent);
    val.init();
    sz = 0;
}

template <class Traits, template <class, class> class... MixIn>
bool BinaryTree<Traits, MixIn...>::empty() const noexcept {
    return Getroot()->parent->is_nil;
}

template <class Traits, template <class, class> class... MixIn>
BinaryTree<Traits, MixIn...>& BinaryTree<Traits, MixIn...>::operator=(const BinaryTree& rhs) {
    if (this != std::addressof(rhs)) {
        clear();
        if (rhs.empty())
            return *this;
        Getroot()->parent = Copy(rhs.Getroot()->parent, Getroot());
        Getroot()->left   = Node::leftmost(Getroot()->parent);
        Getroot()->right  = Node::rightmost(Getroot()->parent);
        sz                = rhs.sz;
        cmpr              = rhs.cmpr;
        alloc_pocca(alloc, rhs.alloc);
    }
    return *this;
}

template <class Traits, template <class, class> class... MixIn>
BinaryTree<Traits, MixIn...>& BinaryTree<Traits, MixIn...>::operator=(BinaryTree&& rhs) noexcept(AlnodeTraits::is_always_equal::value&& std::is_nothrow_move_assignable_v<key_compare>) {
    if (this != std::addressof(rhs)) {
        clear();
        if (rhs.empty())
            return *this;
        val.swap(rhs.val);
        std::swap(sz, rhs.sz);
        cmpr = rhs.key_comp();
        alloc_pocma(alloc, rhs.alloc);
    }
    return *this;
}

template <class Traits, class Derived>
class MapPack {
public:
    using allocator_type = typename Traits::allocator_type;
    using key_compare    = typename Traits::key_compare;
    using value_type     = typename Traits::value_type;
    using key_type       = typename Traits::key_type;
    using const_iterator = typename Traits::const_iterator;
    using iterator       = typename Traits::iterator;
    using mapped_type    = typename Traits::value_type::second_type;

private:
    using Nodeptr      = typename Traits::Nodeptr;
    using ScaryVal     = typename Traits::ScaryVal;
    using _Alnode_type = typename std::allocator_traits<typename Traits::allocator_type>::template rebind_alloc<TreeNode<value_type>>;
    using AlnodeTraits = std::allocator_traits<_Alnode_type>;

public:
    static_assert(!Traits::Multi && std::is_const_v<typename value_type::first_type>, "traits is not compatible with map");
    /**
    *	@brief  If a key equivalent to k already exists in the container, does nothing.
    Otherwise, behaves like emplace except that the element is constructed as value_type
    *	@param key : the key used both to look up and to insert if not found
    *	@param mapped_value : arguments to forward to the constructor of the element
    *	@return same as emplace
    */
    template <class... Mapped>
    std::pair<iterator, bool> try_emplace(const key_type& key, Mapped&&... mapped_value) {
        auto res = _Try_emplace(key, std::forward<Mapped>(mapped_value)...);
        return { TreeAccessor::make_iter(Derptr(), res.first), res.second };
    }
    template <class... Mapped>
    std::pair<iterator, bool> try_emplace(key_type&& key, Mapped&&... mapped_value) {
        const auto res = _Try_emplace(std::move(key), std::forward<Mapped>(mapped_value)...);
        return { TreeAccessor::make_iter(Derptr(), res.first), res.second };
    }

    template <class Mapped>
    std::pair<iterator, bool> insert_or_assign(const key_type& key, Mapped&& mapped_value) {
        return _Insert_or_assign(key, std::forward<Mapped>(mapped_value));
    }

    template <class Mapped>
    std::pair<iterator, bool> insert_or_assign(key_type&& key, Mapped&& mapped_value) {
        return _Insert_or_assign(std::move(key), std::forward<Mapped>(mapped_value));
    }

    mapped_type& at(const key_type& key) { return const_cast<mapped_type&>(const_cast<const MapPack*>(this)->at(key)); }

    const mapped_type& at(const key_type& key) const {
        const auto res = TreeAccessor::find_lower_bound(Derptr(), key);
        if (res.curr->is_nil || Derptr()->key_comp()(key, KFN(res.curr)))
            throw std::out_of_range("invalid map<K, T> key");
        return res.curr->value.second;
    }

    mapped_type& operator[](const key_type& key) { return _Try_emplace(key).first->value.second; }
    mapped_type& operator[](key_type&& key) { return _Try_emplace(std::move(key)).first->value.second; }

private:
    Derived* Derptr() const noexcept { return const_cast<Derived*>(static_cast<const Derived*>(this)); }

    template <class Key, class... Mapped>
    std::pair<Nodeptr, bool> _Try_emplace(Key&& key, Mapped&&... mapped_value) {
        const auto res  = TreeAccessor::find_lower_bound(Derptr(), key);
        auto       cmpr = Derptr()->key_comp();
        if (!res.curr->is_nil && !cmpr(key, KFN(res.curr)))
            return { res.curr, false };

        // clang-format off
        const Nodeptr new_node = TreeTempNode<_Alnode_type>(TreeAccessor::get_alnode(Derptr()),
            TreeAccessor::root(Derptr()), std::piecewise_construct,
            std::forward_as_tuple(std::forward<Key>(key)), 
            std::forward_as_tuple(std::forward<Mapped>(mapped_value)...)).release();
        // clang-format on
        return { TreeAccessor::insert_at(Derptr(), res.pack, new_node), true };
    }

    template <class Key, class Mapped>
    std::pair<iterator, bool> _Insert_or_assign(Key&& key, Mapped&& mapped_value) {
        const auto res  = TreeAccessor::find_lower_bound(Derptr(), key);
        const auto cmpr = Derptr()->key_comp();
        if (!res.curr->is_nil && !cmpr(key, KFN(res.curr))) {
            res.pack.parent->value.second = std::forward<Mapped>(mapped_value);
            return { TreeAccessor::make_iter(Derptr(), res.pack.parent), false };
        }

        // clang-format off
        const Nodeptr new_node =
            TreeTempNode<_Alnode_type>(TreeAccessor::get_alnode(Derptr()), 
                TreeAccessor::root(Derptr()), 
                std::forward<Key>(key), 
                std::forward<Mapped>(mapped_value)).release();
        // clang-format on
        return { TreeAccessor::make_iter(Derptr(), TreeAccessor::insert_at(Derptr(), res.pack, new_node)), true };
    }

protected:
    MapPack() = default;
};

struct TreeAccessor {
    template <class Traits, template <class, class> class... MixIn>
    inline static auto /*Nodeptr*/ root(BinaryTree<Traits, MixIn...>* tree) noexcept {
        return tree->Getroot();
    }

    template <class Traits, template <class, class> class... MixIn>
    inline static auto /*Nodeptr*/ insert_at(BinaryTree<Traits, MixIn...>* tree, const typename BinaryTree<Traits, MixIn...>::Inspack& pack,
                                             typename BinaryTree<Traits, MixIn...>::Nodeptr new_node) noexcept {
        return tree->InsertAt(pack, new_node);
    }

    template <class Key, class Traits, template <class, class> class... MixIn>
    inline static auto /*FindResult*/ find_lower_bound(BinaryTree<Traits, MixIn...>* tree, const Key& key) noexcept {
        return tree->LowerBound(key);
    }

    template <class Key, class Traits, template <class, class> class... MixIn>
    inline static auto /*FindResult*/ find_upper_bound(BinaryTree<Traits, MixIn...>* tree, const Key& key) noexcept {
        return tree->UpperBound(key);
    }

    template <class Traits, template <class, class> class... MixIn>
    inline static auto /*iterator*/ make_iter(BinaryTree<Traits, MixIn...>* tree, typename Traits::Nodeptr node) noexcept {
        return tree->MakeIter(node);
    }

    template <class Traits, template <class, class> class... MixIn>
    inline static auto& /*_Alnode_type&*/ get_alnode(BinaryTree<Traits, MixIn...>* tree) noexcept {
        return tree->alloc;
    }
};

namespace {
    template <class Tp, class Compare>
    struct SetCate {
        using key_type      = Tp;
        using value_type    = key_type;
        using key_compare   = Compare;
        using value_compare = key_compare;

        template <class... Args>
        struct in_place_key_extract {
            static constexpr bool extractable = false;
        };

        template <>
        struct in_place_key_extract<Tp> {
            static constexpr bool extractable = true;
            static const Tp&      extract(const Tp& value) noexcept { return value; }
        };

        static const Tp& kfn(const Tp& value) { return value; }
    };

    template <class Key, class Value, class Compare>
    struct MapCate {
        using key_type    = Key;
        using value_type  = std::pair<const Key, Value>;
        using key_compare = Compare;
        struct value_compare {
            [[nodiscard]] bool operator()(const value_type& lhs, const value_type& rhs) const { return key_compare()(lhs.first, rhs.first); }
        };

        template <class... Args>
        struct in_place_key_extract {
            static constexpr bool extractable = false;
        };

        template <>
        struct in_place_key_extract<Key, Value> {
            static constexpr bool extractable = true;
            static const Key&     extract(const Key& key, const Value&) noexcept { return key; }
        };

        static const Key& kfn(const std::pair<const Key, Value>& value) { return value.first; }
    };

    template <class Cate, class Alloc, bool Mfl, class = void>
    struct bs_traits {
        using key_type        = typename Cate::key_type;
        using value_type      = typename Cate::value_type;
        using key_compare     = typename Cate::key_compare;
        using value_compare   = typename Cate::value_compare;
        using Node            = TreeNode<value_type>;
        using Nodeptr         = Node*;
        using allocator_type  = Alloc;
        using _Altp_traits    = std::allocator_traits<allocator_type>;
        using size_type       = typename _Altp_traits::size_type;
        using difference_type = typename _Altp_traits::difference_type;
        using pointer         = typename _Altp_traits::pointer;
        using const_pointer   = typename _Altp_traits::const_pointer;
        using reference       = value_type&;
        using const_reference = const value_type&;

        using ScaryVal       = ScaryTree<scary_iter_types<value_type, size_type, difference_type, pointer, const_pointer, reference, const_reference, Nodeptr>>;
        using const_iterator = bid_citer<ScaryVal>;
        using iterator       = std::conditional_t<std::is_same_v<key_type, value_type>, const_iterator, bid_iter<ScaryVal>>;

        using TraitsCategory = Cate;

        template <class... Args>
        using InPlaceKeyExtractor = typename Cate::template in_place_key_extract<Args...>;

        static constexpr bool Multi = Mfl;

        static const auto& kfn(const typename Cate::value_type& value) { return Cate::kfn(value); }

        template <template <class, class> class... MixIn>
        static void insert_fixup(BinaryTree<bs_traits<Cate, Alloc, Mfl, void>, MixIn...>*, Nodeptr) noexcept {
            // DO NOTHING
        }
        template <class Crtp, template <class, class> class... MixIn>
        static void insert_fixup(BinaryTree<bs_traits<Cate, Alloc, Mfl, Crtp>, MixIn...>* tree, Nodeptr node) {
            static_cast<Crtp*>(this)->insert_fixup(tree, node);
        }

        template <template <class, class> class... MixIn>
        static void erase_fixup(BinaryTree<bs_traits<Cate, Alloc, Mfl, void>, MixIn...>*, Nodeptr) noexcept {
            // DO NOTHING
        }
        template <class Crtp, template <class, class> class... MixIn>
        static void erase_fixup(BinaryTree<bs_traits<Cate, Alloc, Mfl, Crtp>, MixIn...>* tree, Nodeptr node) {
            static_cast<Crtp*>(this)->erase_fixup(tree, node);
        }

        template <template <class, class> class... MixIn>
        static void access_fixup(BinaryTree<bs_traits<Cate, Alloc, Mfl, void>, MixIn...>*, Nodeptr) noexcept {
            // DO NOTHING
        }
        template <class Crtp, template <class, class> class... MixIn>
        static void access_fixup(BinaryTree<bs_traits<Cate, Alloc, Mfl, Crtp>, MixIn...>* tree, Nodeptr node) {
            static_cast<Crtp*>(this)->access_fixup(tree, node);
        }

        template <template <class, class> class... MixIn>
        static Nodeptr extract_node(BinaryTree<bs_traits<Cate, Alloc, Mfl, void>, MixIn...>* tree, Nodeptr node) noexcept {
            return extract_node_impl(tree, node);
        }
        template <class Crtp, template <class, class> class... MixIn>
        static Nodeptr extract_node(BinaryTree<bs_traits<Cate, Alloc, Mfl, Crtp>, MixIn...>* tree, Nodeptr node) {
            return static_cast<Crtp*>(this)->extract_node(tree, node);
        }

    protected:
        /**
         *	@brief take node from tree.the old position will be occupied by its successor.
         *	@param tree : current tree
         *	@param node : the node need to be taken.
         *	@param suc : the successor of node.
         */
        template <class Traits, template <class, class> class... MixIn>
        static Nodeptr take_node(BinaryTree<Traits, MixIn...>* tree, Nodeptr node, Nodeptr suc) noexcept {
            if (node->parent == TreeAccessor::root(tree))
                TreeAccessor::root(tree)->parent = suc;
            else if (node == node->parent->left)
                node->parent->left = suc;
            else if (node == node->parent->right)
                node->parent->right = suc;
            if (!suc->is_nil)
                suc->parent = node->parent;
            return node->parent;
        }

        /**
         *	@brief extract node from tree and find its successor to replace the old node
         *	@param tree : current tree
         *	@param node : the node need to be extracted.
         *	@return parent of node.
         */
        template <class Traits, template <class, class> class... MixIn>
        static Nodeptr extract_node_impl(BinaryTree<Traits, MixIn...>* tree, Nodeptr node) noexcept {
            Nodeptr parent;
            if (node->left->is_nil)
                parent = take_node(tree, node, node->right);
            else if (node->right->is_nil)
                parent = take_node(tree, node, node->left);
            else {
                Nodeptr suc = Node::leftmost(node->right);
                if (suc->parent != node) {
                    parent             = take_node(tree, suc, suc->right);
                    suc->right         = node->right;
                    suc->right->parent = suc;
                }
                parent            = take_node(tree, node, suc);
                suc->left         = node->left;
                suc->left->parent = suc;
            }
            node->left = node->right = node->parent = TreeAccessor::root(tree);
            node->prop                              = 0;
            return parent;
        }
    };

}  // namespace

#define _DEFINE_ASSO_CONTAINER(NAME)                                                                                          \
    template <class Tp, class Compare = std::less<>, class Alloc = std::allocator<Tp>>                                        \
    using NAME##_set = BinaryTree<NAME##_traits<SetCate<Tp, Compare>, Alloc, false>>;                                         \
    template <class Tp, class Compare = std::less<>, class Alloc = std::allocator<Tp>>                                        \
    using NAME##_multiset = BinaryTree<NAME##_traits<SetCate<Tp, Compare>, Alloc, true>>;                                     \
    template <class Key, class Value, class Compare = std::less<>, class Alloc = std::allocator<std::pair<const Key, Value>>> \
    using NAME##_map = BinaryTree<NAME##_traits<MapCate<Key, Value, Compare>, Alloc, false>, MapPack>;                        \
    template <class Key, class Value, class Compare = std::less<>, class Alloc = std::allocator<std::pair<const Key, Value>>> \
    using NAME##_multimap = BinaryTree<NAME##_traits<MapCate<Key, Value, Compare>, Alloc, true>>;

_DEFINE_ASSO_CONTAINER(bs);

#undef _DEFINE_ASSO_CONTAINER