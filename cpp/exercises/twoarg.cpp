#include <iostream>

using namespace std;

void n_chars(char, int);

int main()
{
    int times;
    char ch;
    cout << "Enter a symbol: ";
    cin >> ch;
    while (ch != 'q')
    {
	cout << "Enter a number: ";
	cin >> times;
	n_chars(ch, times);
	cout << "\nEnter another symbol or 'q' to exit: ";
	cin >> ch;
    }
    cout << times << " done!\n";
    return 0;
}

void n_chars(char c, int n)
{
    while (n-- > 0)
	cout << c;
}
