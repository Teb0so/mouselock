#ifndef MOUSE_H
#define MOUSE_H

#include <stdbool.h>
#include <time.h>

enum actiontype {
    CLICK,
    RELEASE
};

typedef struct {
    int fd;
    struct libevdev *dev;
} Mouse;

typedef struct {
    struct libevdev_uinput *udev;
    bool locked;
    struct timespec press_time;
} FakeMouse;

extern int mouse_init(Mouse *mouse, const char *device);
extern int fmouse_init(Mouse mouse, FakeMouse *fmouse);
extern void fmouse_action(FakeMouse *fmouse, int actiontype);
extern int mouse_destroy(Mouse *mouse);
extern int fmouse_destroy(FakeMouse *fmouse);

#endif //MOUSE_H
