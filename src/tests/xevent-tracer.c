/*
 * Copyright (C) 2017 ~ $year Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>

static Display* dpy;
static Window focus_win = None;

static void attach_to_focuswin(void) {
  int revert_to = 0;

  XGetInputFocus(dpy, &focus_win, &revert_to);
  XSetWindowAttributes attr;
  attr.event_mask = FocusChangeMask | KeyPressMask;
  XChangeWindowAttributes(dpy, focus_win, CWEventMask, &attr);

  if (focus_win != None)
    XSelectInput(dpy, focus_win, FocusChangeMask | KeyPressMask);
  else
    sleep(1);
}

static void handle_event(void) {
  XEvent ev;
  char buf[100];
  int len;

  XNextEvent(dpy, &ev);
  switch (ev.xany.type) {
    case FocusIn: {
      fprintf(stdout, "FocusIn: %ld\n", focus_win);
      break;
    }
    case FocusOut: {
      fprintf(stdout, "FocusOut: %ld\n", focus_win);
      focus_win = None;
      break;
    }
    case KeyPress: {
      len = XLookupString(&ev.xkey, buf, 99, 0, 0);
      buf[len] = 0;
      printf("KeyPress: %s\n", buf);
      break;
    }
    case KeyRelease: {
      len = XLookupString(&ev.xkey, buf, 99, 0, 0);
      buf[len] = 0;
      printf("KeyRelease: %s\n", buf);
      break;
    }
    default: {
      fprintf(stdout, "type: %d\n", ev.type);
    }
  }
}

int main(void) {

  dpy = XOpenDisplay(getenv("DISPLAY"));

  if (dpy == NULL) {
    fprintf(stdout, "cannot init display\n");
    exit(1);
  }

  while (1) {
    if (focus_win == None)
      attach_to_focuswin();
    else
      handle_event();
  }
}