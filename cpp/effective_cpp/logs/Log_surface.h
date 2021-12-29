#ifndef LOG_SURFACE_H
#define LOG_SURFACE_H

#include "Log_disc.h"

namespace log_measurement {

/*
 * Log surface class is a collection (vector) of log discs
 */
class Log_surface {
public:
    using value_type       = Log_disc;
    using size_type        = typename std::vector<value_type>::size_type;
    using iterator         = typename std::vector<value_type>::iterator;
    using reverse_iterator = typename std::vector<value_type>::reverse_iterator;
    using const_iterator   = typename std::vector<value_type>::const_iterator;
    using const_reverse_iterator =
        typename std::vector<value_type>::const_reverse_iterator;

    Log_surface() = default;
    explicit Log_surface(const std::vector<Log_disc>& v): discs_{v} { }
    explicit Log_surface(std::vector<Log_disc>&& v): discs_{std::move(v)} { }

    // read data from a file
    explicit Log_surface(const std::string&);

    Log_surface(const Log_surface&) = default;
    Log_surface& operator=(const Log_surface&) = default;
    Log_surface(Log_surface&&) = default;
    Log_surface& operator=(Log_surface&&) = default;
    ~Log_surface() = default;

    /*
     * ------------ Iterators and access ------------
     */
    // begin iterators
    iterator begin() noexcept { return discs_.begin(); }
    const_iterator begin() const noexcept { return discs_.begin(); }
    const_iterator cbegin() const noexcept { return discs_.cbegin(); }

    // end iterators
    iterator end() noexcept { return discs_.end(); }
    const_iterator end() const noexcept { return discs_.end(); }
    const_iterator cend() const noexcept { return discs_.cend(); }

    // reverse begin iterators
    reverse_iterator rbegin() noexcept { return discs_.rbegin(); }
    const_reverse_iterator rbegin() const noexcept { return discs_.rbegin(); }
    const_reverse_iterator crbegin() const noexcept { return discs_.crbegin(); }

    // reverse end iterators
    reverse_iterator rend() noexcept { return discs_.rend(); }
    const_reverse_iterator rend() const noexcept { return discs_.rend(); }
    const_reverse_iterator crend() const noexcept { return discs_.crend(); }

    // provide access to the first and last points
    value_type& front() { return discs_.front(); }
    const value_type& front() const { return discs_.front(); }
    value_type& back() { return discs_.back(); }
    const value_type& back() const { return discs_.back(); }

    // index access
    value_type& operator[](const size_t i) { return discs_[i]; }
    const value_type& operator[](const size_t i) const { return discs_[i]; }
    value_type& at(const size_t i) { return discs_.at(i); }
    const value_type& at(const size_t i) const { return discs_.at(i); }
    /*
     * ------------ End Iterators and access ------------
     */

    // checking if the polygon is empty
    bool is_empty() const noexcept { return discs_.empty(); }

    // number of points
    size_type size() const noexcept { return discs_.size(); }

    // clear points
    void clear() noexcept { discs_.clear(); }

    // add element to the end
    void push_back(const value_type& p) { discs_.push_back(p); }
    void push_back(value_type&& p) { discs_.push_back(p); }

    // emplace to the end
    template <class... Args>
        void emplace_back(Args&&... args) { discs_.emplace_back(args...); }

    // swapping
    void swap(Log_surface& o) noexcept { discs_.swap(o.discs_); }

    // volume of the log
    double volume() const noexcept;
    // volume of the log when distance between discs is the same
    double volume(const double) const noexcept;

private:
    std::vector<Log_disc> discs_;
};

// load log points from a file
Log_surface::Log_surface(const std::string& file_name): discs_ {} {
    std::ifstream ifs{file_name};
    if (!ifs)
        throw std::runtime_error {"cannot open file " + file_name};
    ifs.exceptions(ifs.exceptions() | std::ios_base::badbit);

    size_t curr_disc_idx{};
    Point2D<double> p{};
    double h {};
    if (!(ifs >> curr_disc_idx >> p >> h))
        throw std::runtime_error {"wrong format for the first line"};

    Log_disc curr_disc{h};
    curr_disc.push_back(p);
    for (size_t disc_idx; ifs >> disc_idx >> p >> h; ) {
        if (ifs.fail()) {
            ifs.unget();
            ifs.clear(std::ios_base::failbit);
        }
        // found next disc
        if (disc_idx != curr_disc_idx) {
            discs_.push_back(curr_disc);
            curr_disc = Log_disc{h};
            curr_disc.push_back(p);
            curr_disc_idx = disc_idx;
        } else { // keep adding points to the current disc
            curr_disc.push_back(p);
        }
    }
    discs_.push_back(curr_disc);
}

// swap non-member function
void swap(Log_surface& lhs, Log_surface& rhs) noexcept { lhs.swap(rhs); }

// volume of the log: using average sum of two neighbouring discs areas
double Log_surface::volume() const noexcept {
    auto num_discs = size();

    // log should have at least two discs
    if (num_discs < 2)
        return 0;

    double v {0.0};
    auto prev_area   = front().area();
    auto prev_height = front().height();
    for (size_type i {1}; i < num_discs; ++i) {
        const auto next_area   = discs_[i].area();
        const auto next_height = discs_[i].height();

        v += (prev_area + next_area) * 0.5 * (next_height - prev_height);

        prev_area   = next_area;
        prev_height = next_height;
    }
    return v;
}

double Log_surface::volume(const double h) const noexcept {
    auto num_discs = size();

    // log should have at least two discs
    if (num_discs < 2)
        return 0;

    double v {0.0};
    auto prev_area = front().area();
    for (size_type i {1}; i < num_discs; ++i) {
        const auto next_area   = discs_[i].area();
        v += (prev_area + next_area) * 0.5 * h;
        prev_area = next_area;
    }
    return v;
}

std::vector<double> disc_areas(const Log_surface& log) {
    std::vector<double> areas(log.size());
    for (Log_surface::size_type i {0}; i < log.size(); ++i)
        areas[i] = log[i].area();
    return areas;
}

} // log_measurement namespace

#endif /* LOG_SURFACE_H */

