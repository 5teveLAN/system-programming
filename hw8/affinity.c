#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();
    cpu_set_t mask;
    CPU_ZERO(&mask);        // 清空集合
    CPU_SET(0, &mask);      // 綁定到 CPU 0
    CPU_SET(2, &mask);      // 綁定到 CPU 2

    // 設定當前進程 (pid=0) 的 Affinity
    if (sched_setaffinity(0, sizeof(cpu_set_t), &mask) == -1) {
        perror("sched_setaffinity 失敗");
    } else {
        printf("成功綁定至 CPU 0 與 CPU 2 \n");
    }
    if (pid == 0){ // child
        printf("this is child, pid=%d\n", getpid());
        while(1);
    }
    else if (pid > 0){ //parent
        printf("this is parent, pid=%d\n", getpid());
    }
    else{
        printf("fork failed\n");
    }


    // 模擬運算
    while(1); 

    return 0;
}
