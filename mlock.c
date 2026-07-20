#include <linux/input-event-codes.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <libevdev-1.0/libevdev/libevdev.h>

typedef struct {
    int fd;
    struct libevdev *dev;
} Mouse;

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

void event_handler(Mouse *mouse) {
    struct input_event ev;
    int rc;

    while ((rc = libevdev_next_event(mouse->dev, LIBEVDEV_READ_FLAG_NORMAL, &ev)) == LIBEVDEV_READ_STATUS_SUCCESS) {
        // printf("type=%d code=%d value=%d\n", ev.type, ev.code, ev.value);
        
        switch (ev.type) {
            case EV_KEY:
                if (ev.code == BTN_LEFT) {
                    if (ev.value) printf("click\n");
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

    mouse_init(&mouse, "/dev/input/event5");

    while (true) {
        event_handler(&mouse);
    }
    mouse_destroy(&mouse);
}
