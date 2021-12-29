#include <iostream>
#include <sstream>

using namespace std;

int my_atoi(const char * s)
{
    istringstream istream;
    ostringstream os;

    istream.str(s);
    int i;
    istream >> i;

    return i;
}

int main()
{
    int i = my_atoi("42");

    cout << i << endl;

    return 0;
}
