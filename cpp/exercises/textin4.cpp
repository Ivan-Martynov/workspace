#include <iostream>
#include <stdio.h>

int main(void)
{
    using namespace std;
    int ch, count = 0;
    while ((ch = cin.get()) != EOF) 
    {
	cout.put(char(ch));
	++count;
    }
    cout << endl << count << " symbols \n";
    return 0;
}
