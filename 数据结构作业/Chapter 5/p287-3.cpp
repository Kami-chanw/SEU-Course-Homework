#include <algorithm>
#include <chrono>
#include <deque>
#include <fstream>
#include <iostream>
#include <random>

using namespace std;
#define COUNT_TIME(OP, SUM)                                       \
    do {                                                          \
        auto start = chrono::high_resolution_clock::now();        \
        OP;                                                       \
        auto end = chrono::high_resolution_clock::now();          \
        SUM += chrono::duration_cast<decltype(SUM)>(end - start); \
    } while (0)  // ”Ô—‘∂•’Î£∫º¯∂®Œ™c-lang‘„∆…

template <class T>
class orderd_list {
public:
    void push(const T& value) {
        ol.push_back(value);
        inplace_merge(ol.begin(), ol.end() - 1, ol.end(), greater<>());
    }

    void pop() { ol.pop_front(); }

private:
    deque<T> ol;
};
template <class T>
class unorderd_list {
public:
    void push(const T& value) {
        if (!ul.empty() && value > ul.front())
            ul.push_front(value);
        else
            ul.push_back(value);
    }

    void pop() {
        ul.pop_front();
        if (!ul.empty())
            iter_swap(ul.begin(), max_element(ul.begin(), ul.end()));
    }

private:
    deque<T> ul;
};
template <class T>
class max_heap {
public:
    void push(const T& value) {
        heap.push_back(value);
        if (heap.size() >= 2) {
            int        hole = heap.size() - 1, des;
            deque<int> path;
            for (des = (hole - 1) >> 1; des >= 0; des = (des - 1) >> 1)
                path.push_back(des);
            reverse(path.begin(), path.end());
            auto res = lower_bound(path.begin(), path.end(), value, [&](int i, const T&) { return heap[i] > value; });

            if (res != end(path)) {
                des = *res;
                for (int i = (hole - 1) >> 1; i >= des; i = (hole - 1) >> 1) {
                    heap[hole] = move(heap[i]);
                    hole = i;
                }
                heap[hole] = value;
            }
        }
    }

    void pop() {
        if (heap.size() >= 2) {
            int       i = 1, hole = 0;
            const int bottom = heap.size() - 1;
            swap(heap.front(), heap.back());
            T value = move(heap.front());
            for (; i < bottom; hole = i, i = 2 * i + 1) {
                if (i < bottom - 1 && heap[i] < heap[i + 1])
                    ++i;
                if (heap[i] < value)
                    break;
                heap[hole] = move(heap[i]);
            }
            heap[hole] = move(value);
        }
        heap.pop_back();
    }

private:
    deque<T> heap;
};

enum Op { PUSH, POP };

int main() {
    constexpr int n[] = { 100, 500, 1000, 2000, 3000, 4000 }, m = 1000;
    fstream       fs("data.csv", ios::trunc | ios::out);
    fs << "n,aver time of ul(us), aver time of ol(us), aver time of hp(us)" << endl;
    vector<Op> seq;

    mt19937                    mt{ random_device()() };
    binomial_distribution<>    op_dis;
    uniform_int_distribution<> dis;
    unorderd_list<int>         ul;
    orderd_list<int>           ol;
    max_heap<int>              hp;
    generate_n(back_inserter(seq), m, [&]() { return static_cast<Op>(op_dis(mt)); });
    for (int cnt : n) {
        chrono::duration<double, micro> hp_time{}, ol_time{}, ul_time{};
        vector<int>                     data;
        generate_n(back_inserter(data), cnt, [&]() { return dis(mt); });
        for (int d : data) {
            hp.push(d);
            ul.push(d);
            ol.push(d);
        }
        for (Op op : seq) {
            if (op == PUSH) {
                int val = dis(mt);
                COUNT_TIME(ol.push(val), ol_time);
                COUNT_TIME(ul.push(val), ul_time);
                COUNT_TIME(hp.push(val), hp_time);
            }
            else {
                COUNT_TIME(ol.pop(), ol_time);
                COUNT_TIME(ul.pop(), ul_time);
                COUNT_TIME(hp.pop(), hp_time);
            }
        }
        fs << cnt << "," << ul_time / m << "," << ol_time / m << "," << hp_time / m << endl;
    }
}