#include <algorithm>
#include <array>
#include <chrono>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <random>
#include <ranges>
#include <vector>
#pragma warning(disable : 4267)
constexpr auto EXPERIMENT_N = 10;
using namespace std;

template <class Derived>
class SortUnit {
public:
    static void do_experiment(ostream& os) {
        constexpr int n[] = { 500, 1000, 2000, 3000, 4000, 5000 };
        os << Derived::category << ",";
        for (auto cnt : n)
            os << cal_time(EXPERIMENT_N, cnt) << ",";
        os << endl;
    }

protected:
    static auto cal_time(int repeat_number, int n) {
        return cal_time(repeat_number, n, [=](const auto cnt) { return Derived::generate(cnt); });
    }

    template <class Gen>
    static auto cal_time(int repeat_number, int n, Gen gen) {
        chrono::duration<double, micro> sum{ 0 };
        vector<int>                     data = gen(n);
        for (auto _ : views::iota(int{ 0 }, repeat_number)) {
            auto start = chrono::high_resolution_clock::now();
            Derived::sort(data);
            auto end = chrono::high_resolution_clock::now();
            sum += chrono::duration_cast<decltype(sum)>(end - start);
        }
        return sum * 1.0 / repeat_number;
    }

    static vector<int> gen_most_time_consuming(int n) {
        array<vector<int>, 10>                     data;
        array<chrono::duration<double, micro>, 10> time{};
        data.fill(views::iota(0, n) | ranges::to<vector>());
        for (auto i : views::iota(0, static_cast<int>(data.size())))
            time[i] = cal_time(1, n, [&](const auto) {
                ranges::shuffle(data[i], mt19937{ random_device{}() });
                return data[i];
            });
        return data[ranges::distance(time.begin(), ranges::max_element(time))];
    }

    static int get_median(vector<int>& arr, int left, int right) {
        int mid = left + (right - left) / 2;
        if (arr[left] > arr[right])
            swap(arr[left], arr[right]);
        if (arr[mid] > arr[right])
            swap(arr[mid], arr[right]);
        if (arr[left] > arr[mid])
            swap(arr[left], arr[mid]);
        return arr[mid];
    }

    static int partition(vector<int>& arr, int left, int right) {
        int pivot = get_median(arr, left, right);

        while (left < right) {
            while (left < right && arr[left] <= pivot)
                left++;
            while (left < right && arr[right] > pivot)
                right--;
            if (left < right)
                swap(arr[left], arr[right]);
        }
        return left;
    }

    static void insertion_sort(vector<int>& arr, int first, int last) {
        int len = last - first;
        for (int i = first + 1; i < len; i++) {
            int j   = i - 1;
            int key = arr[i];
            while (j >= first && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
    }
};

class MergeSort : public SortUnit<MergeSort> {

public:
    constexpr static string_view category = "merge sort";

    static void sort(vector<int>& nums) { merge_sort(nums, 0, nums.size() - 1); }

    static vector<int> generate(int n) {
        auto arr = views::iota(0, n) | ranges::to<vector>();
#ifdef __cpp_explicit_this_parameter
        auto separate = [&](this auto self,
#else
        std::function<void(vector<int>&)> separate = [&](
#endif

                            vector<int>& nums) -> void {
            if (nums.size() == 2)
                swap(nums[0], nums[1]);
            else if (nums.size() > 2) {
                vector<int> left  = nums | views::stride(2) | ranges::to<vector>();
                vector<int> right = nums | views::drop(1) | views::stride(2) | ranges::to<vector>();
#ifdef __cpp_explicit_this_parameter
                self(left);
                self(right);
#else
                separate(left);
                separate(right);
#endif
                ranges::copy(left, nums.begin());
                ranges::copy(right, nums.begin() + left.size());
            }
        };
        separate(arr);
        return arr;
    }

private:
    static void merge_sort(vector<int>& nums, int left, int right) {
        int len = 1;
        while (len <= right) {
            for (int i = 0; i + len <= right; i += len << 1) {
                int low = i, mid = i + len, high = i + 2 * len;
                if (high > right + 1)
                    high = right + 1;
                inplace_merge(nums.begin() + low, nums.begin() + mid, nums.begin() + high);
            }
            len <<= 1;
        }
    }
};

class QuickSort : public SortUnit<QuickSort> {
    using Base = SortUnit<QuickSort>;

public:
    constexpr static string_view category = "quick sort";

    static void        sort(vector<int>& arr) { quick_sort(arr, 0, arr.size() - 1); }
    static vector<int> generate(int n) { return Base::gen_most_time_consuming(n); }

private:
    static void quick_sort(vector<int>& arr, int left, int right) {
        if (left >= right)
            return;

        int mid = partition(arr, left, right);
        quick_sort(arr, left, mid - 1);
        quick_sort(arr, mid + 1, right);
    }
};

class InsertionSort : public SortUnit<InsertionSort> {
public:
    constexpr static string_view category = "insertion sort";

    static void sort(vector<int>& arr) { insertion_sort(arr, 0, arr.size()); }

    static vector<int> generate(int n) { return views::iota(0, n) | views::reverse | ranges::to<vector>(); }
};

class HeapSort : public SortUnit<HeapSort> {
    using Base = SortUnit<HeapSort>;

public:
    constexpr static string_view category = "heap sort";

    static void        sort(vector<int>& arr) { heap_sort(arr, 0, arr.size() - 1); }
    static vector<int> generate(int n) { return Base::gen_most_time_consuming(n); }

private:
    static void heap_sort(vector<int>& arr, int left, int right) {
        build(arr);
        for (int i = right; i > left; i--) {
            swap(arr[left], arr[i]);
            adjust(arr, left, i);
        }
    }

    static void build(vector<int>& arr) {
        for (int i = arr.size() / 2 - 1; i >= 0; i--)
            adjust(arr, i, arr.size());
    }

    static void adjust(vector<int>& arr, int index, int length) {
        int left    = index * 2 + 1;
        int right   = index * 2 + 2;
        int biggest = index;
        if (left < length && arr[left] > arr[biggest])
            biggest = left;
        if (right < length && arr[right] > arr[biggest])
            biggest = right;
        if (biggest != index) {
            swap(arr[index], arr[biggest]);
            adjust(arr, biggest, length);
        }
    }
};

class OptimalSort : public SortUnit<OptimalSort> {
    using Base = SortUnit<OptimalSort>;

public:
    constexpr static string_view category = "optimal sort";

    static void        sort(vector<int>& arr) { optimal_sort(arr, 0, arr.size(), arr.size()); }
    static vector<int> generate(int n) { return Base::gen_most_time_consuming(n); }

private:
    static void optimal_sort(vector<int>& arr, int first, int last, int ideal) {
        while (true) {
            if (last - first <= 32) {  // small, insertion sort
                insertion_sort(arr, first, last);
                return;
            }
            if (ideal <= 0) {  // heap sort if too many divisions
                make_heap(arr.begin() + first, arr.begin() + last);
                sort_heap(arr.begin() + first, arr.begin() + last);
                return;
            }
            auto mid = partition(arr, first, last - 1);
            ideal    = (ideal >> 1) + (ideal >> 2);  // allow 1.5 log2(N) divisions
            if (mid - first < last - mid) {
                optimal_sort(arr, first, mid, ideal);
                first = mid;
            }
            else {
                optimal_sort(arr, mid, last, ideal);
                last = mid;
            }
        }
    }
};

int main() {
    fstream fs("data.csv", ios::out | ios::trunc);
    fs << ",500, 1000, 2000, 3000, 4000, 5000" << endl;
    InsertionSort::do_experiment(fs);
    MergeSort::do_experiment(fs);
    HeapSort::do_experiment(fs);
    QuickSort::do_experiment(fs);
    OptimalSort::do_experiment(fs);
    return 0;
}
