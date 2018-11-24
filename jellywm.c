/* jellywm - by JamsNJellies */
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "config.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main(void)
{
    Display * dpy;
    XWindowAttributes attr;
    XButtonEvent start;
    XEvent ev;
    Screen * scr;
    XEvent newev;

    // If we cannot open the display, return 1
    if(!(dpy = XOpenDisplay(0x0))) return 1;

    scr = XDefaultScreenOfDisplay(dpy);
    int scrwidth = WidthOfScreen(scr);
    int scrheight = HeightOfScreen(scr);
    
    // Grab keyboard input
    XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("r")), Mod4Mask,
            DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
    XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("q")), Mod4Mask,
            DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
    XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("f")), Mod4Mask,
	    DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
    XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("e")), Mod4Mask,
	    DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
    XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("d")), Mod4Mask,
	    DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
    
    // Grab Mouse button input
    XGrabButton(dpy, 1, Mod4Mask, DefaultRootWindow(dpy), True,
            ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);
    XGrabButton(dpy, 3, Mod4Mask, DefaultRootWindow(dpy), True,
            ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);

    start.subwindow = None;
    for(;;)
    {
	// Handle events    
	XNextEvent(dpy, &ev);
	if(ev.type == KeyPress)
	{
		// Allways handle these events
		if(ev.xkey.keycode == XKeysymToKeycode(dpy, XStringToKeysym("e")))
		{
			system("urxvt &");
		}

		if(ev.xkey.keycode == XKeysymToKeycode(dpy, XStringToKeysym("d")))
		{
			system("rofi -show drun &");
		}

		// Handle these events only if the mouse pointer is inside a window
        	if(ev.xkey.subwindow != None)
		{
			if(ev.xkey.keycode == XKeysymToKeycode(dpy, XStringToKeysym("r")))
			{
				XRaiseWindow(dpy, ev.xkey.subwindow);
			}

			else if(ev.xkey.keycode == XKeysymToKeycode(dpy, XStringToKeysym("q")))
			{
				newev.xclient.type = ClientMessage;
				newev.xclient.window = ev.xkey.subwindow;
				newev.xclient.message_type = XInternAtom(dpy, "WM_PROTOCOLS", true);
				newev.xclient.format = 32;
				newev.xclient.data.l[0] = XInternAtom(dpy, "WM_DELETE_WINDOW", false);
				newev.xclient.data.l[1] = CurrentTime;
				XSendEvent(dpy, ev.xkey.subwindow, False, NoEventMask, &newev);
			}

			else if(ev.xkey.keycode == XKeysymToKeycode(dpy, XStringToKeysym("f")))
			{
				XMoveResizeWindow(dpy, ev.xkey.subwindow, 0+GAP, 0+GAP, scrwidth-GAP*2, scrheight-GAP*2);
			}
		}
	}
	
	// Mouse resize and move
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
