#include <iostream>
#include <vector>
using namespace std;

struct Term {
    double coef;
    int    exp;
};

class Polynomial {
public:
    template <class... Args>
    Polynomial(const Args... terms) : capacity(sizeof...(terms)), terms(0) {
        termArray = new Term[sizeof...(terms)];
        construct(terms...);
    }

    double evaluate(double x0) const {
        double res = 0.0;
        for (int i = 0; i < terms; ++i)
            res += pow(x0, termArray[i].exp) * termArray[i].coef;
        return res;
    }

    ~Polynomial() { delete[] termArray; }

private:
    template <class... Args>
    void construct(const Term& term, const Args... args) {
        if (terms == capacity) {
            capacity *= 2;
            Term* newArray = new Term[capacity];
            copy(termArray, termArray + terms, newArray);
            delete[] termArray;
            termArray = newArray;
        }
        termArray[terms++] = term;

        if constexpr (sizeof...(Args))
            construct(args...);
    }

    Term* termArray;
    int   terms;
    int   capacity;
};
int main() {
    Polynomial polynomial(Term(5, 0), Term(3, 2), Term(2, 4));
    cout << polynomial.evaluate(1.5);
}