#ifndef XLIB_PLOT_H
#define XLIB_PLOT_H

#include <unistd.h>
#include <stdexcept>
#include <memory>

#include <X11/Xlib.h>

class Xlib_plot {
private:
    Display* main_dpy_;
public:
    // ctor
    Xlib_plot(): main_dpy_ {XOpenDisplay(nullptr)} { }

    // forbid copying
    Xlib_plot(const Xlib_plot&) = delete;
    Xlib_plot& operator=(const Xlib_plot&) = delete;

    // default moving semantics
    Xlib_plot(Xlib_plot&&) = default;
    Xlib_plot& operator=(Xlib_plot&&) = default;

    // dtor
    ~Xlib_plot() { XCloseDisplay(main_dpy_); }
};

#endif /* XLIB_PLOT_H */

