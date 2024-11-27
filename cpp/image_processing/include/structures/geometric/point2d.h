#ifndef H_INCLUDE_STRUCTURES_GEOMETRIC_POINT2D_H_H
#define H_INCLUDE_STRUCTURES_GEOMETRIC_POINT2D_H_H

namespace Marvin
{

namespace Geometric
{

class Point2d
{
  public:

    /***************************************************************************
     * Construction section.                                                   *
     **************************************************************************/

    explicit constexpr Point2d(double x, double y) : m_x {x}, m_y {y} {}
    explicit constexpr Point2d() : Point2d(0.0, 0.0) {}

    /***************************************************************************
     * End of Construction section.                                            *
     **************************************************************************/

  private:
    double m_x {};
    double m_y {};
};

    
} // namespace Geometric


} // namespace Marvin

#endif // H_INCLUDE_STRUCTURES_GEOMETRIC_POINT2D_H_H
