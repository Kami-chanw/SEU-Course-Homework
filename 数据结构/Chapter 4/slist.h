#pragma once
#include "stuff.h"
#include <utility>

template <class Tp>
class ForwardList {
    using Self = ForwardList;
    using node = ListNode<Tp>;
    using link_type = node*;

public:
    using value_type = Tp;
    using pointer = Tp*;
    using reference = Tp&;
    using difference_type = ptrdiff_t;
    using const_pointer = const Tp*;
    using const_reference = const Tp&;
    using iterator = slist_iter<Tp>;
    using const_iterator = slist_citer<Tp>;
    using size_type = size_t;

    ForwardList() = default;
    ForwardList(std::initializer_list<value_type> l) : ForwardList(l.begin(), l.end()) {}
    template <class Iter>
    ForwardList(Iter first, Iter last) {
        insert(begin(), first, last);
    }
    ForwardList(const ForwardList& x) : ForwardList(x.begin(), x.end()) {}
    ForwardList(ForwardList&& x) noexcept : hd(std::exchange(x.hd, nullptr)), sz(x.hd) {}

    [[nodiscard]] iterator       begin() noexcept { return iterator(hd); }
    [[nodiscard]] const_iterator begin() const noexcept { return const_iterator(hd); }
    [[nodiscard]] const_iterator cbegin() const noexcept { return const_iterator(hd); }
    [[nodiscard]] iterator       end() noexcept { return iterator(nullptr); }
    [[nodiscard]] const_iterator end() const noexcept { return const_iterator(nullptr); }
    [[nodiscard]] const_iterator cend() const noexcept { return const_iterator(nullptr); }

    template <class Iter, class = std::enable_if_t<std::is_convertible_v<typename std::iterator_traits<Iter>::iterator_category, input_iterator_tag>>>
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

    void erase(const_iterator first, const_iterator last) noexcept { sz -= destroy(first.node, break_link(first.node, last.node)); }
    void erase(const_iterator position) noexcept { erase(position, std::next(position)); }

    size_type remove(const_reference value) noexcept {
        return remove_if([&](const_reference _value) { return _value == value; });
    }
    template <class Pred>
    size_type remove_if(Pred pred) noexcept {
        const size_type oldsz = sz;
        RemoveOp<Tp>  op;
        link_type* prev = std::addressof(hd), curr;
        while ((curr = *prev)) {
            if (pred(curr->value)) {
                *prev = curr->next;
                op.push(curr);
                --sz;
            }
            prev = std::addressof(curr->next);
        }
        return oldsz - sz;
    }

    void clear() noexcept { erase(begin(), end()); }

    bool empty() const noexcept { return size() == 0; }

    size_type size() const noexcept { return sz; }

    ForwardList& operator=(const ForwardList& x) {
        if (this != std::addressof(x)) {
            insert(begin(), x.begin(), x.end());
        }
        return *this;
    }
    ForwardList& operator=(ForwardList&& x) noexcept {
        if (this != std::addressof(x)) {
            hd = std::exchange(x.hd, nullptr);
            sz = hd.sz;
        }
        return *this;
    }

    ~ForwardList() noexcept { clear(); }

protected:
    // insert [head, tail]
    link_type make_link(link_type pos, link_type head, link_type tail) noexcept {
        link_type* prev = std::addressof(hd), curr;
        while ((curr = *prev) != pos)
            prev = std::addressof(curr->next);
        tail->next = curr;
        *prev = head;
        return head;
    }

    // remove [first, last)
    link_type break_link(link_type first, link_type last = nullptr) noexcept {
        link_type* prev = std::addressof(hd), curr;
        while ((curr = *prev) != first)
            prev = std::addressof(curr->next);
        *prev = last;
        return last;
    }

    link_type hd = nullptr;  // head

    size_type sz = 0;  // size
};