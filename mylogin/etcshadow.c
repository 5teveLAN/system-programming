#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 4096
static char readline[BUFFER_SIZE];
typedef struct {
    char *username;      // 使用者名稱
	char *password;      // 加密後的密碼 (包含 salt 和 hash)
	long last_change;        // 上次修改密碼日期 (自 1970/1/1 起的天數)
	long min_days;           // 兩次修改密碼間隔的最少天數
	long max_days;           // 密碼有效期的最大天數
	long warn_days;          // 密碼過期前幾天開始警告
	long inactive_days;      // 密碼過期後帳號停用前的天數
	long expiration_date;    // 帳號失效日期
	unsigned long reserved;  // 保留欄位
} Shadow;
static void print_shadow_struct(const Shadow *shadow) {
	printf("\n--- Shadow Entry Detail ---\n");
	printf("%-16s : %s\n", "Username",        shadow->username ? shadow->username : "NULL");
	printf("%-16s : %s\n", "Password Hash",   shadow->password ? shadow->password : "NULL");
	printf("%-16s : %ld\n", "Last Changed",    shadow->last_change);
	printf("%-16s : %ld\n", "Min Days",        shadow->min_days);
	printf("%-16s : %ld\n", "Max Days",        shadow->max_days);
	printf("%-16s : %ld\n", "Warn Days",       shadow->warn_days);
	printf("%-16s : %ld\n", "Inactive Days",   shadow->inactive_days);
	printf("%-16s : %ld\n", "Expiration Date", shadow->expiration_date);
	printf("%-16s : %lu\n", "Reserved",        shadow->reserved);
	printf("---------------------------\n");
}
static void split_tostruct(Shadow *shadow){
    char *token;
    char *running = readline;
    int field = 0;
    // remove linefeed last string
    readline[strcspn(readline, "\n")] = 0;

    while (token=strsep(&running, ":")){
        switch(field){
            case 0:
				shadow->username = token;
				break;
			case 1:
				shadow->password = token;
				break;
			case 2:
				shadow->last_change = (*token) ? atol(token) : -1;
				break;
			case 3:
				shadow->min_days = (*token) ? atol(token) : -1;
				break;
			case 4:
				shadow->max_days = (*token) ? atol(token) : -1;
				break;
			case 5:
				shadow->warn_days = (*token) ? atol(token) : -1;
				break;
			case 6:
				shadow->inactive_days = (*token) ? atol(token) : -1;
				break;
			case 7:
				shadow->expiration_date = (*token) ? atol(token) : -1;
				break;
			case 8:
				shadow->reserved = (*token) ? strtoul(token, NULL, 10) : 0;
				break;
        }
        field++;
    }
}
int read_shadow(char *username, Shadow *shadow){
    FILE *fp = fopen("/etc/shadow", "r");
    long filesize;

    // 0. check file
    if (fp == NULL) {
        perror("開啟失敗");
        exit(EXIT_FAILURE);
    }
    // 1. get filesize
    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // 2. read (move)
    while (ftell(fp) < filesize){
        fgets(readline, BUFFER_SIZE, fp); // read 4096 in once
        split_tostruct(shadow);
        if (shadow->username && strcmp(shadow->username, username)== 0) 
            return 1;
    }

    fclose(fp);
    return 0;
}
/*
int main(){
    Shadow shadow;
    char *username = "ggd";

    if (read_shadow(username, &shadow))
        print_shadow_struct(&shadow);

}
*/
