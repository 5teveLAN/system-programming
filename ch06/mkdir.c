#include <sys/stat.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

int my_mkdir(char* filename, int mode){
    return syscall(SYS_mkdirat, AT_FDCWD, filename, mode);
}

int my_raw_mkdir(char *path, int mode) {
    long res;

    __asm__ volatile (
        "movq $258, %%rax \n\t"   // 1. 系統呼叫號 258 (mkdirat)
        "movq $-100, %%rdi \n\t"  // 2. dirfd = AT_FDCWD (-100) 當前目錄
        "movq %1, %%rsi \n\t"     // 3. pathname = path
        "movq %2, %%rdx \n\t"     // 4. mode = mode
        //"movq $0, %%r10 \n\t"     // 5. flags = 0
        "syscall \n\t"            // 6. 發動接力賽！
        "movq %%rax, %0 \n\t"     // 7. 拿回核心的回傳值
        : "=r"(res)               // 輸出到 res
        : "g"(path), "g"(mode)    // 輸入參數
        : "rax", "rdi", "rsi", "rdx", "rcx", "r11" // 被更動的暫存器
    );
	//# 等於 syscall(SYS_mkdirat, AT_FDCWD, path, mode);
    return (int)res;
}

int main() {
    char *dirname = "test_minifs_dir";
    
    // 建立目錄，權限設定為 0755
    if (my_raw_mkdir(dirname, 0755) == 0) {
        printf("Directory created successfully: %s\n", dirname);
    } else {
        if (errno == EEXIST) {
            printf("Directory already exists.\n");
        } else {
            perror("mkdir failed");
            return 1;
        }
    }
    
    return 0;
}
