/*
題目：
    • 寫一隻小的應用程式名稱為mylogin，每當使用者輸入姓名時，mylogin會判斷這個使用者是否可以進入伺服器，判斷的依據為該成員是否在/etc/passwd內。『不用輸入密碼』
    • 如果可以進入伺服器，則login會產生一個child process，這個child process會設定適當的變數，然後執行『bash』
    • 當使用者離開shell以後，要再跳出提示符號，讓使用者再次登入
    • 挑戰：是否可以讓使用者輸入密碼，然後到/etc/shadow內驗證密碼呢？（不計分）
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h> // For wait
#include "etcpasswd.h"
#include "etcshadow.h"
#include <crypt.h> // 必須包含這個標頭檔

Passwd passwd;   
Shadow shadow;
#define PASS 1
#define FAIL 0
#define ATTEMPT_COUNT 3

int verify_password(const char *input_plain_password, const char *shadow_password_hash) {
    // crypt 會回傳加密後的結果指標
    char *hashed_input = crypt(input_plain_password, shadow_password_hash);

    if (hashed_input == NULL) {
        perror("crypt error");
        return 0;
    }

    // 比較新產生的雜湊值與 Shadow 檔案中的雜湊值
    if (strcmp(hashed_input, shadow_password_hash) == 0) {
        return PASS; // 密碼正確
    } else {
        return FAIL; // 密碼錯誤
    }
}
int auth(){
	int attempt = 0;
    char username[100];
    char *password;

    printf("Username: ");
    scanf("%s", username);
	// 1. check user exist
    if (!read_passwd(username, &passwd) ||
        !read_shadow(username, &shadow)){ 
        printf("No such user\n");
        return FAIL;
    }

	// 2. check user can access
    if (strstr(passwd.shell, "nologin") ||
        strstr(passwd.shell, "false") ||
        strstr(passwd.shell, "sync") ) {
        printf("This account can't be logged\n");
        return FAIL;
    }

	// 3. check user password
	getchar();
	password = getpass("Password: ");
	while (!verify_password(password, shadow.password) &&
			attempt < ATTEMPT_COUNT) {
        printf("Try again.\n");
		password = getpass("Password: ");
		attempt++;
		}
	if (attempt >= ATTEMPT_COUNT)
        return FAIL;

    return PASS;
}
int main(){
    pid_t pid;
    int status;

    while (1){
        if (auth() == FAIL) {
            continue;}
        printf("Loged\n");

        pid = fork();

        // is child
        if (pid == 0){
            setuid(passwd.uid);
            printf("login with username:%s, uid:%d\n", passwd.username, passwd.uid);
            execlp(passwd.shell, passwd.shell, NULL);
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
} 
