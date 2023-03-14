#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;
class Date {
public:
    Date() {
        struct tm* ptr;
        time_t     t = time(0);
        ptr          = localtime(&t);
        day          = ptr->tm_mday;
        month        = 1 + ptr->tm_mon;
        year         = ptr->tm_year + 1900;
    }
    Date(int ddd, int yyyy) : year(yyyy) { setMMDDFromDDD(ddd); }
    Date(int mm, int dd, int yy) : day(dd), month(mm), year(yy + 2000) {}
    Date(const string& monthName, int dd, int yyyy) : day(dd), year(yyyy) { setMMFromMonth(monthName); }
    void setDay(int d) { day = checkDay(d); }
    void setMonth(int m) {
        if (m >= 1 && m <= 12)
            month = m;
        else
            cout << "Invalid month. Month set to 1 (January)." << endl;
    }
    void print() const { cout << month << "/" << day << "/" << year << endl; }
    void printDDDYYYY() const { cout << convertDDToDDD() << " " << year << endl; }
    void printMMDDYY() const {
        int yy = convertYYYYToYY();
        cout << setw(2) << setfill('0') << month << "/" << setw(2) << setfill('0') << day << "/" << setw(2) << setfill('0') << yy
             << endl;
    }
    void printMonthDDYYYY() const { cout << convertMMToMonth(month) << " " << day << ", " << year << endl; }
    ~Date() { cout << "Date object destructor for date " << month << "/" << day << "/" << year << "\n\n"; }

private:
    int month;
    int day;
    int year;
    int checkDay(int testDay) const {
        static const int daysPerMonth[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        if (testDay > 0 && testDay <= daysPerMonth[month]) {
            return testDay;
        }
        if (month == 2 && testDay == 29 && isLeapYear()) {
            return testDay;
        }
        cout << "Invalid day (" << testDay << "). Day set to 1." << endl;
        return 1;
    }
    int daysInMonth(int m) const {
        static const int daysPerMonth[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        if (m == 2 && isLeapYear()) {
            return 29;
        }
        return daysPerMonth[m];
    }
    bool isLeapYear() const {
        if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0) {
            return true;
        }
        return false;
    }
    int convertDDToDDD() const {
        static const int daysPerMonth[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        int              ddd              = 0;
        for (int i = 1; i < month; i++)
            ddd += daysPerMonth[i];
        ddd += day;
        if (month > 2 && isLeapYear())
            ddd++;
        return ddd;
    }
    void setMMDDFromDDD(int ddd) {
        static const int daysPerMonth[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        int              i;
        for (i = 1; i <= 12; i++) {
            if (ddd <= daysPerMonth[i])
                break;
            if (i == 2 && isLeapYear())
                ddd--;
            ddd -= daysPerMonth[i];
        }
        month = i;
        day   = ddd;
    }
    string convertMMToMonth(int m) const {
        static const string monthNames[13] = { "",     "January", "February",  "March",   "April",    "May",     "June",
                                               "July", "August",  "September", "October", "November", "December" };
        if (m >= 1 && m <= 12)
            return monthNames[m];
        return "Invalid month";
    }
    void setMMFromMonth(string monthName) {
        static const string monthNames[13] = { "",     "January", "February",  "March",   "April",    "May",     "June",
                                               "July", "August",  "September", "October", "November", "December" };

        int i = find(monthNames, end(monthNames), monthName) - monthNames;
        if (i <= 12)
            month = i;
    }
    int  convertYYYYToYY() const { return year % 100; }
    void setYYYYFromYY(int yy) {
        if (yy >= 0 && yy <= 99)
            year = yy >= 0 && yy <= 29 ? 2000 + yy : 1900 + yy;
    }
};

int main() {
    Date date1(256, 1999);             // initialize using ddd yyyy format
    Date date2(3, 25, 04);             // initialize using mm/dd/yy format
    Date date3("September", 1, 2000);  // "month" dd, yyyy format
    Date date4;                        // initialize to current date with default constructor
    // print Date objects in default format
    date1.print();
    date2.print();
    date3.print();
    date4.print();
    cout << '\n';
    // print Date objects in 'ddd yyyy' format
    date1.printDDDYYYY();
    date2.printDDDYYYY();
    date3.printDDDYYYY();
    date4.printDDDYYYY();
    cout << '\n';
    // print Date objects in 'mm/dd/yy' format
    date1.printMMDDYY();
    date2.printMMDDYY();
    date3.printMMDDYY();
    date4.printMMDDYY();
    cout << '\n';
    // print Date objects in '"month" d, yyyy' format
    date1.printMonthDDYYYY();
    date2.printMonthDDYYYY();
    date3.printMonthDDYYYY();
    date4.printMonthDDYYYY();
    cout << endl;
    return 0;
}