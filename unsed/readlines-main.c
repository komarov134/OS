#include <stdlib.h>
#include <unistd.h>

#include "readlines.h"

void write_line(char *buf, int length) {
    int count = 0;
    while (count != length) {
        int w = write(1, buf + count, length - count);
        count += w;
    }
}


int main(int argc, char *argv[]) {
    int len = atoi(argv[1]);
    RL *rl = rl_open(0, len);
    char *buf = (char *) malloc(len + 1);
    
    while (1) {
        int rc = rl_readline(rl, buf, len);
        if (rc == 0)
            break;
        else if (rc > 0) {
            write_line(buf, rc);
        }
    }
    free(buf);
    rl_close(rl);
    return 0;
}
