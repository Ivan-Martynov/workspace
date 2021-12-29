/* testing program
** g++ input.c -o output -lX11 -lXinerama -lxf86vmode
*/

#include <X11/Xlib.h>
#include <X11/extensions/Xinerama.h>
#include <X11/extensions/xf86vmode.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
	Display *dpy = XOpenDisplay(NULL);

	if (!dpy)
	{
		fprintf(stderr,"Cannot open the display\n");
		exit(1);
	}
	int num_scr; // number of screens
	int defscr = DefaultScreen(dpy);
	int black = BlackPixel(dpy,defscr);
	int white = WhitePixel(dpy,defscr);
	
	XineramaScreenInfo *xscreens = XineramaQueryScreens(dpy, &num_scr);
	int scrnum = 0; // define a desired screen
	/* define the parameter of this screen
	** width and height of the screen */
	int w = xscreens[scrnum].width, h = xscreens[scrnum].height;
	// offset of the screen
	int x = xscreens[scrnum].x_org, y = xscreens[scrnum].y_org;
	// create a window
	Window win = XCreateSimpleWindow(dpy,DefaultRootWindow(dpy),x,y,w,h,
		0,black,black);

	/* fullscreen implementation */
	typedef struct
	{
		unsigned long flags;
		unsigned long functions;
		unsigned long decorations;
		long inputMode;
		unsigned long status;
	} Hints;
	
	Hints hints;
	Atom property;
	
	// Specify that we're changing the window decorations
	hints.flags = 2;
	// remove window's decorations (zero means false here)
	hints.decorations = 0;
	property = XInternAtom(dpy, "_MOTIF_WM_HINTS", True);
	XChangeProperty(dpy,win,property,property,32,PropModeReplace,
	(unsigned char *)&hints,5);
	
	XF86VidModeSwitchToMode(dpy,defscr,video_mode);
        XF86VidModeSetViewPort(dpy,defscr,0,0);
	XMoveResizeWindow(dpy,win,0,0,w,h);
	XMapRaised(dpy,win);
	XGrabPointer(dpy,win,True,0,GrabModeAsync,GrabModeAsync,win,0L,
	CurrentTime);
	XGrabKeyboard(dpy,win,False,GrabModeAsync,GrabModeAsync,CurrentTime);


	
	// We want to get MapNotify events
	XSelectInput(dpy,win,StructureNotifyMask);
	
	// "Map" the window (that is, make it appear on the screen)
	XMapWindow(dpy,win);
	
	// Create a "Graphics Context"
	GC gc = XCreateGC(dpy,win,0,NULL);
	
	// Tell the GC we draw using the white color
	XSetForeground(dpy,gc,white);

	for(;;)
	{
		XEvent e;
		XNextEvent(dpy,&e);
		if (e.type == MapNotify)
			break;
	}

	XDrawLine(dpy,win,gc,0,0,w,h);

	// Send the "DrawLine" request to the server
	XFlush(dpy);
	
	sleep(1.7);
	// printf("number of screens = %d\n",num_scr);
	return 0;
}

