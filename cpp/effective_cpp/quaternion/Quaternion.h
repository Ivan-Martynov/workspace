#ifndef QUATERNION_H
#define QUATERNION_H

#include <cmath>

class Quaternion
{
  private:
    double x_, y_, z_, w_;

  public:
    constexpr Quaternion() noexcept : x_ {0}, y_ {0}, z_ {0}, w_ {1} {}

    constexpr explicit Quaternion(
        const double x, const double y, const double z, const double w) noexcept
        : x_ {x}, y_ {y}, z_ {z}, w_ {w}
    {
    }

    constexpr Quaternion(const Quaternion& other) noexcept
        : x_ {other.x()}, y_ {other.y()}, z_ {other.z()}, w_ {other.w()}
    {
    }

    Quaternion(Quaternion&&) = default;

    Quaternion& operator=(const Quaternion&) = default;
    Quaternion& operator=(Quaternion&&) = default;
    ~Quaternion() = default;

    constexpr double x() const noexcept
    {
        return x_;
    }
    constexpr double y() const noexcept
    {
        return y_;
    }
    constexpr double z() const noexcept
    {
        return z_;
    }
    constexpr double w() const noexcept
    {
        return w_;
    }

    constexpr Quaternion& operator+=(const Quaternion& rhs) noexcept
    {
        x_ += rhs.x();
        y_ += rhs.y();
        z_ += rhs.z();
        w_ += rhs.w();
        return *this;
    }

    constexpr Quaternion& operator-=(const Quaternion& rhs) noexcept
    {
        x_ -= rhs.x();
        y_ -= rhs.y();
        z_ -= rhs.z();
        w_ -= rhs.w();
        return *this;
    }

    constexpr Quaternion& operator*=(const double rhs) noexcept
    {
        x_ *= rhs;
        y_ *= rhs;
        z_ *= rhs;
        w_ *= rhs;
        return *this;
    }

    constexpr Quaternion& operator/=(const double rhs)
    {
        x_ /= rhs;
        y_ /= rhs;
        z_ /= rhs;
        w_ /= rhs;
        return *this;
    }

    friend constexpr Quaternion operator/(
        const Quaternion& lhs, const double rhs)
    {
        return Quaternion {lhs} /= rhs;
    }

    constexpr Quaternion& operator*=(const Quaternion& rhs) noexcept
    {
        x_ = w_ * rhs.x() + rhs.w() * x_ + y_ * rhs.z() - z_ * rhs.y();
        y_ = w_ * rhs.y() + rhs.w() * y_ + z_ * rhs.x() - x_ * rhs.z();
        z_ = w_ * rhs.z() + rhs.w() * z_ + x_ * rhs.y() - x_ * rhs.y();
        w_ = w_ * rhs.w() - x_ * rhs.x() - y_ * rhs.y() - z_ * rhs.z();
        return *this;
    }

    constexpr double squared_norm() const noexcept
    {
        return x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_;
    }

    constexpr double norm() const noexcept
    {
        return std::sqrt(squared_norm());
    }

    constexpr Quaternion conjugate() const noexcept
    {
        return Quaternion(x_, y_, z_, -w_);
    }

    constexpr Quaternion inverse() const
    {
        return conjugate() / squared_norm();
    }

    constexpr double dot(const Quaternion& rhs) const noexcept
    {
        return x_ * rhs.x() + y_ * rhs.y() + z_ * rhs.z() + w_ * rhs.w();
    }
};

std::ostream& operator<<(std::ostream& os, const Quaternion& q)
{
    return os << '(' << q.x() << ", " << q.y() << ", " << q.z() << ", " << q.w()
              << ')';
}

#endif /* QUATERNION_H */
