#include <iostream>
#include <cmath>

using namespace std;

class Real
{
    public:
        Real(double a = 0) : _x(a) {
            cout << "Real constructor called" << endl;
        }

        virtual ~Real() = default;

        virtual Real operator + (Real &other) {
            return Real(_x + other._x);
        }

        virtual Real operator - (Real &other) {
            return Real(_x - other._x);
        }

        virtual Real operator * (Real &other) {
            return Real(_x * other._x);
        }

        virtual Real operator / (Real &other) {
            return Real(_x / other._x);
        }

        friend ostream &operator << (ostream &output, Real const & that)
        {
            return output << that._x;
        }

        void showinfo()
        {
            cout << "Real value: " << _x << endl;
        }

    protected:

        double _x;
};

class Integer : public Real {
    public:
      Integer(int a = 0) :Real(int(round(a))) {};

    protected:
        //int _x;
};

int main(void)
{
    Real r(3.2);
    Real q(1.3);
    r = r + q;

    r.showinfo();

    Integer i(3), j(7);

    cout << i + j << endl;
    cout << i - j << endl;
    cout << i * r << endl;
    cout << q / j << endl;
    cout << i / q << endl;

    return 0;
}
