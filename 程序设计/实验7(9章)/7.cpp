#include <iomanip>
#include <iostream>

using namespace std;

class Time {
public:
    explicit Time() : totalSeconds(0){};
    void         setTime(int h, int m, int s) { totalSeconds = h * 3600 + m * 60 + s; }
    void         setHour(int h) { totalSeconds = (h * 3600) + (totalSeconds % 3600); }
    void         setMinute(int m) { totalSeconds = (totalSeconds / 3600) * 3600 + (m * 60) + (totalSeconds % 60); }
    void         setSecond(int s) { totalSeconds = (totalSeconds / 60) * 60 + s; }
    unsigned int getHour() const { return totalSeconds / 3600; }
    unsigned int getMinute() const { return (totalSeconds % 3600) / 60; }
    unsigned int getSecond() const { return totalSeconds % 60; }
    void         printUniversal() const {
        cout << setfill('0') << setw(2) << getHour() << ":" << setw(2) << getMinute() << ":" << setw(2) << getSecond();
    }
    void printStandard() const {
        cout << ((getHour() == 0 || getHour() == 12) ? 12 : getHour() % 12) << ":" << setfill('0') << setw(2) << getMinute()
             << ":" << setw(2) << getSecond() << (getHour() < 12 ? " AM" : " PM");
    }

private:
    unsigned int totalSeconds;
};

void tick(Time& t) {
    t.setSecond(t.getSecond() + 1);
    if (t.getSecond() == 60) {
        t.setSecond(0);
        t.setMinute(t.getMinute() + 1);
        if (t.getMinute() == 60) {
            t.setMinute(0);
            t.setHour(t.getHour() + 1);
            if (t.getHour() == 24) {
                t.setHour(0);
            }
        }
    }
}

int main() {
    Time t;
    t.setTime(18, 30, 22);
    cout << "Universal time: ";
    t.printUniversal();
    cout << "\nStandard time: ";
    t.printStandard();
    cout << endl;
    tick(t);
    cout << "\nNew standard time: ";
    t.printStandard();
    cout << endl;
    return 0;
}