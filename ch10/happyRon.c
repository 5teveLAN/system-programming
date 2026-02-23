#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <assert.h>

sig_atomic_t isExit =0;
//main_jumpbuf;

int row=-1;
int col=-1;

void winChange(int signumber) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    col = w.ws_col;
    row = w.ws_row;
    printf("row=%d, col=%d\n", row, col);
}

void sigShow(int signumber) {
    printf("get a signal named '%d', '%s'\n", signumber,
           strsignal(signumber));
}


void sighandler(int signo) {
    if (signo == SIGINT) {
        printf("按下ctr-c，但殺不死我\n");
        sleep(3);  //假裝在ctr-c裡面運算了10秒鐘
    }
    if (signo == SIGTSTP)
        printf("按下ctr-z，但殺不死我\n");
    if (signo == SIGKILL)
        printf("按下ctr-z，但殺不死我\n");
}


int main(int argc, char** argv) {
    printf("本task的學號是 %d\n", getpid());

    int sig_exist[100];
    int idx = 0;
    int n;  //n1
	signal(SIGINT, SIG_IGN);
    //使用者按下ctr-c，OS直接忽略ctr-c，然後這個signal『『不會』』送給這個task
    do {
        int n;  //n2 macro
        printf("告訴作業系統，使用者按下ctr-c時，這個 「訊號（singal）」不處理\n");
    }while(0);

    printf("SIGKILL是直接殺掉一個task\n");
    if (signal(SIGKILL, sighandler) == SIG_ERR) {
        printf("無法改變SIGKILL的行為\n");
        perror("SIGKILL的問題，具體來說是：");
    }
    signal(SIGWINCH, winChange);

    if (signal(SIGSTOP, sighandler) == SIG_ERR) {
        printf("無法改變SIGSTOP的行為\n");
        perror("SIGSTOP的問題，具體來說是：");
    }

    signal(SIGTSTP, sighandler);
    

    //測試在fork的環境下是child是否繼承parent的signal的設定
    int child_pid = fork();
    if (child_pid ==0) {    //child
        printf("child：child準備執行execv('ls')，等一下試著按下ctr-c\n");
        printf("child：如果按下ctr-c無法終止，試著按下ctr-\\\n");
        execlp("ls", "ls","/","-alhR",NULL);
    } else {    //parent
        printf("chld的pid是%d\n", child_pid);
        //讓parent執行一個無窮迴圈
        while(1) {
            pause();
        }
    }
}
