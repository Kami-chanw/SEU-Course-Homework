// Fig. 10.10: Array.h
// Array class definition with overloaded operators.
#ifndef ARRAY_H
#define ARRAY_H
#include <iomanip>
#include <iostream>
#include <stdexcept>

using namespace std;

template <typename T>
class Array {
    friend std::ostream& operator<<(ostream& os, const Array& arr) {
        for (size_t i = 0; i < arr.size; i++) {
            os << " " << arr.ptr[i];
        }
        return os;
    }

    friend std::istream& operator>>(istream& is, const Array& arr) {
        for (size_t i = 0; i < arr.size; i++) {
            is >> arr.ptr[i];
        }
        return is;
    }

public:
    explicit Array(int arraySize = 10)
        : size(arraySize > 0 ? arraySize : throw invalid_argument("Array size must be greater than 0")), ptr(new T[size]) {
        for (size_t i = 0; i < size; ++i)
            ptr[i] = 0;  // set pointer-based array element
    }                    // end Array default constructor // default constructor

    Array(const Array& arrayToCopy) : size(arrayToCopy.size), ptr(new T[size]) {
        for (size_t i = 0; i < size; ++i)
            ptr[i] = arrayToCopy.ptr[i];  // copy into object
    }                                     // end Array copy constructor
    // destructor for class Array // copy constructor

    ~Array() {
        delete[] ptr;  // release pointer-based array space
    }                  // end destructor

    // return number of elements of Array // destructor

    size_t getSize() const {
        return size;  // number of elements in Array
    }                 // end function getSize
                      // return size
private:
    size_t size;  // pointer-based array size
    T*     ptr;   // pointer to first element of pointer-based array

};  // end class Array
#endif
