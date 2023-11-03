// Date.h
#ifndef DATE_H
#define DATE_H
#include <string>
using std::string;
class Date {
public:
    Date();                           // default constructor uses <ctime> functions to set date
    Date(int, int);                   // constructor using ddd yyyy format
    Date(int, int, int);              // constructor using dd/mm/yy format
    Date(string, int, int);           // constructor using Month dd, yyyy format
    void   setDay(int);               // set the day
    void   setMonth(int);             // set the month
    void   print() const;             // print date in month/day/year format
    void   printDDDYYYY() const;      // print date in ddd yyyy format
    void   printMMDDYY() const;       // print date in mm/dd/yy format
    void   printMonthDDYYYY() const;  // print date in Month dd, yyyy format
    bool   isSameMonth(const Date&) const;
    string convertMMToMonth(int) const;  // convert mm to month name
    ~Date();                             // provided to confirm destruction order
    string toString() const;

private:
    int month;  // 1-12 (January-December)
    int day;    // 1-31 based on month
    int year;   // any year
    // utility functions
    int  checkDay(int) const;     // check if day is proper for month and year
    int  daysInMonth(int) const;  // returns number of days in given month
    bool isLeapYear() const;      // indicates whether date is in a leap year
    int  convertDDToDDD() const;  // get 3-digit day based on month and day
    void setMMDDFromDDD(int);     // set month and day based on 3-digit day

    void setMMFromMonth(string);   // convert month name to mm
    int  convertYYYYToYY() const;  // get 2-digit year based on 4-digit year
    void setYYYYFromYY(int);       // set year based on 2-digit year
};                                 // end class Date

#endif