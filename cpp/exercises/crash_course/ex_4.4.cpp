#include <iostream>

using namespace std;

class Singleton
{
    public:

        static Singleton * instance() {
            if (!_instance)
                _instance = new Singleton;

            return _instance;
        }

    private:

        Singleton(void) { }

        Singleton(Singleton const & other) = delete;

        Singleton & operator = (Singleton const & other) = delete;

        static Singleton * _instance;
};

Singleton *Singleton::_instance = 0;

int main(void)
{
    Singleton *singleton = Singleton::instance();

    delete singleton;

    return 0;
}
