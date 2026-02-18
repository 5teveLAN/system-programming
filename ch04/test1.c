#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
    int fd;
    char *msg = "Log entry: User logged in.\n";

    // 任務：打開一個名為 "sys.log" 的檔案，如果不存在就建立，
    //      如果存在，就在「末尾」繼續寫入 (Append)，不可清空舊資料。
    //      權限設為 0644。

    // 錯誤 1 & 2 藏在這裡
    fd = open("sys.log", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR);

    if (fd == 1) { // 錯誤 3 藏在這裡
        perror("Open failed");
        exit(1);
    }

    // 任務：將訊息寫入檔案
    // 錯誤 4 藏在這裡
    write(fd, msg, sizeof(msg)); 

    // 任務：讀取剛剛寫入的內容並印出來
    char buf[100];
    lseek(fd, 0, SEEK_SET); // 回到開頭
    read(fd, buf, 100);
    printf("Read from log: %s", buf);

    // 錯誤 5 藏在這裡 (關於資源回收)
    return 0;
}
