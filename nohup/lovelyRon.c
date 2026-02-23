#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char** argv) {

    //底下這一行是將『標準輸入』改成從 "/dev/null"
    //dev/null : 在類Unix系統中，/dev/null，或稱空設備，是一個特殊的設備文件，它丟棄一切寫入其中的數據（但報告寫入操作成功），
    //🟧 🟨 🟩 🟦讀取它則會立即得到一個EOF。🟧 🟨 🟩 🟦
    //原文網址：https://kknews.cc/code/valkama.html
	printf("start\n");
    freopen("/dev/null", "R+", stdin);      // 1️⃣ 
    //底下這一行是將『標準輸出』改成從 "stdout.log"
    freopen("/home/ggd/Desktop/system-programming/nohup/stdout.log", "w+", stdout);    // 2️⃣ 
    //底下這一行是將『標準錯誤』改成從 "stderr.log"
    freopen("/home/ggd/Desktop/system-programming/nohup/stderr.log", "w+", stderr);    // 3️⃣ 
    //假裝我們的程式需要印出一些東西，然後又讀取一些東西
    for (int i=0; i<10; i++) {
        printf("%d: hello\n", i);       // 4️⃣  
        int c = getchar();              // 5️⃣ 
    }
    //睡10秒鐘
    //告訴作業系統，忽略『掛斷』的信號。SIGHUP。signal hang up
    signal(SIGHUP, SIG_IGN);    //  6️⃣
    //在20秒內切斷terminal的連線會怎樣呢？ls還會正常執行嗎？
    printf("試著在20秒內切斷連線\n");
    for (int i=0; i<5; i++) {
        fprintf(stderr, "*");
        sleep(1);
        sync();
    }
	//err須重定向
    execlp("ls", "ls", "/", "-R", NULL);  // 7️⃣
	//out須重定向
}
