#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
    if (argc != 2)
        exit(1);
    const char* path = argv[1];
    DIR *dir = opendir(path);
    struct dirent *dentry;

    char types[100] = {0};
    long totalSize = 0;
    long filesize;
    char abspath[4096];
    struct stat statbuf;
    while (dentry = readdir(dir)){
        types[dentry->d_type]=1;
        //printf("%20s%10d%10d\n", dentry->d_name, dentry->d_type,dentry->d_reclen);
        if (dentry->d_type==DT_REG){
            strncpy(abspath, path, sizeof(path));
            strcat(abspath, dentry->d_name);
            stat(abspath, &statbuf);
            filesize = statbuf.st_size; 
            totalSize+=filesize;}

    }
    printf("total size: %ld\n", totalSize);
    for (int i=0;i<100;++i)
        if (types[i]==1)
            switch(i){
            case DT_REG:
                printf("-");
                break;
            case DT_DIR:
                printf("d");
                break;
            case DT_LNK:
                printf("l\n");
                break;
            }

    closedir(dir);
}
