#include <iostream>

using namespace std;

int main()
{
    const int n = 4;

    // create 2D array
    int **a = new int*[n];
    for (int i = 0; i < n; ++i)
        a[i] = new int[n];

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            a[i][j] = i * n + j;
            cout << a[i][j] << " ";
        }
        cout << endl;
    }

    for (int i = 0; i < n; ++i)
        delete [] a[i];

    delete [] a;

    return 0;
}
