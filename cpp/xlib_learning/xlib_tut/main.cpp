#include <iostream>
#include <X11/Xlib.h>
#include <unistd.h>
#include <stdexcept>
#include <memory>
#include <exception>
#include <thread>

Window setup_display(Display *dpy, int width, int height) {
    if (!dpy)
        throw std::runtime_error {"cannot connect to display"};

    int screen_num {DefaultScreen(dpy)};
    int black_color {BlackPixel(dpy, screen_num)};
    int white_color {WhitePixel(dpy, screen_num)};

    // create window
    Window win {XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
            0, 0, width, height, 0, black_color, black_color)};

    // get MapNotify events
    XSelectInput(dpy, win, ButtonPressMask | StructureNotifyMask | ExposureMask);

    // show the window
    XMapWindow(dpy, win);

    // create a "Graphics Context"
    //GC gc = XCreateGC(dpy, win, 0, 0);
    GC gc {DefaultGC(dpy, screen_num)};

    // inform GC that we use white color for drawing
    XSetForeground(dpy, gc, white_color);

    return win;
}

void beginning(Display *dpy) {
    int width = DisplayWidth(dpy, DefaultScreen(dpy));
    int height = DisplayHeight(dpy, DefaultScreen(dpy));

    // wait for MapNotify event
    while (true) {
        XEvent ev;
        XNextEvent(dpy, &ev);
        switch (ev.type) {
            case ConfigureNotify:
                if (width != ev.xconfigure.width ||
                        height != ev.xconfigure.height) {
                    width = ev.xconfigure.width;
                    height = ev.xconfigure.height;
                    XClearWindow(dpy, ev.xany.window);
                    std::cout << "new size: " << width  << ' ' << height << '\n';
                }
                break;
            case ButtonPress:
                XCloseDisplay(dpy);
                return;
        }
    }

    //// draw a line
    //XDrawLine(dpy, win, gc, 10, 60, 180, 20);

    //// draw text
    //const std::string s {"Hello, World!"};
    //XDrawString(dpy, win, gc, 1, 20, s.c_str(), s.length());

    //// send the request to the server
    //XFlush(dpy);

    //// sleep to demonstrate
    //sleep(2);

    //XCloseDisplay(dpy);
}

void lines(Display *dpy, Window win) {
    int screen_num {DefaultScreen(dpy)};

    // create a "Graphics Context" and inform GC we use white color for drawing
    XGCValues vals;
    vals.foreground = WhitePixel(dpy, screen_num);
    vals.line_width = 1;
    vals.line_style = LineSolid;

    GC pen {XCreateGC(dpy, win, GCForeground | GCLineWidth | GCLineStyle,
            &vals)};

    int width = DisplayWidth(dpy, screen_num);
    int height = DisplayHeight(dpy, screen_num);

    // wait for MapNotify event
    while (true) {
        XEvent ev;
        XNextEvent(dpy, &ev);
        switch (ev.type) {
            case Expose:
                XDrawLine(dpy, win, pen, 0, 0, width, height);
                XDrawLine(dpy, win, pen, width, 0, 0, height);
                break;
            case ConfigureNotify:
                if (width != ev.xconfigure.width ||
                        height != ev.xconfigure.height) {
                    width = ev.xconfigure.width;
                    height = ev.xconfigure.height;
                    XClearWindow(dpy, ev.xany.window);
                    std::cout << "new size: " << width  << ' ' << height << '\n';
                }
                break;
            case ButtonPress:
                XCloseDisplay(dpy);
                return;
        }
    }
}

void colours(Display *dpy, int width, int height) {
    if (!dpy)
        throw std::runtime_error {"cannot connect to display"};

    int screen_num {DefaultScreen(dpy)};

    // create a "Graphics Context" and inform GC we use white color for drawing
    XGCValues vals;
    vals.foreground = WhitePixel(dpy, screen_num);
    vals.line_width = 1;
    vals.line_style = LineSolid;

    Colormap colorMap {DefaultColormap(dpy, screen_num)};
    XColor color1, color2;

    XAllocNamedColor(dpy, colorMap, "DarkGreen", &color1, &color2);
    int background = color1.pixel;
    XAllocNamedColor(dpy, colorMap, "LightGreen", &color1, &color2);
    int border = color1.pixel;
    XAllocNamedColor(dpy, colorMap, "Red", &color1, &color2);
    vals.foreground = color1.pixel;

    // create window
    Window win {XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
            0, 0, width, height, 0, border, background)};

    // get MapNotify events
    XSelectInput(dpy, win, ButtonPressMask | StructureNotifyMask | ExposureMask);

    // show the window
    XMapWindow(dpy, win);

    GC pen {XCreateGC(dpy, win, GCForeground | GCLineWidth | GCLineStyle,
            &vals)};

    // wait for MapNotify event
    while (true) {
        XEvent ev;
        XNextEvent(dpy, &ev);
        switch (ev.type) {
            case Expose:
                XDrawLine(dpy, win, pen, 0, 0, width, height);
                XDrawLine(dpy, win, pen, width, 0, 0, height);
                break;
            case ConfigureNotify:
                if (width != ev.xconfigure.width ||
                        height != ev.xconfigure.height) {
                    width = ev.xconfigure.width;
                    height = ev.xconfigure.height;
                    XClearWindow(dpy, ev.xany.window);
                    std::cout << "new size: " << width  << ' ' << height << '\n';
                }
                break;
            case ButtonPress:
                XCloseDisplay(dpy);
                return;
        }
    }
}

void text(Display *dpy, int width, int height) {
    if (!dpy)
        throw std::runtime_error {"cannot connect to display"};

    int screen_num {DefaultScreen(dpy)};

    // create a "Graphics Context" and inform GC we use white color for drawing
    XGCValues vals;
    vals.foreground = WhitePixel(dpy, screen_num);
    vals.line_width = 1;
    vals.line_style = LineSolid;

    Colormap colorMap {DefaultColormap(dpy, screen_num)};
    XColor color1, color2;

    XAllocNamedColor(dpy, colorMap, "DarkGreen", &color1, &color2);
    int background = color1.pixel;
    XAllocNamedColor(dpy, colorMap, "LightGreen", &color1, &color2);
    int border = color1.pixel;
    XAllocNamedColor(dpy, colorMap, "Red", &color1, &color2);
    vals.foreground = color1.pixel;

    const std::string fontname {"-*-helvetica-*-10-*"};
    //XFontStruct *font {XLoadQueryFont(dpy, fontname.c_str())};
    //auto font = std::make_unique<XFontStruct>(dpy, fontname.c_str());

    auto font = std::make_unique<XFontStruct>(XLoadQueryFont(dpy, "fixed"));
    if (font == nullptr) {
        std::cerr << "Unable to load preferred font: " << fontname <<
            ", using fixed\n";
        //font.get() = XLoadQueryFont(dpy, "fixed");
        //font = std::make_unique<XFontStruct>(dpy, "fixed");
    }
    vals.font = font->fid;


    // create window
    Window win {XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
            0, 0, width, height, 0, border, background)};

    // get MapNotify events
    XSelectInput(dpy, win, ButtonPressMask | StructureNotifyMask | ExposureMask);

    // show the window
    XMapWindow(dpy, win);

    GC pen {XCreateGC(dpy, win,
            GCForeground | GCLineWidth | GCLineStyle | GCFont, &vals)};

    std::string text {"Hi!"};
    int text_width {XTextWidth(font.get(), text.c_str(), text.length())};

    // wait for MapNotify event
    while (true) {
        XEvent ev;
        XNextEvent(dpy, &ev);
        switch (ev.type) {
            case Expose:
                XDrawLine(dpy, win, pen, 0, 0, width, height);
                XDrawLine(dpy, win, pen, width, 0, 0, height);
                XDrawString(dpy, ev.xany.window, pen, (width - text_width) >> 1,
                        (height + font->ascent) >> 1, text.c_str(),
                        text.length());
                break;
            case ConfigureNotify:
                if (width != ev.xconfigure.width ||
                        height != ev.xconfigure.height) {
                    width = ev.xconfigure.width;
                    height = ev.xconfigure.height;
                    XClearWindow(dpy, ev.xany.window);
                    std::cout << "new size: " << width  << ' ' << height << '\n';
                }
                break;
            case ButtonPress:
                XCloseDisplay(dpy);
                return;
        }
    }
}
int main() try {

    Display *dpy {XOpenDisplay(nullptr)};

    int width = 600;
    int height = 400;

    //setup_display(dpy, width, height);
    //beginning(dpy);

    //Window main_window = setup_display(dpy, width, height);
    //lines(dpy, main_window);
    //colours(dpy, width, height);
    text(dpy, width, height);

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


