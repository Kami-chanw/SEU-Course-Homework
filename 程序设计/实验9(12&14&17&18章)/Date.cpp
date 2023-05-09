// Date.cpp
#define _CRT_SECURE_NO_WARNINGS
#include "Date.h"
#include <array>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

using std::cout;
using std::endl;
using std::setfill;
using std::setw;

Date::Date()  // default constructor uses <ctime> functions to set date
{
    tm*    tPtr = NULL;
    time_t t    = time(0);
    tPtr        = localtime(&t);

    year  = tPtr->tm_year + 1900;  // tm_year表示从1900年起经过了多少年；
    month = tPtr->tm_mon + 1;      // tm_mon表示从1月起过了几个月；
    day = tPtr->tm_mday;  // tm_mday表示这个月过去了几天；tm_yday表示这一年过去了几天；tm_wday表示这一周过去了几天；
}
Date::Date(int Day, int Year)  // constructor using ddd yyyy format 111
{
    year = Year;
    setMMDDFromDDD(Day);
}
Date::Date(int Month, int Day, int Year)  // constructor using dd/mm/yy format 111
{
    setYYYYFromYY(Year);
    setMonth(Month);
    setDay(Day);
}
Date::Date(string Month, int Day, int Year)  // constructor using Month dd, yyyy format 111
{
    year = Year;
    setMMFromMonth(Month);
    setDay(Day);
}
void Date::setDay(int Day)  // set the day 111
{
    day = checkDay(Day);
}
void Date::setMonth(int Month)  // set the month 111
{
    if (Month >= 1 && Month <= 12)
        month = Month;
    else
        throw std::invalid_argument("Month must be between 1 and 12.");
}
bool Date::isSameMonth(const Date& date) const { return month == date.month; }
void Date::print() const  // print date in month/day/year format 111
{
    cout << month << '/' << day << '/' << year << endl;
}
void Date::printDDDYYYY() const  // print date in ddd yyyy format 111
{
    cout << convertDDToDDD() << " " << year << endl;
}
void Date::printMMDDYY() const  // print date in mm/dd/yy format 111
{
    cout << setw(2) << setfill('0') << month << '/' << setw(2) << day << '/' << setw(2) << convertYYYYToYY() << endl;
}
void Date::printMonthDDYYYY() const  // print date in Month dd, yyyy format 111
{
    cout << convertMMToMonth(month) << " " << day << ", " << year << endl;
}
Date::~Date()  // provided to confirm destruction order
{}

int month;  // 1-12 (January-December)
int day;    // 1-31 based on month
int year;   // any year
// utility functions
int Date::checkDay(int testDay) const  // check if day is proper for month and year 111
{
    std::array<int, 13> daysPerMonth{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (testDay > 0 && testDay <= daysPerMonth[month])
        return testDay;

    if (2 == month && 29 == testDay && isLeapYear())
        return testDay;

    throw std::invalid_argument("Invalid day for current month and year");
}
int Date::daysInMonth(int Month) const  // returns number of days in given month 111
{
    int x = (isLeapYear()) ? 29 : 28;
    int mday[]{ 0, 31, x, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    return mday[Month];
}
bool Date::isLeapYear() const  // indicates whether date is in a leap year 111
{
    return ((0 == year % 4 && 0 != year % 100) || (0 == year % 400)) ? true : false;
}
int Date::convertDDToDDD() const  // get 3-digit day based on month and day 111
{
    int total = 0;
    for (int i = 1; i < month; i++) {
        total += daysInMonth(i);
    }

    return total + day;
}
void Date::setMMDDFromDDD(int Day)  // set month and day based on 3-digit day ****
{
    int total = 0, count = 1;

    do {
        total += daysInMonth(count);
        count++;
    } while (Day > total);

    setMonth(count - 1);
    setDay(daysInMonth(month) + Day - total);
}
string Date::convertMMToMonth(int mm) const  // convert mm to month name
{
    switch (mm) {
    case 1:
        return "January";
    case 2:
        return "February";
    case 3:
        return "March";
    case 4:
        return "April";
    case 5:
        return "May";
    case 6:
        return "June";
    case 7:
        return "July";
    case 8:
        return "August";
    case 9:
        return "September";
    case 10:
        return "October";
    case 11:
        return "November";
    case 12:
        return "December";
    }
    return "";
}
void Date::setMMFromMonth(string Month)  // convert month name to mm 111
{
    const char* MonthName[]{ "January", "February", "March",     "April",   "May",      "June",
                             "July",    "August",   "September", "October", "November", "December" };
    for (int i = 0; i < 12; i++) {
        if (MonthName[i] == Month)
            setMonth(i + 1);
    }
}
int Date::convertYYYYToYY() const  // get 2-digit year based on 4-digit year 111
{
    return year >= 2000 ? year - 2000 : year - 1900;
}
void Date::setYYYYFromYY(int Year)  // set year based on 2-digit year 111
{
    year = 2000 + Year;
}

string Date::toString() const {
    std::ostringstream output;
    output << convertMMToMonth(month) << ' ' << day << ", " << year;
    return output.str();
}