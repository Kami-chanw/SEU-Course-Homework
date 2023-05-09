// Sequential reading and writing
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

int main(void) {
    ofstream outFile("./ascii.txt", ios::out | ios::app);
    if (!outFile) {
        cerr << "open failed";
    }
    outFile << setw(7) << "Decimal" << setw(9) << "Octal " << setw(15) << "Hexadecimal " << setw(13) << "Character" << showbase
            << '\n';

    for (int i = 33; i <= 126; i++)
        outFile << right << setw(7) << dec << i << setw(9) << oct << i << setw(15) << hex << i << setw(13) << static_cast<char>(i)
                << showbase << '\n';

    outFile.flush();
    outFile.close();

    ifstream inFile("./ascii.txt", ios::in);
    if (!inFile) {
        cerr << "open failed";
    }
    string s;
    while (getline(inFile, s)) {
        cout << s << endl;
    }
    inFile.close();

    return 0;
}