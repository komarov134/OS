#include <stddef.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "readlines.h"

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

RL * rl_open(int fd, size_t max_size) {
    RL *rl = (RL *) malloc(sizeof(RL));
    if (rl == NULL)
        return NULL;
    rl->fd = fd;
    rl->max_size = max_size;
    rl->size = 0;
    rl->skip = 0;
    rl->buf = (char *) malloc(max_size + 1);
    if (rl->buf == NULL) {
        free(rl);
        return NULL;
    }
    return rl;
}

size_t rl_max_size(RL *rl) {
    return rl->max_size;
}

int rl_close(RL *rl) {
    int close_status = close(rl->fd);
    int error_code = errno;
    free(rl->buf); // should not cause any error?
    free(rl); // should not cause any error?
    if (close_status == -1)
        errno = error_code;
    return close_status;
}
    
int rl_readline(RL *rl, char *buf, size_t buf_size) {
    size_t rsize = min(rl_max_size(rl) + 1, buf_size);
    int ssize = -1;
    size_t i;
    for (i = 0; i < rl->size; i++) { // check if we already have some
                                            // line in buffer
        if (rl->buf[i] == '\n') {
            ssize = i + 1;
            break;
        }
    }

    if (ssize == -1) { // try reading additional data
        while (ssize == -1) {
            if (rl->size == rl_max_size(rl) + 1) { // overflow, we
                                                   // should clean
                                                   // buffer, set "skipping"
                                                   // state and return -3
                rl->size = 0;
                rl->skip = 1;
                return -3;
            }

            int r = read(rl->fd, rl->buf + rl->size, rl_max_size(rl) + 1 - rl->size);
            if (r == -1) { // error
                return -1; // errno has already been set
            } else if (r == 0) { // end of file
                ssize = rl->size;
            } else { // have read something
                for (i = 0; i < r; i++) {
                    if (rl->buf[rl->size + i] == '\n') {
                        ssize = rl->size + i + 1;
                        break;
                    }
                }
                rl->size += r;
            }
        }
    }

    // ssize should be >= 0 by this point

    if (rl->skip == 1) {
        rl->skip = 0;
        memmove(rl->buf, rl->buf + ssize, rl->size - ssize);
        rl->size -= ssize;
        return -3;
    }
    
    if (ssize == 0) {
        return 0;
    }

    int rc = (ssize > rsize) ? -2 : ssize;
    if (rc != -1) {
        memmove(buf, rl->buf, ssize);
    }
    memmove(rl->buf, rl->buf + ssize, rl->size - ssize); // drop the string from the buffer
    rl->size -= ssize;
    return rc;
}
