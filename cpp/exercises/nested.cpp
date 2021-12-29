#include <iostream>

const int Cities = 5;
const int Years = 4;

int main()
{
    using namespace std;
    const char * cities[Cities] = // pointers array
    {
	"Gribble City",
	"Gribbletown",
	"New Gribble",
	"San Gribble",
	"Gribble Vista"
    };
    int maxtemps[Years][Cities] = // 2D array
    {
	{95, 99, 86, 100, 104},
	{95, 97, 90, 106, 102},
	{96, 100, 940, 107, 105},
	{97, 102, 89, 108, 104}
    };
    cout << "Max temperatures for years 2003 - 2006: \n";
    for (int city = 0; city < Cities; ++city)
    {
	cout << cities[city] << ":\t";
	for (int year = 0; year < Years; ++year)
	    cout << maxtemps[year][city] << "\t";
	cout << endl;
    }
    return 0;
}
