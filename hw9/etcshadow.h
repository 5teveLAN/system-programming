#ifndef ETCSHADOW_H
#define ETCSHADOW_H

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
int read_shadow(char *username, Shadow *shadow);
#endif


