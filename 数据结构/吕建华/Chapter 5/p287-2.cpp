#include <iostream>
#include <vector>
using namespace std;
template <class T>
class MinPQ {
public:
    virtual ~MinPQ() {}
    // virtual destructor
    virtual bool IsEmpty() const = 0;
    // return true iff thepriority queue is empty
    virtual const T& Top() const = 0;
    // return reference to max element
    virtual void Push(const T&) = 0;
    // add an element to the priority queue
    virtual void Pop() = 0;
    // delete element with max priority;
};

template <class T>
class MinHeap : public MinPQ<T> {
public:
    bool     IsEmpty() const override { return heap.empty(); }
    const T& Top() const override { return heap.front(); }
    void     Push(const T& value) override {
        heap.push_back(value);
        if (heap.size() >= 2) {
            int hole = heap.size() - 1;
            for (int i = (hole - 1) >> 1; hole > 0 && heap[i] > value; i = (hole - 1) >> 1) {
                heap[hole] = move(heap[i]);
                hole = i;
            }
            heap[hole] = move(value);
        }
    }
    void Pop() override {
        if (heap.size() >= 2) {
            int       i = 1, hole = 0;
            const int bottom = heap.size() - 1;
            swap(heap.front(), heap.back());
            T value = move(heap.front());
            for (; i < bottom; hole = i, i = 2 * i + 1) {
                if (i < bottom - 1 && heap[i] > heap[i + 1])  // find small subtree
                    ++i;
                if (heap[i] > value)
                    break;
                heap[hole] = move(heap[i]);
            }
            heap[hole] = move(value);
        }
        heap.pop_back();
    }

    template <class T>
    friend void print(const MinHeap<T>& h) {
        for (const auto& i : h.heap)
            cout << i << " ";
        cout << endl;
    }

    ~MinHeap() {}
private:
    vector<T> heap;
};

int main() {
    MinHeap<int> p;
    p.Push(14);
    p.Push(10);
    p.Push(20);
    p.Push(2);
    cout << "current heap is " << endl;
    print(p);
    cout << "after pop 2 elements" << endl;
    p.Pop();
    p.Pop();
    print(p);

}