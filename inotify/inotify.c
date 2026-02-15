#include <stdio.h>      /* 標準輸入輸出 (printf, perror) */
#include <stdlib.h>     /* 標準工具庫 (exit, malloc) */
#include <unistd.h>     /* Unix 標準函數 (read, close) */
#include <stdint.h>     /* 定義 uint32_t 等固定長度型別 */
#include <sys/inotify.h>/* Inotify 核心 API (inotify_init, inotify_event) */
#include <limits.h>     /* (選擇性) 若需要用到 NAME_MAX 等常數 */
#define BUFFER_SIZE 1000

int main(int argc, char* argv[]){
    int fd, wd;
    char buffer[BUFFER_SIZE];
    ssize_t read_bytes;
    int current_pos;

    /* 1. 初始化 inotify */
    fd = inotify_init();
    if (fd < 0) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }
    /* 2. 加入監控路徑：目前的目錄 (.)
	   監控：建立、刪除、修改 */
    wd = inotify_add_watch(fd, ".", IN_MODIFY | IN_CREATE | IN_DELETE);
    if (wd == -1) {
        printf("無法監控該目錄\n");
    }
    else {
        printf("正在監控目前的目錄事件...\n");
    }
    // 3. loop
    while (1){
        current_pos = 0;
        read_bytes = read(fd, buffer, BUFFER_SIZE);
        while (current_pos < read_bytes){
            struct inotify_event *event = (struct inotify_event *)&buffer[current_pos];
            if (event->len) {
                if (event->mask & IN_CREATE) {
                    printf("檔案 %s 被建立。\n", event->name);
                } else if (event->mask & IN_DELETE) {
                    printf("檔案 %s 被刪除。\n", event->name);
                } else if (event->mask & IN_MODIFY) {
                    printf("檔案 %s 被修改。\n", event->name);
                }
            }
            current_pos+=sizeof(event)+event->len;

        }
    }

}
