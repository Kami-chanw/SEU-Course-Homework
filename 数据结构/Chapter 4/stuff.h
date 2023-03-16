#pragma once
#include <iterator>
#include <ranges>

template <class Tp>
struct ListNode {
    using value_type = Tp;

    ListNode() = default;
    ListNode(ListNode* next) : next(next) {}
    explicit ListNode(const Tp& value, ListNode* next = nullptr) : value(value), next(next) {}

    Tp        value{};
    ListNode* next = nullptr;
};

template <class Tp>
class slist_citer {
    using Self = slist_citer;

protected:
    using link_type = ListNode<Tp>*;

public:
    using value_type = Tp;
    using pointer = const Tp*;
    using reference = const Tp&;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    constexpr slist_citer() = default;
    constexpr explicit slist_citer(link_type node) : node(node) {}

    constexpr reference operator*() const noexcept { return node->value; }
    constexpr pointer   operator->() const noexcept { return std::addressof(operator*()); }

    [[nodiscard]] constexpr bool operator==(const Self& rhs) const noexcept(noexcept(std::declval<link_type>() == std::declval<link_type>())) { return rhs.node == node; }
    [[nodiscard]] constexpr bool operator!=(const Self& rhs) const noexcept(noexcept(*this == rhs)) { return !(*this == rhs); }

    constexpr Self& operator++() {
        node = node->next;
        return *this;
    }

    constexpr Self operator++(int) {
        Self tmp = *this;
        ++* this;
        return tmp;
    }

    link_type node = nullptr;
};
template <class Tp>
class slist_iter : public slist_citer<Tp> {
    using Self = slist_iter<Tp>;
    using Base = slist_citer<Tp>;
    using link_type = typename Base::link_type;

public:
    using value_type = Tp;
    using pointer = Tp*;
    using reference = Tp&;
    using difference_type = ptrdiff_t;
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
        ++* this;
        return tmp;
    }
};

// destroy [first, last)
template <class Tp>
size_t destroy(ListNode<Tp>* first, ListNode<Tp>* last) {
    using value_type = Tp;

    size_t sz = 0;
    for (; first != last; ++sz)
        delete std::exchange(first, first->next);

    return sz;
}

template <class Tp>
struct CreateOp {
    using size_type = size_t;
    using pointer = ListNode<Tp>*;
    using value_type = Tp;

    CreateOp() = default;

    CreateOp(const CreateOp&) = delete;
    CreateOp& operator=(const CreateOp&) = delete;

    template <class Iter>
    size_type create_from_range(Iter first, Iter last) {
        head = tail = new ListNode<Tp>(*first);
        size_type cnt = 1;
        for (; ++first != last; tail = tail->next, ++cnt) {
            tail->next = new ListNode<Tp>(*first);
        }
        return cnt;
    }

    size_type create_n(size_type n, const Tp& value) {
        head = tail = new ListNode<Tp>(value);
        size_type cnt = 1;
        for (; --n; tail = tail->next, ++cnt) {
            tail->next = new ListNode<Tp>(value);
        }
        return cnt;
    }

    auto get() noexcept { return std::make_pair(head, std::exchange(tail, nullptr)); }

    ~CreateOp() {
        if (tail)
            destroy(head, tail);
    }

private:
    pointer head = nullptr;
    pointer tail = nullptr;
};

template <class Tp>
struct RemoveOp {
    using value_type = Tp;
    using pointer = ListNode<Tp>*;

    RemoveOp() noexcept : head(), tail(std::addressof(head)) {}

    RemoveOp(const RemoveOp&) = delete;
    RemoveOp& operator=(const RemoveOp&) = delete;

    pointer take_node(pointer prev) noexcept {
        const pointer curr = prev->next;
        const pointer next = curr->next;

        prev->next = next;
        push(curr);
        return next;
    }

    void push(pointer curr) {
        *tail = curr;
        tail = std::addressof(curr->next);
    }

    ~RemoveOp() { destroy(head, *tail); }

private:
    pointer  head;
    pointer* tail;
};

template <typename T>
struct EnumeratedElement {
    std::size_t index;
    T& element;
};

auto enumerate(std::ranges::range auto& range)->std::ranges::view auto{
    return range | std::views::transform([i = std::size_t{}](auto& element) mutable { return EnumeratedElement{ i++, element }; });
}