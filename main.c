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


    while ((rc = libevdev_next_event(mouse->dev, LIBEVDEV_READ_FLAG_NORMAL, &ev)) == LIBEVDEV_READ_STATUS_SUCCESS) {
        // printf("type=%d code=%d value=%d\n", ev.type, ev.code, ev.value);
        
        switch (ev.type) {
            case EV_KEY:
                if (ev.code == BTN_LEFT) {
                    if (ev.value) {
                        printf("click\n");
                        if (fmouse->locked) {
                            clock_gettime(CLOCK_MONOTONIC, &fmouse->press_time);
                            fmouse_action(fmouse, RELEASE);
                            fmouse->locked = false;
                        }
                        else {
                            clock_gettime(CLOCK_MONOTONIC, &fmouse->press_time);
                        }
                    }
                    else {
                        struct timespec now;
                        printf("release\n");
                        clock_gettime(CLOCK_MONOTONIC, &now);
                        double elapsed =
                            (now.tv_sec - fmouse->press_time.tv_sec) +
                            (now.tv_nsec - fmouse->press_time.tv_nsec) / 1e9;
                        printf("pressed for: %fms\n", elapsed);
                        if (!fmouse->locked && elapsed >= 0.5) {
                            fmouse_action(fmouse, CLICK);
                            fmouse->locked = true;
                        }
                        printf("%d\n", fmouse->locked);
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
