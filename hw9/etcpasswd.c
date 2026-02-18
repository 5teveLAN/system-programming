#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 4096
char readline[BUFFER_SIZE];
typedef struct Passwd{
    char* username;
    char* password;
    int uid;
    int gid;
    char* gecos;
    char* directory;
    char* shell;

    int username_len;
    int password_len;
    int gecos_len;
    int directory_len;
    int shell_len;
};

void split_tostruct(){
    char *token;
    token = strtok(readline, ":");
    while (strtok(NULL, ":")){
        
    }

}
void read_passwd(){
    FILE *fp = fopen("/etc/passwd", "r");
    long filesize;
    Passwd passwd[1000];
    int linecount = 0;

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
        split_tostruct();
    }

    fclose(fp);
}


int main(){
    read_passwd();

}

