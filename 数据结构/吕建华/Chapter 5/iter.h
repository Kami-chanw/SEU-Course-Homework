#pragma once

#include <cassert>
#include <compare>
#include <memory>

template <class Value_type, class Size_type, class Difference_type, class Pointer, class Const_pointer, class Reference, class Const_reference, class Nodeptr_type>
struct scary_iter_types {  // for SCARY machinery
    using value_type      = Value_type;
    using size_type       = Size_type;
    using difference_type = Difference_type;
    using pointer         = Pointer;
    using const_pointer   = Const_pointer;
    using reference       = Reference;
    using const_reference = Const_reference;
    using Node            = std::remove_pointer_t<Nodeptr_type>;
    using Nodeptr         = Nodeptr_type;
};

template <class Ty, class = void>
struct get_pointer {
    using type = typename Ty::value_type*;
};

template <class Ty>
struct get_pointer<Ty, std::void_t<typename Ty::pointer>> {
    using type = typename Ty::pointer;
};

template <class Ty>
using get_pointer_t = typename get_pointer<Ty>::type;

template <class Ty, class = void>
struct get_const_pointer {
    using type = typename std::pointer_traits<get_pointer_t<Ty>>::template rebind<const typename Ty::value_type>;
};

template <class Ty>
struct get_const_pointer<Ty, std::void_t<typename Ty::const_pointer>> {
    using type = typename Ty::const_pointer;
};

template <class Ty>
using get_const_pointer_t = typename get_const_pointer<Ty>::type;

template <class Ty, class = void>
struct get_reference {
    using type = typename Ty::value_type&;
};

template <class Ty>
struct get_reference<Ty, std::void_t<typename Ty::reference>> {
    using type = typename Ty::reference;
};

template <class Ty>
using get_reference_t = typename get_reference<Ty>::type;

template <class Ty, class = void>
struct get_const_reference {
    using type = const typename Ty::value_type&;
};

template <class Ty>
struct get_const_reference<Ty, std::void_t<typename Ty::const_reference>> {
    using type = typename Ty::const_reference;
};

template <class Ty>
using get_const_reference_t = typename get_const_reference<Ty>::type;

template <class Ty, class = void>
struct get_difference {
    using type = typename std::pointer_traits<get_pointer_t<Ty>>::difference_type;
};

template <class Ty>
struct get_difference<Ty, std::void_t<typename Ty::difference_type>> {
    using type = typename Ty::difference_type;
};

template <class Ty>
using get_difference_t = typename get_difference<Ty>::type;

template <class ScaryVal>
class bid_citer {
    using Self    = bid_citer<ScaryVal>;
    using Nodeptr = typename ScaryVal::Nodeptr;

public:
    using value_type        = typename ScaryVal::value_type;
    using pointer           = get_const_pointer_t<ScaryVal>;
    using reference         = get_const_reference_t<ScaryVal>;
    using difference_type   = get_difference_t<ScaryVal>;
    using iterator_category = std::bidirectional_iterator_tag;

    constexpr bid_citer() = default;
    constexpr bid_citer(Nodeptr node) : node(node) {}

    constexpr reference          operator*() const noexcept { return ScaryVal::extract(this->node); }
    constexpr pointer            operator->() const noexcept { return std::addressof(ScaryVal::extract(this->node)); }
    [[nodiscard]] constexpr bool operator==(const Self& rhs) const noexcept(noexcept(std::declval<Nodeptr>() == std::declval<Nodeptr>())) { return rhs.node == this->node; }
    [[nodiscard]] constexpr bool operator!=(const Self& rhs) const noexcept(noexcept(*this == rhs)) { return !(*this == rhs); }

    constexpr Nodeptr base() const { return node; }

    constexpr Self& operator++() {
        ScaryVal::incr(this->node);
        return *this;
    }

    constexpr Self operator++(int) {
        Self tmp = *this;
        ++*this;
        return tmp;
    }

    constexpr Self& operator--() {
        ScaryVal::decr(this->node);
        return *this;
    }

    constexpr Self operator--(int) {
        Self tmp = *this;
        --*this;
        return tmp;
    }

protected:
    Nodeptr node;
};

template <class ScaryVal>
class bid_iter : public bid_citer<ScaryVal> {
    using Self    = bid_iter<ScaryVal>;
    using Base    = bid_citer<ScaryVal>;
    using Nodeptr = typename ScaryVal::Nodeptr;

public:
    using value_type        = typename ScaryVal::value_type;
    using pointer           = get_pointer_t<ScaryVal>;
    using reference         = get_reference_t<ScaryVal>;
    using difference_type   = get_difference_t<ScaryVal>;
    using iterator_category = std::bidirectional_iterator_tag;

    constexpr bid_iter() = default;
    constexpr bid_iter(Nodeptr node) : Base(node) {}

    constexpr reference operator*() const noexcept { return ScaryVal::extract(this->node); }
    constexpr pointer   operator->() const noexcept { return std::addressof(ScaryVal::extract(this->node)); }

    constexpr Self& operator++() {
        ++static_cast<Base&>(*this);
        return *this;
    }
    constexpr Self operator++(int) {
        Self tmp = *this;
        ++*this;
        return tmp;
    }

    constexpr Self& operator--() {
        --static_cast<Base&>(*this);
        return *this;
    }

    constexpr Self operator--(int) {
        Self tmp = *this;
        --*this;
        return tmp;
    }
};

// propagate on container swap
template <class Alloc>
void alloc_pocs(Alloc& left, Alloc& right) noexcept(std::allocator_traits<Alloc>::propagate_on_container_swap::value) {
    using std::swap;
    if constexpr (std::allocator_traits<Alloc>::propagate_on_container_swap::value)
        swap(left, right);
    else
        assert(("containers incompatible for swap", left == right));
}

// propagate on container copy assignment
template <class Alloc>
void alloc_pocca(Alloc& left, const Alloc& right) noexcept {
    if constexpr (std::allocator_traits<Alloc>::propagate_on_container_copy_assignment::value)
        left = right;
}

// propagate on container move assignment
template <class Alloc>
void alloc_pocma(Alloc& left, Alloc& right) noexcept {
    if constexpr (std::allocator_traits<Alloc>::propagate_on_container_move_assignment::value)
        left = std::move(right);
}