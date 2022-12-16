#include <iostream>
using namespace std;
template <class Tp>
struct list_node {
    using value_type = Tp;

    list_node() = default;
    list_node(list_node* next) : next(next) {}
    explicit list_node(const Tp& value, list_node* next = nullptr) : value(value), next(next) {}

    Tp        value{};
    list_node* next = nullptr;
};

template <class Tp>
class slist_citer {
    using Self = slist_citer;

protected:
    using link_type = list_node<Tp>*;

public:
    using value_type        = Tp;
    using pointer           = const Tp*;
    using reference         = const Tp&;
    using difference_type   = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    constexpr slist_citer() = default;
    constexpr explicit slist_citer(link_type node) : node(node) {}

    constexpr reference operator*() const noexcept { return node->value; }
    constexpr pointer   operator->() const noexcept { return std::addressof(operator*()); }

    [[nodiscard]] constexpr bool operator==(const Self& rhs) const
        noexcept(noexcept(std::declval<link_type>() == std::declval<link_type>())) {
        return rhs.node == node;
    }
    [[nodiscard]] constexpr bool operator!=(const Self& rhs) const noexcept(noexcept(*this == rhs)) { return !(*this == rhs); }

    constexpr Self& operator++() {
        node = node->next;
        return *this;
    }

    constexpr Self operator++(int) {
        Self tmp = *this;
        ++*this;
        return tmp;
    }

    link_type node = nullptr;
};
template <class Tp>
class slist_iter : public slist_citer<Tp> {
    using Self      = slist_iter<Tp>;
    using Base      = slist_citer<Tp>;
    using link_type = typename Base::link_type;

public:
    using value_type        = Tp;
    using pointer           = Tp*;
    using reference         = Tp&;
    using difference_type   = ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    constexpr slist_iter() = default;
    constexpr explicit slist_iter(link_type node) : Base(node) {}

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

// destroy [first, last)
template <class Tp>
size_t destroy(list_node<Tp>* first, list_node<Tp>* last) {
    using value_type = Tp;

    size_t sz = 0;
    for (; first != last; ++sz)
        delete std::exchange(first, first->next);

    return sz;
}

template <class Tp>
struct CreateOp {
    using size_type  = size_t;
    using pointer    = list_node<Tp>*;
    using value_type = Tp;

    CreateOp() = default;

    CreateOp(const CreateOp&)            = delete;
    CreateOp& operator=(const CreateOp&) = delete;

    template <class Iter>
    size_type create_from_range(Iter first, Iter last) {
        head = tail   = new list_node<Tp>(*first);
        size_type cnt = 1;
        for (; ++first != last; tail = tail->next, ++cnt) {
            tail->next = new list_node<Tp>(*first);
        }
        return cnt;
    }

    size_type create_n(size_type n, const Tp& value) {
        head = tail   = new list_node<Tp>(value);
        size_type cnt = 1;
        for (; --n; tail = tail->next, ++cnt) {
            tail->next = new list_node<Tp>(value);
        }
        return cnt;
    }

    auto get() noexcept { return std::pair(head, std::exchange(tail, nullptr)); }

    ~CreateOp() {
        if (tail)
            destroy(head, tail);
    }

private:
    pointer head = nullptr;
    pointer tail = nullptr;
};

template <class Tp>
class slist {
    using Self      = slist;
    using node      = list_node<Tp>;
    using link_type = node*;

public:
    using value_type      = Tp;
    using pointer         = Tp*;
    using reference       = Tp&;
    using difference_type = ptrdiff_t;
    using const_pointer   = const Tp*;
    using const_reference = const Tp&;
    using iterator        = slist_iter<Tp>;
    using const_iterator  = slist_citer<Tp>;
    using size_type       = size_t;

    slist() = default;
    slist(std::initializer_list<value_type> l) : slist(l.begin(), l.end()) {}
    template <class Iter>
    slist(Iter first, Iter last) {
        insert(begin(), first, last);
    }
    slist(const slist& x) : slist(x.begin(), x.end()) {}
    slist(slist&& x) noexcept : hd(std::exchange(x.hd, nullptr)), sz(x.hd) {}

    [[nodiscard]] iterator       begin() noexcept { return iterator(hd); }
    [[nodiscard]] const_iterator begin() const noexcept { return const_iterator(hd); }
    [[nodiscard]] const_iterator cbegin() const noexcept { return const_iterator(hd); }
    [[nodiscard]] iterator       end() noexcept { return iterator(nullptr); }
    [[nodiscard]] const_iterator end() const noexcept { return const_iterator(nullptr); }
    [[nodiscard]] const_iterator cend() const noexcept { return const_iterator(nullptr); }

    template <class Iter, class = std::enable_if_t<
                              std::is_convertible_v<typename std::iterator_traits<Iter>::iterator_category, input_iterator_tag>>>
    void insert(const_iterator position, Iter first, Iter last) {
        CreateOp<Tp> c;
        sz += c.create_from_range(first, last);
        auto [head, tail] = c.get();
        make_link(position.node, head, tail);
    }
    void insert(const_iterator position, std::initializer_list<Tp> l) { insert(position, l.begin(), l.end()); }
    void insert(const_iterator position, size_type n, const_reference value) {
        CreateOp<Tp> c;
        sz += c.create_n(n, value);
        auto [head, tail] = c.get();
        make_link(position.node, head, tail);
    }
    void insert(const_iterator position, const_reference value) { insert(position, 1, value); }

    void insertion_sort() {
        node dummy;
        dummy.next     = hd;
        link_type cur   = hd;

        while (cur && cur->next) {
            if (cur->value <= cur->next->value)
                cur = cur->next;
            else {
                link_type pre = &dummy;
                while (pre->next->value <= cur->next->value)
                    pre = pre->next;
                link_type temp = cur->next;
                cur->next      = temp->next;
                temp->next     = exchange(pre->next, temp);
            }
        }

        hd = dummy.next;
    }

    void erase(const_iterator first, const_iterator last) noexcept {
        sz -= destroy(first.node, break_link(first.node, last.node));
    }
    void erase(const_iterator position) noexcept { erase(position, std::next(position)); }


    void clear() noexcept { erase(begin(), end()); }

    bool empty() const noexcept { return size() == 0; }

    size_type size() const noexcept { return sz; }

    slist& operator=(const slist& x) {
        if (this != std::addressof(x)) {
            insert(begin(), x.begin(), x.end());
        }
        return *this;
    }
    slist& operator=(slist&& x) noexcept {
        if (this != std::addressof(x)) {
            hd = std::exchange(x.hd, nullptr);
            sz = hd.sz;
        }
        return *this;
    }

    ~slist() noexcept { clear(); }

protected:
    // insert [head, tail]
    link_type make_link(link_type pos, link_type head, link_type tail) noexcept {
        link_type *prev = std::addressof(hd), curr;
        while ((curr = *prev) != pos)
            prev = std::addressof(curr->next);
        tail->next = curr;
        *prev      = head;
        return head;
    }

    // remove [first, last)
    link_type break_link(link_type first, link_type last = nullptr) noexcept {
        link_type *prev = std::addressof(hd), curr;
        while ((curr = *prev) != first)
            prev = std::addressof(curr->next);
        *prev = last;
        return last;
    }

    link_type hd = nullptr;  // head

    size_type sz = 0;  // size
};
int main() {
    slist<int> l;
    l.insert(l.begin(), {5,4,3,2,1});
    l.insertion_sort();
    for (auto i : l)
        cout << i << " ";

}