#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <libevdev/libevdev.h>
#include <libevdev/libevdev-uinput.h>
#include <time.h>
#include <bits/time.h>
#include "mouse.h"

void event_handler(Mouse *mouse, FakeMouse *fmouse) {
    struct input_event ev;
    int rc;

    struct timespec start, end;

    while ((rc = libevdev_next_event(mouse->dev, LIBEVDEV_READ_FLAG_NORMAL, &ev)) == LIBEVDEV_READ_STATUS_SUCCESS) {
        // printf("type=%d code=%d value=%d\n", ev.type, ev.code, ev.value);
        
        switch (ev.type) {
            case EV_KEY:
                if (ev.code == BTN_LEFT) {
                    if (ev.value) {
                        printf("click\n");
                        clock_gettime(CLOCK_MONOTONIC, &start);
                        // fmouse_action(fmouse, CLICK);
                        // fmouse_action(fmouse, RELEASE);
                    }
                    else {
                        printf("release\n");
                        clock_gettime(CLOCK_MONOTONIC, &end);
                        double elapsed =
                            (end.tv_sec - start.tv_sec) +
                            (end.tv_nsec - start.tv_nsec) / 1e9;
                        printf("clicked for: %f\n", elapsed);
                    }
                }

                break;
            case EV_REL:
                break;
                
            case EV_SYN:
                break;
        }
    }
}

int main() {
    Mouse mouse;
    FakeMouse fmouse;

    mouse_init(&mouse, "/dev/input/event5");
    fmouse_init(mouse, &fmouse);

    while (true) {
        event_handler(&mouse, &fmouse);
    }
    mouse_destroy(&mouse);
    fmouse_destroy(&fmouse);
}
