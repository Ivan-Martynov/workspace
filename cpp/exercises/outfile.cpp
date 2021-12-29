#include <iostream>
#include <fstream>

int main()
{
    using namespace std;
    char automobile[50];
    int year;
    double a_price, d_price;
    ofstream outFile;
    outFile.open("carinfo.txt");
    cout << "Enter the manufacturer and model of a car: ";
    cin.getline(automobile, 50);
    cout << "Enter the car year: ";
    cin >> year;
    cout << "Enter the offered price: ";
    cin >> a_price;
    d_price = 0.913 * a_price;
    cout << fixed;
    cout.precision(2);
    cout.setf(ios_base::showpoint);
    cout << "Manufacturer and model: " << automobile << endl;
    cout << "Year: " << year << endl;
    cout << "Starting price $" << a_price << endl;
    cout << "Final price $" << d_price << endl;
    /* the same using outFile instead of cout */
    outFile << fixed;
    outFile.precision(2);
    outFile.setf(ios_base::showpoint);
    outFile << "Manufacturer and model: " << automobile << endl;
    outFile << "Year: " << year << endl;
    outFile << "Starting price $" << a_price << endl;
    outFile << "Final price $" << d_price << endl;
    outFile.close();
    return 0;
}
