#ifndef LOG_DISC_H
#define LOG_DISC_H

#include "Polygon.h"

namespace log_measurement {

/*
 * Log disc class represents a polygon at specified height value (z coordinate)
 */
class Log_disc : public Polygon<double> {
public:
    using Polygon2D = Polygon<double>;
    using Point = Point2D<double>;

    Log_disc() noexcept: Polygon2D{}, h_{0.0} { };
    explicit Log_disc(const double h): Polygon2D{}, h_{h} { };
    explicit Log_disc(const Polygon2D& pgn, const double h = 0):
        Polygon2D{pgn}, h_{h} { };
    explicit Log_disc(Polygon2D&& pgn, const double h = 0):
        Polygon2D{pgn}, h_{h} { };
    explicit Log_disc(const std::vector<Point>& p, const double h = 0):
        Polygon2D{p}, h_{h} { };
    explicit Log_disc(std::vector<Point>&& p, const double h = 0):
        Polygon2D{p}, h_{h} { };

    Log_disc(const Log_disc& o): Polygon2D{o}, h_{o.h_} { }
    Log_disc& operator=(const Log_disc& o) {
        if (this != &o) { Polygon2D::operator=(o); h_ = o.h_; } return *this;
    };
    Log_disc(Log_disc&& o): Polygon2D{o}, h_{std::move(o.h_)} { }
    Log_disc& operator=(Log_disc&& o) {
        Polygon2D::operator=(o); h_ = std::move(o.h_); return *this;
    }
    ~Log_disc() = default;

    void swap(Log_disc& o) noexcept { Polygon2D::swap(o); std::swap(h_, o.h_); }

    double height() const noexcept { return h_; }

private:
    double h_; // height of the log disc
};

// swap non-member function
void swap(Log_disc& lhs, Log_disc& rhs) noexcept { lhs.swap(rhs); }

} // log_measurement namespace

#endif /* LOG_DISC_H */

