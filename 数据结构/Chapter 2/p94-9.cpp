// use c++20 standard
#include <algorithm>
#include <compare>
#include <format>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <vector>
#pragma warning(disable : 4267)
using namespace std;

struct Term {
	Term(int exp = 0, double coef = 0.0) : exp(exp), coef(coef) {}
	Term(const Term&) = default;
	Term(Term&&) = default;
	template <class T, class = enable_if_t<is_same_v<remove_cvref_t<decltype(get<0>(declval<T>()))>, int>&& is_same_v<remove_cvref_t<decltype(get<1>(declval<T>()))>, double>>>
	Term(const T& term) : exp(get<0>(term)), coef(get<1>(term)) {}

	Term& operator=(const Term&) = default;
	Term& operator=(Term&&) = default;
	template <class T, class = enable_if_t<is_same_v<remove_cvref_t<decltype(get<0>(declval<T>()))>, int>&& is_same_v<remove_cvref_t<decltype(get<1>(declval<T>()))>, double>>>
	Term& operator=(const T& term) {
		coef = get<1>(term);
		exp = get<0>(term);
		return *this;
	}

	int    exp = 0;
	double coef = 0.0;
};

struct Comparer {
	bool operator()(const Term& lhs, const Term& rhs) { return lhs.exp > rhs.exp; }
};

class Polynomial {
	friend class formatter<Polynomial>;

public:
	template <class... Args>
	Polynomial(const Args&... terms) : start(free), finish(free - 1) {
		if constexpr (sizeof...(terms)) {
			checkCapacity(sizeof...(terms));
			size_t sz = construct<0>(terms...) + 1;
			free += sz;
			finish = start + sz - 1;
			sort(termArray + start, termArray + finish + 1, Comparer());
		}
	}

	template <class Iter, class = enable_if_t<std::is_convertible_v<typename iterator_traits<Iter>::iterator_category, std::bidirectional_iterator_tag>>>
	Polynomial(Iter first, Iter last) : start(free), finish(free - 1) {
		size_t sz = distance(first, last);
		checkCapacity(sz);
		sz = copy_if(first, last, termArray + start, [](const Term& term) { return term.coef; }) - (termArray + start);

		finish = start + sz - 1;
		free += sz;
		sort(termArray + start, termArray + finish + 1, Comparer());
	}

	Polynomial(initializer_list<Term> l) : Polynomial(l.begin(), l.end()) {}

	Polynomial(const Polynomial& other) { *this = other; }

	Polynomial(Polynomial&& other) noexcept : start(other.start), finish(other.finish) {}

	Polynomial add(const Polynomial& rhs) const { return *this + rhs; }

	Polynomial multiply(const Polynomial& rhs) const { return *this * rhs; }

	double evaluate(double x0) const {
		return accumulate(termArray + start, termArray + finish + 1, 0.0, [=](double sum, const Term& term) { return sum + pow(x0, term.exp) * term.coef; });
	}

	Polynomial& operator+=(const Polynomial& rhs) { return (*this = move(*this + rhs)); }
	Polynomial  operator+(const Polynomial& rhs) const {
		map<int, double, greater<>> terms;
		for (int i = start; i <= finish; ++i)
			terms[termArray[i].exp] = termArray[i].coef;
		for (int i = rhs.start; i <= rhs.finish; ++i)
			terms[termArray[i].exp] += termArray[i].coef;
		erase_if(terms, [](const auto& term) { return abs(term.second) < 1e-6; });
		return Polynomial(terms.begin(), terms.end());
	}

	Polynomial& operator*=(const Polynomial& rhs) { return (*this = move(*this * rhs)); }
	Polynomial  operator*(const Polynomial& rhs) const {
		vector<vector<Term>> sums(rhs.size(), vector<Term>(termArray + start, termArray + finish + 1));

		for (int i = 0; i < rhs.size(); ++i) {
			for (auto& terms : sums[i]) {
				terms.exp += termArray[rhs.start + i].exp;
				terms.coef *= termArray[rhs.start + i].coef;
			}
		}

		map<int, double, greater<>> res;
		for (const auto& sum : sums)
			for (const auto& term : sum) {
				auto [it, suc] = res.insert({ term.exp, term.coef });
				if (!suc)
					it->second += term.coef;
			}
		return Polynomial(res.begin(), res.end());
	}

	Polynomial& operator=(const Polynomial& rhs) {
		if (this != addressof(rhs)) {
			if (rhs.size() > size()) {
				start = free;
				free += rhs.size();
				finish = free - 1;
			}
			else
				finish = start + static_cast<int>(rhs.size()) - 1;
			copy(termArray + rhs.start, termArray + rhs.finish + 1, termArray + start);
		}
		return *this;
	}

	Polynomial& operator=(Polynomial&& rhs) noexcept {
		if (this != addressof(rhs)) {
			start = rhs.start;
			finish = rhs.finish;
		}
		return *this;
	}

	friend ostream& operator<<(ostream& os, const Polynomial& poly) { return os << format("{}", poly) << endl; }

	size_t size() const { return max(finish - start + 1, 0); }

private:
	template <size_t Idx, class... Args>
	size_t construct(const Term& term, const Args&... args) {
		size_t sz = Idx;
		if (abs(term.coef) > 1e-6) {
			termArray[start + Idx] = term;
			if constexpr (sizeof...(Args))
				sz = construct<Idx + 1>(args...);
		}
		else {
			if constexpr (sizeof...(Args))
				sz = construct<Idx>(args...);
		}
		return sz;
	}

	void checkCapacity(size_t expect) {
		if (expect + free > capacity) {
			int oldCapacity = capacity;
			do {
				capacity *= 2;
			} while (expect < capacity);
			Term* newArray = new Term[capacity];
			copy(termArray, termArray + oldCapacity, newArray);
			delete[] termArray;
			termArray = newArray;
		}
	}

	static Term* termArray;
	static int   capacity;
	static int   free;

	int start, finish;
};

template <>
struct formatter<Polynomial> : formatter<string_view> {
	auto format(const Polynomial& poly, format_context& ctx) {
		string tmp;
		for (int i = poly.start; i < poly.finish; i++)
			format_to(back_inserter(tmp), "{}x^{} + ", poly.termArray[i].coef, poly.termArray[i].exp);
		format_to(back_inserter(tmp), "{}", poly.termArray[poly.finish].coef);
		if (poly.termArray[poly.finish].exp)
			format_to(back_inserter(tmp), "x^{}", poly.termArray[poly.finish].exp);
		return formatter<string_view>::format(tmp, ctx);
	}
};

Term* Polynomial::termArray = new Term[100];
int   Polynomial::capacity = 100;
int   Polynomial::free = 0;

int main() {
	static_assert(!(__cplusplus >= 201103L && __cplusplus < 202002L), "requires c++20 standard or newer");
	cout << "1. show default polynomials calculation result" << endl;
	cout << "2. input custom polynomials" << endl;
	while (true) {
		int option;
		cin >> option;
		switch (option) {
		case 1: {
			Polynomial p1(Term(5, 0), Term(3, 2), Term(2, 4)), p2(Term(2, 4), Term(1, 7));
			Polynomial sum = p1 + p2;

			cout << format("polynomial 1 is {}\npolynomial 2 is {}\n", p1, p2);
			cout << format("sum of them is {}\nproduct of them is {}\n", sum, p1 * p2);
			cout << "when x0 is 2, the result of sum polynomial is " << sum.evaluate(2) << endl;
		} break;
		case 2: {
			cout << "input the n of polynomials :";
			int n;
			cin >> n;
			vector<Polynomial> v;
			for (int i = 0; i < n; i++) {
				cout << format("polynomial {}, input each exponent(int) and coefficient(double)", i + 1) << endl;
				cout << "press q to quit" << endl;
				string       expStr;
				double       coef;
				vector<Term> terms;
				while (true) {
					cin >> expStr;
					if (expStr == "q")
						break;
					cin >> coef;
					terms.emplace_back(stoi(expStr), coef);
				}
				v.emplace_back(terms.begin(), terms.end());
			}
			Polynomial sum = v[0], product = v[0];
			cout << format("polynomial {} is {}\n", 1, v[0]);
			for (int i = 1; i < n; ++i) {
				cout << format("polynomial {} is {}\n", i + 1, v[i]);
				sum += v[i];
				product *= v[i];
			}
			cout << format("sum of them is {}\nproduct of them is {}\n", sum, product);
			cout << "when x0 is 2, the result of sum polynomial is " << sum.evaluate(2) << endl;

		} break;
		default:
			return 0;
		}
	}
}