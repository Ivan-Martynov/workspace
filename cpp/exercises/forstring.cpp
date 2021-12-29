#include <iostream>
#include <string>

int main()
{
    using namespace std;
    cout << "Enter a word: ";
    string word;
    cin >> word;
    int x = 10;
    // display charachters in a reverse order
    for (int i = word.size() - 1; i >= 0; --i)
    {
	int x = 100;
	cout << x << endl;
	cout << word[i];
    }
    cout << "\ndone!\n";
    cout << x << endl;
    return 0;
}
