#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>


using namespace std;

class Item {
public:
    Item() = default;
    Item(int value) : value(value) {
        cout << "Item " << value << " constructor called" << endl;
        if (value == 3) {
            throw runtime_error("An exception was thrown");
        }
    }

    ~Item() { cout << "Item " << value << " destructor called" << endl; }

private:
    int value;
};

int main() {
    cout << "Constructing an object of class ItemGroup" << endl;
    try {
        Item item1(1);
        Item item2(2);
        Item item3(3);
    } catch (const runtime_error& e) {
        cerr << e.what() << endl;
    }
    return 0;
}
