#include <iomanip>
#include <iostream>
using namespace std;
class Time {
public:
    friend void tick(Time& t);
    explicit Time() : hour(0), minute(0), second(0){};

    void setTime(int, int, int);
    void setHour(int);
    void setMinute(int);
    void setSecond(int);

    unsigned int getHour() const;
    unsigned int getMinute() const;
    unsigned int getSecond() const;
    void         printUniversal() const;
    void         printStandard() const;

private:
    unsigned int hour;
    unsigned int minute;
    unsigned int second;
};

void Time::setTime(int a, int b, int c) {
    hour   = a;
    minute = b;
    second = c;
}
void Time::printStandard() const {
    cout << ((hour == 0 || hour == 12) ? 12 : hour % 12) << ':' << setfill('0') << setw(2) << minute << ':' << setw(2) << second
         << (hour < 12 ? " AM" : " PM");
}

void tick(Time& t) {
    Time time(t);
    int  cur1 = 0, cur2 = 0;
    for (int i = 0; i < 15; ++i) {
        time.printStandard();
        cout << endl;
        time.second++;
        if (time.second == 60)
            time.minute++, time.second = 0;
        if (time.minute == 60)
            time.hour++, time.minute = 0;
        time.hour %= 24;
    }
}

int main() {
    Time t;
    t.setTime(23, 59, 57);
    tick(t);

    return 0;
}
