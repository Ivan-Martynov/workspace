#include <iostream>
#include <stdexcept>
#include <cmath>

using namespace std;

struct DivideByZero: public runtime_error
{
    DivideByZero(void) : runtime_error("DivideByZero") { }
};

struct OverflowError : public runtime_error
{
    OverflowError(void) : runtime_error("OverflowError") { }
};

struct UnderflowError : public runtime_error
{
    UnderflowError(void) : runtime_error("UnderflowError") { }
};

class Integer
{
    public:

        Integer(int value = 0) : _value(value) { }

        virtual Integer operator + (Integer &other)
        {
            unsigned char result = _value + other._value;

            if (result < _value)
                throw new OverflowError;

            return Integer(result);
        }

        virtual Integer operator - (Integer &other)
        {
            unsigned char result = _value - other._value;

            if (result > _value)
                throw new UnderflowError;

            return Integer(result);
        }

        virtual Integer operator * (Integer &other)
        {
            unsigned char result = _value * other._value;

            if ((_value > 1) && (other._value > 1) && (result < _value))
                throw new OverflowError;

            return Integer(result);
        }

        virtual Integer operator / (Integer &other)
        {
            if (other._value == 0)
                throw new DivideByZero;

            return Integer(_value / other._value);
        }

        friend ostream & operator << (ostream & output, Integer const & that)
        {
            return output << that._value;
        }

    protected:

        unsigned char _value;
};

int main(void)
{
    Integer a(129), b(128), zero(0);

    // Integer c(a + b); // overflow
    // Integer c(b - a); // underflow
    Integer c(a / zero); // divide by zero

    return 0;
}
