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
        cout << word[i];
    }
    cout << "\ndone!\n";
    cout << x << endl;
    return 0;
}
