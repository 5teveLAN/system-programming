/*
題目：
    • 寫一隻小的應用程式名稱為mylogin，每當使用者輸入姓名時，mylogin會判斷這個使用者是否可以進入伺服器，判斷的依據為該成員是否在/etc/passwd內。『不用輸入密碼』
    • 如果可以進入伺服器，則login會產生一個child process，這個child process會設定適當的變數，然後執行『bash』
    • 當使用者離開shell以後，要再跳出提示符號，讓使用者再次登入
    • 挑戰：是否可以讓使用者輸入密碼，然後到/etc/shadow內驗證密碼呢？（不計分）
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> // For wait
       
int main(){
    pid_t pid;
    int status;

    //getchar();

    pid = fork();

    // is child
    if (pid == 0){
        execlp("bash", "bash", NULL);
    }
    // is parent
    else if (pid > 0){
        wait(&status);
        printf("Program quit\n");
    }
    else {
        perror("error");
        exit(EXIT_FAILURE);
    }

} 
