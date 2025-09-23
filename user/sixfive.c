#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"

char *separators = " -\r\t\n./,";

int is_sep(char c) {
    for(int i = 0; separators[i]; i++)
        if(c == separators[i]) return 1;
    return 0;
}

int main(int argc, char *argv[]) {
    if(argc < 2){
        printf("Usage: sixfive <file> [...]\n");
        exit(1);
    }

    for(int i = 1; i < argc; i++){
        int fd = open(argv[i], 0);
        if(fd < 0){
            printf("cannot open %s\n", argv[i]);
            continue;
        }

        char buf[512];
        int n;
        char num_buf[20];
        int num_len = 0;

        while((n = read(fd, buf, sizeof(buf))) > 0){
            for(int j = 0; j < n; j++){
                char c = buf[j];
                if(c >= '0' && c <= '9'){
                    if(num_len < sizeof(num_buf)-1)
                        num_buf[num_len++] = c;
                } else {
                    if(num_len > 0){
                        num_buf[num_len] = 0;
                        int val = atoi(num_buf);
                        if(val % 5 == 0 || val % 6 == 0)
                            printf("%d\n", val);
                        num_len = 0;
                    }
                }
            }
        }

        if(num_len > 0){
            num_buf[num_len] = 0;
            int val = atoi(num_buf);
            if(val % 5 == 0 || val % 6 == 0)
                printf("%d\n", val);
        }

        close(fd);
    }

    exit(0);
}
