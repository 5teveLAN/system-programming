#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/file.h>

void init_db(int fd){
    char *value = "3500";
    ssize_t bytes_written = write(fd, value, 4);

    if (bytes_written == -1) {
        perror("write failed");
    }
}


void reverse_str(char* str){
    char tmp[10];
    int len = strlen(str);
    tmp[len]='\0';
    for (int i = 0;i<len;++i)
        tmp[len-i-1] = str[i];
    strncpy(str, tmp, len+1);
}

void get_num(int fd, char *num){
    int idx =0;
    while (lseek(fd, -1, SEEK_CUR) != -1){
        read(fd, &num[idx], 1);
        if (num[idx]=='\0') break; // hole
        lseek(fd, -1, SEEK_CUR);
        idx++;
    }
    num[idx] = '\0';
}

void jump_and_write(int fd, int start_num, int count){
    char str[20];
    struct timespec req = {0, 100 * 1000 * 1000}; // 100毫秒 (0.1秒)
    struct timespec rem;
    lseek(fd, 0, SEEK_END);
    
    while (count--){
        lseek(fd, start_num++, SEEK_CUR);
        snprintf(str, sizeof(str), "%d", start_num);
        
        ssize_t bytes_written = write(fd, str, strlen(str));
        lseek(fd, -strlen(str), SEEK_CUR);

        if (bytes_written == -1) {
            perror("write failed");
        }
        nanosleep(&req, &rem); // 睡眠500毫秒
    }
}
int main(){
    // 用法：int fd = open(path, flags, mode);
    char* filename = "gg.db";
    int fd = open(filename, O_RDWR | O_CREAT , 0644);
    printf("Blocking...\n");
    flock(fd, LOCK_EX);
    printf("UNBlocked!\n");

    if (fd == -1) {
        perror("open failed");
        exit(1);
    }
    off_t filesize = lseek(fd, 0, SEEK_END);
    if (!filesize)
        init_db(fd);

    char num[20];
    int count = 10000;

    get_num(fd, num);
    reverse_str(num);
    jump_and_write(fd, atoi(num), count);
    
    close(fd);

}
