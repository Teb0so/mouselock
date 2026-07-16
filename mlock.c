#include <fcntl.h>
#include <unistd.h>
#include <libevdev-1.0/libevdev/libevdev.h>

typedef struct {
    int fd;
    struct libevdev *dev;
} Mouse;

int mouse_init(Mouse *mouse, const char *device) {
    mouse->fd = open(device, O_RDONLY | O_NONBLOCK);
    if (mouse->fd < 0) return -1;

    int rc = libevdev_new_from_fd(mouse->fd, &mouse->dev);
    if (rc < 0) {
        close(mouse->fd);
        return rc;
    }
    return 0;
}

int mouse_destroy(Mouse *mouse) {
    libevdev_free(mouse->dev);
    close(mouse->fd);
    return 0;
}
