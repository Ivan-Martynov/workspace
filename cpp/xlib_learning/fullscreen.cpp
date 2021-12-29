/* testing program
** g++ input.c -o output -lX11 -lXinerama
*/

#include <X11/Xlib.h>
#include <X11/extensions/Xinerama.h>
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
	int scrnum = 1; // define a desired screen
	/* define the parameter of this screen
	** width and height of the screen */
	int w = xscreens[scrnum].width, h = xscreens[scrnum].height;
	// offset of the screen
	int x = xscreens[scrnum].x_org, y = xscreens[scrnum].y_org;
	Window win = XCreateSimpleWindow(dpy,DefaultRootWindow(dpy),x,y,w,h,
		0,black,black);
		
	/* full screen implementation */		
	
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
	
	/* -------------------------------------------------------- */

	XSelectInput(dpy,win,StructureNotifyMask);
	XMapWindow(dpy,win);
	GC gc = XCreateGC(dpy,win,0,NULL);
	XSetForeground(dpy,gc,white);
	
	for(;;)
	{
		XEvent e;
		XNextEvent(dpy,&e);
		if (e.type == MapNotify)
			break;
	

	}

	XMoveWindow(dpy,win,x,y);
	XDrawLine(dpy,win,gc,0,0,w,h);

	XFlush(dpy);
	
	sleep(2.7);

	XCloseDisplay(dpy);
	//printf("number of screens = %d\n",num_scr);
	return 0;
}

