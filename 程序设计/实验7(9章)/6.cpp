#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
using namespace std;
class IntegerSet {
public:
    IntegerSet() { emptySet(); }

    IntegerSet(int arr[], int size) : IntegerSet() {
        for_each_n(arr, size, [this](const int x) {
            if (validEntry(x))
                insertElement(x);
            else
                cout << "Invalid insert attempted!" << endl;
        });
    }

    void emptySet() { fill_n(set, 101, 0); }

    void inputSet() {
        int num;
        while (true) {
            cout << "Enter an element (-1 to end): ";
            cin >> num;
            if (validEntry(num))
                insertElement(num);
            else if (num == -1)
                break;
            else
                cout << "Invalid Element" << endl;
        }
        cout << "Entry complete" << endl;
    }

    void insertElement(int k) {
        if (validEntry(k))
            set[k] = 1;
    }

    void deleteElement(int m) {
        if (validEntry(m))
            set[m] = 0;
    }

    void printSet() const {
        string res = "(   ";
        for (int i = 0; i <= 100; i++)
            if (set[i] == 1)
                res += to_string(i) + "  ";
        if (res == "(  ")
            cout << "---";
        else
            cout << res << " )" << endl;
    }

    bool isEqualTo(const IntegerSet& other) const { return equal(begin(set), end(set), begin(other.set)); }

    IntegerSet unionOfSets(const IntegerSet& other) const {
        IntegerSet result;
        for (int i = 0; i <= 100; i++)
            if (set[i] == 1 || other.set[i] == 1)
                result.set[i] = 1;
        return result;
    }

    IntegerSet intersectionOfSets(const IntegerSet& other) const {
        IntegerSet result;
        for (int i = 0; i <= 100; i++)
            if (set[i] == 1 && other.set[i] == 1)
                result.set[i] = 1;
        return result;
    }

private:
    int set[101];
    int validEntry(int x) const { return (x >= 0 && x <= 100); }
};

int main() {
    IntegerSet A, B;
    cout << "Enter set A:" << endl;
    A.inputSet();
    cout << "\nEnter set B:" << endl;
    B.inputSet();
    cout << endl;
    cout << "Union of A and B is:" << endl;
    A.unionOfSets(B).printSet();
    cout << "Intersection of A and B is:" << endl;
    A.intersectionOfSets(B).printSet();
    assert(!A.isEqualTo(B));
    cout << "Set A is no equal to set B" << endl;
    cout << "\nInserting 77 into set A...\nSet A is now:" << endl;
    A.insertElement(77);
    A.printSet();
    cout << "\nDeleting 77 from set A...\nSet A is now:" << endl;
    A.deleteElement(77);
    A.printSet();

    int arr[] = { 1, 2, 9, 25, 45, 105, 67, -3, 99, 100 };
    IntegerSet e(arr, extent<decltype(arr)>::value);
    cout << "\nSet e is:" << endl;
    e.printSet();
    return 0;
}