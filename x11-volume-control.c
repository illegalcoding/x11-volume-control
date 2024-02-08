/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2024, illegalcoding
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <X11/X.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <signal.h>
#include <stdlib.h>

#define XF86AudioLowerVolume 122
#define XF86AudioRaiseVolume 123
#define KEYCODE event.xkey.keycode
int do_exit = 0;
void signal_handler(int signum) {
	fprintf(stderr, "Signal caught, exiting...\n");
	do_exit = 1;	
}

int main() {
	struct sigaction new_action, old_action;
	new_action.sa_handler = signal_handler;
	sigemptyset(&new_action.sa_mask);
	new_action.sa_flags = 0;

	sigaction(SIGINT, NULL, &old_action);
	if (old_action.sa_handler != SIG_IGN) {
		sigaction(SIGINT, &new_action, NULL);
	}
	sigaction(SIGHUP, NULL, &old_action);
	if (old_action.sa_handler != SIG_IGN) {
		sigaction(SIGHUP, &new_action, NULL);
	}
	sigaction(SIGTERM, NULL, &old_action);
	if (old_action.sa_handler != SIG_IGN) {
		sigaction (SIGTERM, &new_action, NULL);
	}

	Window root;
	Display *dpy = NULL;
	dpy = XOpenDisplay(0);
	if(dpy == NULL) {
		fprintf(stderr,"Open display failed");
		return -1;
	}
	root = DefaultRootWindow(dpy);
	unsigned int mod = 0;
	XGrabKey(dpy,XF86AudioLowerVolume,AnyModifier,root,True,GrabModeAsync,GrabModeAsync);
	XGrabKey(dpy,XF86AudioRaiseVolume,AnyModifier,root,True,GrabModeAsync,GrabModeAsync);
	XEvent event;
	while(!do_exit) {
		XNextEvent(dpy, &event);
		if(event.type == KeyPress) {
			/* printf("Key press: %d\n", KEYCODE); */
			if(KEYCODE == XF86AudioLowerVolume) {
				/* fprintf(stderr,"Lowering volume...\n"); */
				system("/usr/bin/pamixer -d 2");
			} else if(KEYCODE == XF86AudioRaiseVolume) {
				/* fprintf(stderr,"Raising volume...\n"); */
				system("/usr/bin/pamixer -i 2");
			}
		}
	}
	XAllowEvents(dpy, AsyncKeyboard,CurrentTime);
	XUngrabKey(dpy,XF86AudioLowerVolume,AnyModifier,root);
	XUngrabKey(dpy,XF86AudioRaiseVolume,AnyModifier,root);
	XSync(dpy,False);
	XCloseDisplay(dpy);
	return 0;
}
