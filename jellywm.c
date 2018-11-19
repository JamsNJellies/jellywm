/* jellywm - by JamsNJellies */
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main(void)
{
    Display * dpy;
    XWindowAttributes attr;
    XButtonEvent start;
    XEvent ev;

    if(!(dpy = XOpenDisplay(0x0))) return 1;

    XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("r")), Mod4Mask,
            DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
    XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("q")), Mod4Mask,
            DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);

    XGrabButton(dpy, 1, Mod4Mask, DefaultRootWindow(dpy), True,
            ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);
    XGrabButton(dpy, 3, Mod4Mask, DefaultRootWindow(dpy), True,
            ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);

    start.subwindow = None;
    for(;;)
    {
        XNextEvent(dpy, &ev);
        if(ev.type == KeyPress && ev.xkey.subwindow != None)
	{
		if(ev.xkey.keycode == XKeysymToKeycode(dpy, XStringToKeysym("r")))
		{
			XRaiseWindow(dpy, ev.xkey.subwindow);
		}

		else if(ev.xkey.keycode == XKeysymToKeycode(dpy, XStringToKeysym("q")))
		{
			XDestroySubwindows(dpy, ev.xkey.subwindow);
			XDestroyWindow(dpy, ev.xkey.subwindow);
		}
	}
	
        else if(ev.type == ButtonPress && ev.xbutton.subwindow != None)
        {
            XGetWindowAttributes(dpy, ev.xbutton.subwindow, &attr);
            start = ev.xbutton;
        }

        else if(ev.type == MotionNotify && start.subwindow != None)
        {
            int xdiff = ev.xbutton.x_root - start.x_root;
            int ydiff = ev.xbutton.y_root - start.y_root;
            XMoveResizeWindow(dpy, start.subwindow,
                attr.x + (start.button==1 ? xdiff : 0),
                attr.y + (start.button==1 ? ydiff : 0),
                MAX(1, attr.width + (start.button==3 ? xdiff : 0)),
                MAX(1, attr.height + (start.button==3 ? ydiff : 0)));
        }
        else if(ev.type == ButtonRelease)
	{
            start.subwindow = None;
	}
    }
}
