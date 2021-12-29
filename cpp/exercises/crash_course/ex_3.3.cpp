#include <iostream>
#include <cmath>

using namespace std;

class Point
{
    public:

        Point(void) : _x(0.0), _y(0.0) { };

        static Point cartesian(const double x, const double y)
        {
            return Point(x, y);
        }

        static Point polar(const double radius, const double phi)
        {
            return Point(radius * cos(phi), radius * sin(phi));
        }

        void showinfo()
        {
            cout << "x = " << _x << ", y = " << _y << endl;
        }

     private:

        Point(const double x, const double y): _x(x), _y(y) { };

        double _x;
        double _y;
};

int main(void)
{
    Point p1 = Point::cartesian(5.7, 1.2);
    p1.showinfo();
    Point p2 = Point::polar(5.7, 1.4);
    p2.showinfo();
    p1.showinfo();

    return 0;
}
