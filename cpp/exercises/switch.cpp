#include <iostream>
using namespace std;
void showmenu();
void report();
void comfort();

int main()
{
    showmenu();
    int choice;
    cin >> choice;
    while (choice != 5)
    {
	switch (choice)
	{
	    case 1 : cout << "\a\n";
		     break;
	    case 2 : report();
		     break;
	    case 3 : cout << "The boss was in all day!\n";
		     break;
	    case 4 : comfort();
		     break;
	    default : cout << "That's not a choice!\n";
	}
	showmenu();
	cin >> choice;
    }
    cout << "Bye!\n";
    return 0;
}

void showmenu()
{
    cout << "Please enter 1, 2, 3, 4 or 5:\n"
	"1) alarm 2) report\n"
	"3) alibi 4) comfort\n"
	"5) quit\n";
}

void report()
{
    cout << "It's been a great week!\n";
}

void comfort()
{
    cout << "You are the best and can relax ;)\n";
}

