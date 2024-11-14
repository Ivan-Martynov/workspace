#include <iostream>
#include <vector>
#include <memory>

class Point
{
  private:
    int m_x {};
    int m_y {};

  public:
    Point(int x, int y) : m_x {x}, m_y {y} {}

    friend std::ostream& operator<<(std::ostream& out, const Point& p)
    {
        return out << "Point(" << p.m_x << ", " << p.m_y << ')';
    }
};

class Shape
{
  public:
    virtual std::ostream& print(std::ostream& stream) const = 0;

    friend std::ostream& operator<<(std::ostream& stream, const Shape& shape)
    {
        return shape.print(stream);
    }

    virtual ~Shape() = default;
};

class Triangle : public Shape
{
  public:
    Triangle(Point first, Point second, Point third)
        : m_first {first}, m_second {second}, m_third {third}
    {
    }

    std::ostream& print(std::ostream& stream) const override
    {
        stream << "Trianle(" << m_first << ", " << m_second << ", " << m_third
               << ")";
        return stream;
    }

  private:
    Point m_first;
    Point m_second;
    Point m_third;
};

class Circle : public Shape
{
  public:
    Circle(Point center, int radius) : m_center {center}, m_radius {radius} {}

    std::ostream& print(std::ostream& stream) const override
    {
        stream << "Circle(" << m_center << ", raidus " << m_radius << ")";
        return stream;
    }

    int radius() const
    {
        return m_radius;
    }

  private:
    Point m_center;
    int m_radius;
};

int getLargestRadius(const std::vector<Shape*>& shapes)
{
    int radius {0};

    for (const auto* shape_ptr : shapes)
    {
        if (const auto* circle_ptr {dynamic_cast<const Circle*>(shape_ptr)};
            circle_ptr && (circle_ptr->radius() > radius))
        {
            radius = circle_ptr->radius();
        }
    }

    return radius;
}

int getLargestRadius(const std::vector<std::unique_ptr<Shape>>& shapes)
{
    int radius {0};

    for (const auto& shape_ptr : shapes)
    {
        if (const auto* circle_ptr {
                dynamic_cast<const Circle*>(shape_ptr.get())};
            circle_ptr && (circle_ptr->radius() > radius))
        {
            radius = circle_ptr->radius();
        }
    }

    return radius;
}

int main()
{
    Circle c {Point {1, 2}, 7};
    std::cout << c << '\n';

    Triangle t {Point {1, 2}, Point {3, 4}, Point {5, 6}};
    std::cout << t << '\n';

    std::vector<Shape*> v {new Circle {Point {1, 2}, 7},
        new Triangle {Point {1, 2}, Point {3, 4}, Point {5, 6}},
        new Circle {Point {7, 8}, 3}};

    // print each shape in vector v on its own line here
    for (const auto* shape_ptr : v)
    {
        std::cout << *shape_ptr << "\n";
    }

    std::cout << "The largest radius is: " << getLargestRadius(v)
              << '\n'; // write this function

    // delete each element in the vector here
    for (auto* shape_ptr : v)
    {
        delete shape_ptr;
    }

    std::vector<std::unique_ptr<Shape>> shapes {};
    shapes.emplace_back(new Circle(Point {1, 2}, 7));
    shapes.emplace_back(new Triangle(Point {1, 2}, Point {3, 4}, Point {5, 6}));
    shapes.emplace_back(new Circle(Point {7, 8}, 13));

    for (const auto& shape_ptr : shapes)
    {
        std::cout << *shape_ptr << "\n";
    }

    std::cout << "The largest radius is: " << getLargestRadius(shapes)
              << '\n'; // write this function

    return 0;
}
