#include <iostream>
using namespace std;

class Complex {
private:
    double realPart;
    double imaginaryPart;

public:
    Complex(double r = 0.0, double i = 0.0) : realPart(r), imaginaryPart(i) {}

    Complex add(const Complex& other) const {
        double r = realPart + other.realPart;
        double i = imaginaryPart + other.imaginaryPart;
        return Complex(r, i);
    }

    Complex subtract(const Complex& other) const {
        double r = realPart - other.realPart;
        double i = imaginaryPart - other.imaginaryPart;
        return Complex(r, i);
    }

    void print() { cout << "(" << realPart << ", " << imaginaryPart << ")"; }
};

int main() {
    {
        Complex c1(1.0, 7.0);
        Complex c2(9.0, 2.0);

        c1.print();
        cout << " + ";
        c2.print();

        Complex c3 = c1.add(c2);
        cout << " = ";
        c3.print();
    }
    cout << endl;
    {
        Complex c1(10.0, 1.0);
        Complex c2(11.0, 5.0);

        c1.print();
        cout << " - ";
        c2.print();

        Complex c3 = c1.subtract(c2);
        cout << " = ";
        c3.print();
    }
}