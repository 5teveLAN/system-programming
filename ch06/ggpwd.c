#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 1024

void ggpwd(char* path){
    long res;
    __asm__ volatile(
    "movq $79, %%rax \n\t"
    "movq %1, %%rdi \n\t"
    "movq %2, %%rsi \n\t"
    "syscall \n\t"
    "movq %%rax, %0  \n\t"
    : "=r"(res) // output
    : "r" (path), "r" ((long)BUFFER_SIZE) // input
    : "rax", "rdi", "rsi", "rcx", "r11", "memory"
    // memory 只要你的組合語言會「修改」指標指向的記憶體內容，就要加。
    );
}


int main(){
    char* path;
    path = (char*)malloc(BUFFER_SIZE);
    ggpwd(path);
    printf("%s\n", path);


}
