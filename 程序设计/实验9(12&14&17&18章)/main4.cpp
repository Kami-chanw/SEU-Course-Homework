#include <iostream>
#include <stdexcept>

using namespace std;

class TestException : public runtime_error {
public:
    TestException(const string& message) : runtime_error(message) {}
};

void f() {
    try {
        throw TestException("This is a test");
    } catch (const TestException& e) {
        cerr << e.what() << endl;
        throw;
    }
}

int main() {
    try {
        f();
    } catch (const TestException& e) {
        cerr << "abnormal program termination" << endl;
    }
    return 0;
}
