#ifndef ETCPASSWD_H
#define ETCPASSWD_H
typedef struct {
    char* username;
    char* password;
    int uid;
    int gid;
    char* gecos;
    char* directory;
    char* shell;
} Passwd;
int read_passwd(char *username, Passwd *passwd);
#endif


