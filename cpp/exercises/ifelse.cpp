#include <iostream>

int main()
{
    using namespace std;
    char ch;
    cout << "Type and I'll repeat.\n";
    cin.get(ch);
    while (ch != '.')
    {
	if (ch == '\n')
	    cout << ch;
	else
	    cout << ch + 1;
	cin.get(ch);
    }
    cout << endl;
    return 0;
}

