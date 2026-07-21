#include <linux/input-event-codes.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <libevdev/libevdev.h>
#include <libevdev/libevdev-uinput.h>

enum actiontype{
    CLICK,
    RELEASE
};

typedef struct {
    int fd;
    struct libevdev *dev;
} Mouse;

typedef struct {
    struct libevdev_uinput *udev;
} FakeMouse;

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

int fmouse_create(Mouse mouse, FakeMouse *fmouse) {
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
                        fmouse_action(fmouse, CLICK);
                        fmouse_action(fmouse, RELEASE);
                    }
                    else printf("release\n");
                }

                break;
            case EV_REL:
                break;
                
            case EV_SYN:
                break;
        }
    }
}

int mouse_destroy(Mouse *mouse) {
    libevdev_free(mouse->dev);
    close(mouse->fd);
    return 0;
}

int main() {
    Mouse mouse;
    FakeMouse fmouse;

    mouse_init(&mouse, "/dev/input/event5");
    fmouse_create(mouse, &fmouse);

    while (true) {
        event_handler(&mouse, &fmouse);
    }
    mouse_destroy(&mouse);
}
