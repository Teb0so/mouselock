#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <libevdev/libevdev.h>
#include <libevdev/libevdev-uinput.h>
#include "mouse.h"

// TODO: iterate over every device to find mouses with findmouse() function
int mouse_init(Mouse *mouse, const char *device) {
    mouse->fd = open(device, O_RDONLY);
    if (mouse->fd < 0) return -1;

    int rc = libevdev_new_from_fd(mouse->fd, &mouse->dev);
    if (rc < 0) {
        close(mouse->fd);
        return rc;
    }
    return 0;
}

int fmouse_init(Mouse mouse, FakeMouse *fmouse) {
    int rc;

    rc = libevdev_uinput_create_from_device(mouse.dev, LIBEVDEV_UINPUT_OPEN_MANAGED, &fmouse->udev);
    if (rc == 0) return -1;

    return 0;
}

// actiontype can be CLICK or RELEASE
void fmouse_action(FakeMouse *fmouse, int actiontype) {
    if (actiontype == CLICK) {
        libevdev_uinput_write_event(fmouse->udev, EV_KEY, BTN_LEFT, 1);
        libevdev_uinput_write_event(fmouse->udev, EV_SYN, SYN_REPORT, 0);
    }
    else if (actiontype == RELEASE) {
        libevdev_uinput_write_event(fmouse->udev, EV_KEY, BTN_LEFT, 0);
        libevdev_uinput_write_event(fmouse->udev, EV_SYN, SYN_REPORT, 0);
    }
}

int mouse_destroy(Mouse *mouse) {
    libevdev_free(mouse->dev);
    close(mouse->fd);
    return 0;
}

int fmouse_destroy(FakeMouse *fmouse) {
    libevdev_uinput_destroy(fmouse->udev);
    return 0;
}
