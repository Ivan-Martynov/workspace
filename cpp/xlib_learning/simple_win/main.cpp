#include <iostream>
#include <X11/Xlib.h>
#include <unistd.h>
#include <stdexcept>
#include <memory>
#include <thread>

#include "Xlib_plot.h"

void test_empty_win() {
    // open display
    Display* dpy {XOpenDisplay(nullptr)};
    //auto dpy = std::make_unique<Display>(XOpenDisplay(nullptr));
    if (dpy) {
        // create window
        Window win {XCreateWindow(dpy, DefaultRootWindow(dpy),
                0, 0, 200, 100, 0, CopyFromParent, CopyFromParent,
                CopyFromParent, 0, 0)};
        // show the window
        XMapWindow(dpy, win);
        XFlush(dpy);
        // sleep to demonstrate
        sleep(2);
    }
    XCloseDisplay(dpy);
}

void test_hello_world() {
    Display *dpy {XOpenDisplay(nullptr)};
    if (!dpy)
        throw std::runtime_error {"cannot connect to display"};

    int screen_num {DefaultScreen(dpy)};
    int black_color {BlackPixel(dpy, screen_num)};
    int white_color {WhitePixel(dpy, screen_num)};

    std::cout << XDisplayWidth(dpy, screen_num) << '\n';
    std::cout << XDisplayHeight(dpy, screen_num) << '\n';

    // create window
    Window win {XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, 200, 100,
            0, black_color, black_color)};
    // get MapNotify events
    XSelectInput(dpy, win, StructureNotifyMask);

    // show the window
    XMapWindow(dpy, win);

    // create a "Graphics Context"
    //GC gc = XCreateGC(dpy, win, 0, 0);
    GC gc {DefaultGC(dpy, screen_num)};

    // inform GC that we use white color for drawing
    XSetForeground(dpy, gc, white_color);

    // wait for MapNotify event
    while (true) {
        XEvent e;
        XNextEvent(dpy, &e);
        if (e.type == MapNotify)
            break;
    }

    // draw a line
    XDrawLine(dpy, win, gc, 10, 60, 180, 20);

    // draw text
    const std::string s {"Hello, World!"};
    XDrawString(dpy, win, gc, 1, 20, s.c_str(), s.length());

    // send the request to the server
    XFlush(dpy);

    // sleep to demonstrate
    sleep(2);

    XCloseDisplay(dpy);
}

void test_button_event() {
    Display *dpy {XOpenDisplay(nullptr)};
    if (!dpy)
        throw std::runtime_error {"cannot connect to display"};


    int screen_num {DefaultScreen(dpy)};
    int black_color {BlackPixel(dpy, screen_num)};
    int white_color {WhitePixel(dpy, screen_num)};
    //int win_w {XDisplayWidth(dpy, screen_num)};
    //int win_h {XDisplayHeight(dpy, screen_num)};
    int win_w {800};
    int win_h {600};

    // create window
    Window win {XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0,
            win_w, win_h, 0, black_color, black_color)};

    // show the window
    XMapWindow(dpy, win);

    // create a "Graphics Context" and inform GC we use white color for drawing
    //XGCValues vals;
    //vals.foreground = white_color;
    //vals.line_width = 1;
    //vals.line_style = LineSolid;
    //GC gc {XCreateGC(dpy, win, GCForeground | GCLineWidth | GCLineStyle, &vals)};
    GC gc {DefaultGC(dpy, screen_num)};
    XSetForeground(dpy, gc, white_color);

    // get MapNotify events
    XSelectInput(dpy, win, ButtonPressMask | StructureNotifyMask |
            ExposureMask);

    while (true) {
        XEvent e;
        XNextEvent(dpy, &e);
        switch (e.type) {
            case Expose:
                XDrawLine(dpy, win, gc, 0, 0, win_w, win_h);
                XDrawPoint(dpy, win, gc, 100, 100);
                break;
            case ConfigureNotify:
                if (win_w != e.xconfigure.width ||
                        win_h != e.xconfigure.height) {
                    win_w = e.xconfigure.width;
                    win_h = e.xconfigure.height;
                    XClearWindow(dpy, e.xany.window);
                    std::cout << "new size: " << win_w << ' ' << win_h << '\n';
                }
                break;
            case ButtonPress:
                XCloseDisplay(dpy);
                return;
        }
    }
}

int main()
try {

    auto t1 = std::thread{test_empty_win};
    auto t2 = std::thread{test_hello_world};
    auto t3 = std::thread{test_button_event};
    t1.join();
    t2.join();
    t3.join();
    //test_empty_win();
    //test_hello_world();
    //test_button_event();

    return 0;
} catch (const std::runtime_error& err) {
    std::cerr << err.what() << '\n';
    return 1;
}  catch (const std::exception& err) {
    std::cerr << err.what() << '\n';
    return 2;
}  catch (...) {
    std::cerr << "Unknown exception\n";
    return 3;
}


