#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 4096
char readline[BUFFER_SIZE];
typedef struct {
    char* username;
    char* password;
    int uid;
    int gid;
    char* gecos;
    char* directory;
    char* shell;
} Passwd;
void print_passwd_struct(const Passwd *p) {
	if (p == NULL) return;

	printf("[User: %-15s] UID: %-5d GID: %-5d Shell: %s\n", 
		p->username ? p->username : "N/A", 
		p->uid, 
		p->gid, 
		p->shell ? p->shell : "N/A"
	);
	printf("  |- Directory: %s\n", p->directory ? p->directory : "N/A");
	printf("  |- Gecos:     %s\n", p->gecos ? p->gecos : "N/A");
	printf("------------------------------------------------------------------\n");
}
void split_tostruct(Passwd *passwd){
    char *running = readline;
    char *token;
    // 1. username
    passwd->username = strsep(&running, ":");

    // 2. password (遇到 :: 會回傳空字串 "")
    passwd->password = strsep(&running, ":");

    // 3. uid
    token = strsep(&running, ":");
    passwd->uid = token ? atoi(token) : 0;

    // 4. gid
    token = strsep(&running, ":");
    passwd->gid = token ? atoi(token) : 0;

    passwd->gecos = strsep(&running, ":");

    passwd->directory = strsep(&running, ":");

    passwd->shell = strsep(&running, ":");
    int i;
    for (i = 0; passwd->shell[i] != '\n'; ++i);
    passwd->shell[i] = '\0';
     
}
int read_passwd(char *username, Passwd *passwd){
    FILE *fp = fopen("/etc/passwd", "r");
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
        split_tostruct(passwd);
        //print_passwd_struct(&passwd);
        if (strcmp(passwd->username, username)== 0) 
            return 1;
    }

    fclose(fp);
    return 0;
}


