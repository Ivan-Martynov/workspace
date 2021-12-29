#include <iostream>

int main()
{
    using namespace std;
    char ch;
    int count = 0;
    while (cin.get(ch)) 
    {
	cout << ch;
	++count;
    }
    cout << endl << count << " symbols \n";
    return 0;
}
