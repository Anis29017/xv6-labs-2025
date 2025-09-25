#include "user/user.h"

int main(int argc, char **argv) {
    if(argc < 2) {
        printf("Usage: sleep <ticks>\n");
        return 1;
    }
    int ticks = atoi(argv[1]);
    pause(ticks);
    return 0;
}
