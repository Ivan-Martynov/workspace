#include <iostream>

class I
{
  public:
    virtual ~I() {}
    virtual void do_it() = 0;
};

class A : public I
{
  public:
    ~A()
    {
        std::cout << "A dtor" << '\n';
    }

    void do_it()
    {
        std::cout << 'A';
    }
};

class D : public I
{
  private:
    I* m_wrappee_ptr;

  public:
    D(I* inner_ptr) : m_wrappee_ptr {inner_ptr} {}

    D(const D&) = default;
    D& operator=(const D&) = default;

    ~D()
    {
        delete m_wrappee_ptr;
    }

    void do_it()
    {
        m_wrappee_ptr->do_it();
    }
};

class X : public D
{
  public:
    X(I* core) : D(core) {}
    ~X()
    {
        std::cout << "X dtor"
                  << "   ";
    }
    /*virtual*/
    void do_it()
    {
        D::do_it();
        std::cout << 'X';
    }
};

class Y : public D
{
  public:
    Y(I* core) : D(core) {}
    ~Y()
    {
        std::cout << "Y dtor"
                  << "   ";
    }
    /*virtual*/
    void do_it()
    {
        D::do_it();
        std::cout << 'Y';
    }
};

class Z : public D
{
  public:
    Z(I* core) : D(core) {}

    ~Z()
    {
        std::cout << "Z dtor"
                  << "   ";
    }

    void do_it()
    {
        D::do_it();
        std::cout << 'Z';
    }
};

int main()
{
    I* anX = new X(new A);
    I* anXY = new Y(new X(new A));
    I* anXYZ = new Z(new Y(new X(new A)));

    anX->do_it();
    std::cout << '\n';

    anXY->do_it();
    std::cout << '\n';

    anXYZ->do_it();
    std::cout << '\n';

    delete anX;
    delete anXY;
    delete anXYZ;
}
