#include "stuff.h"
#include <cassert>
#include <format>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>
using namespace std;

struct Term {
    Term(int exp = 0, double coef = 0.0) : exp(exp), coef(coef) {}
    Term(const Term&) = default;
    Term(Term&&) = default;
    template <class T, class = enable_if_t<is_same_v<remove_cvref_t<decltype(get<0>(declval<T>()))>, int>&& is_same_v<remove_cvref_t<decltype(get<1>(declval<T>()))>, double>>>
    Term(const T& term) : exp(get<0>(term)), coef(get<1>(term)) {}

    Term& operator=(const Term&) = default;
    Term& operator=(Term&&) = default;
    template <class T, class = enable_if_t<is_same_v<remove_cvref_t<decltype(get<0>(declval<T>()))>, int>&& is_same_v<remove_cvref_t<decltype(get<1>(declval<T>()))>, double>>>
    Term& operator=(const T& term) {
        coef = get<1>(term);
        exp = get<0>(term);
        return *this;
    }

    int    exp = 0;
    double coef = 0.0;
};

class Polynomial {
    using Self = Polynomial;
    using node = ListNode<Term>;
    using link_type = node*;

    struct avail_node_list {
        template <class Iter>
        static tuple<link_type, link_type, size_t> create_from_range(Iter first, Iter last) {  // return-type: head, tail, n
            using value_type = Term;

            size_t    n = 0;
            link_type res = av, prev = av;
            for (; first != last && av; ++first, av = av->next, ++n) {
                prev = av;
                av->value = *first;
            }

            if (av == nullptr && first != last) {
                CreateOp<value_type> c;
                n += c.create_from_range(first, last);
                auto [sub_head, sub_tail] = c.get();
                if (prev)
                    prev->next = sub_head;
                else
                    res = sub_head;
                return { res, sub_tail, n };
            }
            prev->next = nullptr;
            if (av)
                av = av->next;
            return { res, prev, n };
        }

        static link_type create_node(const Term& term) {
            if (av) {
                av->value = term;
                return exchange(av, av->next);
            }
            else {
                CreateOp<value_type> c;
                c.create_n(1, term);
                return c.get().first;
            }
        }

        static tuple<link_type, link_type, size_t> allocate_node(const size_t n) {
            if (n <= 0)
                return { nullptr, nullptr, 0 };
            link_type res = av, prev = av;
            size_type cnt = 0;
            for (; av && cnt < n; ++cnt, av = av->next)
                prev = av;

            if (av == nullptr && cnt < n) {
                CreateOp<value_type> c;
                cnt += c.create_n(n - cnt, value_type{});
                auto [sub_head, sub_tail] = c.get();
                if (prev)
                    prev->next = sub_head;
                else
                    res = sub_head;
                return { res, sub_tail, cnt };
            }
            prev->next = nullptr;
            if (av)
                av = av->next;
            return { res, prev, cnt };
        }

        static void recycle(link_type head, link_type tail) { tail->next = exchange(av, head); }

        static inline link_type av = nullptr;
    };

public:
    using value_type = Term;
    using pointer = Term*;
    using reference = Term&;
    using difference_type = ptrdiff_t;
    using const_pointer = const value_type*;
    using const_reference = const value_type&;
    using iterator = slist_iter<value_type>;
    using const_iterator = slist_citer<value_type>;
    using size_type = size_t;

    Polynomial() : head(addressof(head)), tail(addressof(head)) {}
    Polynomial(std::initializer_list<value_type> l) : Polynomial(l.begin(), l.end()) {}
    template <class Iter>
    Polynomial(Iter first, Iter last) : head(addressof(head)), tail(addressof(head)) {
        insert_or_merge(first, last);
    }
    Polynomial(const Polynomial& poly) : head(addressof(head)), tail(addressof(head)) { *this = poly; }
    Polynomial(Polynomial&& poly) noexcept : head(exchange(poly.head.next, addressof(poly.head))), tail(exchange(poly.tail, addressof(poly.head))), sz(poly.sz) { tail->next = addressof(head); }

    [[nodiscard]] iterator       begin() noexcept { return iterator(head.next); }
    [[nodiscard]] const_iterator begin() const noexcept { return const_iterator(head.next); }
    [[nodiscard]] const_iterator cbegin() const noexcept { return const_iterator(head.next); }
    [[nodiscard]] iterator       end() noexcept { return iterator(addressof(head)); }
    [[nodiscard]] const_iterator end() const noexcept { return const_iterator(const_cast<link_type>(addressof(head))); }
    [[nodiscard]] const_iterator cend() const noexcept { return const_iterator(const_cast<link_type>(addressof(head))); }

    size_type size() const noexcept { return sz; }
    bool      empty() const noexcept { return size() == 0; }

    template <class Iter, class = enable_if_t<std::is_convertible_v<typename iterator_traits<Iter>::iterator_category, std::forward_iterator_tag>>>
    void insert_or_merge(Iter first, Iter last) {
        auto [hd, tl, n] = avail_node_list::create_from_range(first, last);
        merge(hd, tl, n);
    }
    void insert_or_merge(const Term& term) {
        link_type tmp = avail_node_list::create_node(term);
        merge(tmp, tmp, 1);
    }
    void insert_or_merge(Polynomial&& poly) {
        if (this != addressof(poly)) {
            poly.tail->next = nullptr;
            merge(poly.head.next, poly.tail, poly.size());
        }
    }

    void erase(const_iterator position) { erase(position, next(position)); }
    void erase(const_iterator first, const_iterator last) {
        if (empty())
            return;
        //clang-format off
        link_type curr = addressof(head);
        for (; curr->next != first.node; curr = curr->next)
            ;
        link_type tl = curr;
        if (last.node == addressof(head))
            tl = exchange(tail, curr);
        else {
            for (; tl->next != last.node; tl = tl->next)
                ;
        }

        curr->next = last.node;

        avail_node_list::recycle(first.node, tl);
        // clang-format on
    }
    void clear() noexcept { erase(begin(), end()); }
    void remove_coef(int coef) {
        remove_if([&](const_reference _value) { return _value.coef == coef; });
    }
    void remove_exp(int exp) noexcept {
        remove_if([&](const_reference _value) { return _value.exp == exp; });
    }
    template <class Pred>
    size_type remove_if(Pred pred) noexcept {
        const size_type oldsz = sz;

        RemoveOp<value_type> op;
        link_type            curr = head.next, prev = addressof(head);
        while (curr != addressof(head)) {
            if (pred(curr->value)) {
                curr = op.take_node(prev);
                --sz;
            }
            else
                prev = exchange(curr, curr->next);
        }
        return oldsz - sz;
    }

    double evaluate(double x0) const {
        return accumulate(begin(), end(), 0.0, [=](double sum, const Term& term) { return sum + pow(x0, term.exp) * term.coef; });
    }

    Polynomial operator*(const Polynomial& poly) const {
        vector sums(poly.size(), *this);

        for (auto const [index, term] : enumerate(poly)) {
            for (auto& terms : sums[index]) {
                terms.exp += term.exp;
                terms.coef *= term.coef;
            }
        }

        map<int, double, greater<>> res;
        for (const auto& sum : sums)
            for (const auto& term : sum) {
                auto [it, suc] = res.insert({ term.exp, term.coef });
                if (!suc)
                    it->second += term.coef;
            }

        return Polynomial(res.begin(), res.end());
    }
    Polynomial& operator*=(const Polynomial& poly) { return (*this = move(*this * poly)); }

    Polynomial  operator-(const Polynomial& poly) const { return Polynomial(*this) -= poly; }
    Polynomial& operator-=(const Polynomial& poly) {
        Polynomial tmp = poly;
        for_each(tmp.begin(), tmp.end(), [](auto& v) { v.coef = -v.coef; });
        insert_or_merge(move(tmp));
        return *this;
    }

    Polynomial  operator+(const Polynomial& poly) const { return Polynomial(*this) += poly; }
    Polynomial& operator+=(const Polynomial& poly) {
        insert_or_merge(poly.begin(), poly.end());
        return *this;
    }

    Polynomial& operator=(const Polynomial& poly) {
        if (this != addressof(poly)) {

            link_type curr = head.next, curr2;
            for (curr2 = poly.head.next; curr != addressof(head) && curr2 != addressof(poly.head); curr2 = curr2->next) {
                curr->value = curr2->value;
                curr = curr->next;
            }
            if (curr != addressof(head)) {
                erase(iterator(curr), end());
            }
            else {
                insert_or_merge(const_iterator(curr2), poly.end());
            }
            sz = poly.sz;
        }
        return *this;
    }

    Polynomial& operator=(Polynomial&& poly) noexcept {
        if (this != addressof(poly)) {
            head.next = poly.head.next;
            poly.tail->next = addressof(head);
            tail = poly.tail;
            sz = poly.sz;
        }
        return *this;
    }

    friend ostream& operator<<(ostream& os, const Polynomial& poly) { return os << format("{}", poly) << endl; }
    friend istream& operator>>(istream& is, Polynomial& poly) {
        Polynomial::size_type n;
        is >> n;
        while (n--) {
            double coef;
            int    exp;
            is >> coef >> exp;
            Term t(exp, coef);
            poly.insert_or_merge(t);
        }
        return is;
    }

    ~Polynomial() { clear(); }

    // private:
    void merge(link_type hd, link_type tl, size_type n) {
        if (empty()) {
            head.next = hd;
            tail = tl;
            tail->next = addressof(head);
        }
        else {
            node      dummy;
            link_type curr = head.next, prev = addressof(dummy);
            auto      pop_front = [&n](link_type& nd) {
                link_type next = nd->next;
                avail_node_list::recycle(nd, nd);
                nd = next;
                --n;
            };
            while (curr != addressof(head) && hd != nullptr) {
                if (hd->value.exp > curr->value.exp) {
                    prev->next = exchange(hd, hd->next);
                }
                else {
                    if (hd->value.exp == curr->value.exp) {
                        curr->value.coef += hd->value.coef;
                        pop_front(hd);
                    }
                    if (curr->value.coef == 0) {
                        pop_front(curr);
                        continue;
                    }

                    else
                        prev->next = exchange(curr, curr->next);
                }
                prev = prev->next;
            }
            if (prev == addressof(dummy)) {
                tail = head.next = addressof(head);
            }
            else {
                if (curr == addressof(head) && hd == nullptr) {
                    prev->next = addressof(head);
                    tail = prev;
                }
                else if (curr == addressof(head)) {
                    prev->next = hd;
                    tl->next = addressof(head);
                    tail = tl;
                }
                else
                    prev->next = curr;
                head.next = dummy.next;
            }
        }
        sz += n;
    }

    node      head;
    link_type tail;
    size_type sz = 0;
};

template <>
struct formatter<Polynomial> : formatter<string_view> {
    auto format(const Polynomial& poly, format_context& ctx) {
        using value_type = Term;
        string tmp;
        if (poly.empty()) {
            tmp = "0";
        }
        else {
            auto it = poly.begin();
            for (; next(it) != poly.end(); ++it) {
                char next_sign;
                if (auto next_coef = next(it)->coef; next_coef > 0)
                    next_sign = '+';
                else if (next_coef < 0)
                    next_sign = '-';
                format_to(back_inserter(tmp), "{}x{} {} ", abs(it->coef), it->exp == 1 ? "" : "^" + to_string(it->exp), next_sign);
            }

            if (it->coef) {
                format_to(back_inserter(tmp), "{}", abs(it->coef));
                if (it->exp)
                    format_to(back_inserter(tmp), "x^{}", it->exp);
            }
        }

        return formatter<string_view>::format(tmp, ctx);
    }
};

int main() {

    Polynomial p1;
    cout << "input p1(n coef1 exp1 ... coefn expn)" << endl;
    cin >> p1;
    Polynomial p2;
    cout << "input p2(n coef1 exp1 ... coefn expn)" << endl;
    cin >> p2;
    cout << format("p1 + p2: {}\n", p1 + p2);
    cout << format("p1 - p2: {}\n", p1 - p2);
    cout << format("p1 * p2: {}\n", p1 * p2);
    cout << "evaluate at x0=2 of p1 is " << p1.evaluate(2);
}