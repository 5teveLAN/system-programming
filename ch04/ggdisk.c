#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char* argv[]){
    char* filename = "ggdisk.img";
    if (argc != 2){
        perror("Usage: filename filesize");
        exit(1);}

    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd==-1){
        perror("fail to open file");
        exit(1);}

    
    lseek(fd, 0, SEEK_SET);
    write(fd,"gg",sizeof("gg"));
    ftruncate(fd, atoi(argv[1]));       

    close(fd);


      

}
