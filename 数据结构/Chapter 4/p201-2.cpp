#include <iostream>
using namespace std;
template <class Tp>
struct LinkedQueueNode {
    using value_type = Tp;

    LinkedQueueNode() = default;
    explicit LinkedQueueNode(const Tp& value, LinkedQueueNode* next = nullptr) : value(value), next(next) {}

    LinkedQueueNode* next = nullptr;
    Tp               value{};
};
template <class Tp>
class LinkedQueue {
    using link_type = LinkedQueueNode<Tp>*;
    using node      = LinkedQueueNode<Tp>;

public:
    using value_type      = Tp;
    using pointer         = Tp*;
    using reference       = Tp&;
    using difference_type = ptrdiff_t;
    using const_pointer   = const Tp*;
    using const_reference = const Tp&;
    using size_type       = size_t;

    LinkedQueue() = default;
    LinkedQueue(const LinkedQueue& x) { *this = x; }
    LinkedQueue(LinkedQueue&& x) noexcept : head(exchange(x.head, nullptr)), tail(exchange(x.tail, nullptr)), sz(x.sz) {}

    LinkedQueue& operator=(const LinkedQueue& x) {
        if (this != addressof(x) && !x.empty()) {
            head = tail = new node(x.head->value);
            for (link_type curr = x.head->next; curr; curr = curr->next)
                tail = tail->next = new node(curr->value);
            sz += x.size();
        }
        return *this;
    }
    LinkedQueue& operator=(LinkedQueue&& x) {
        if (this != addressof(x)) {
            head = exchange(x.head, nullptr);
            tail = exchange(x.tail, nullptr);
            sz   = x.sz;
        }
        return *this;
    }

    reference       front() noexcept { return head->value; }
    const_reference front() const noexcept { return head->value; }
    reference       back() noexcept { return tail->value; }
    const_reference back() const noexcept { return tail->value; }

    bool empty() const noexcept { return size() == 0; }

    size_type size() const noexcept { return sz; }

    void push_front(const_reference value) {
        head = head ? new node(value, head) : tail = new node(value);
        ++sz;
    }
    void push_back(const_reference value) {
        tail = tail->next = new node(value);
        if (!head)
            head = tail;
        ++sz;
    }
    void pop_front() noexcept {
        delete exchange(head, head->next);
        --sz;
    }
    void pop_back() noexcept {
        link_type *prev = addressof(head), curr, before = nullptr;
        for (; (curr = *prev) != tail; prev = addressof(curr->next))
            before = curr;
        *prev = nullptr;
        delete tail;
        tail = before;
        --sz;
    }

    ~LinkedQueue() {
        while (head)
            delete std::exchange(head, head->next);
    }

private:
    link_type head = nullptr, tail = nullptr;
    size_type sz = 0;
};

int main() {
    LinkedQueue<int> l, l2;
    l.push_front(1);
    l.push_back(2);
    l.push_front(3);
    l2 = l;
    while (!l2.empty()) {
        cout << l2.back() << " ";
        l2.pop_back();
    }
    return 0;
}