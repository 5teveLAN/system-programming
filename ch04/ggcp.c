#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define BUFFER_SIZE 4096

int main(int argc, char* argv[]){
    char buffer[BUFFER_SIZE];
    int src_fd, dest_fd;
    ssize_t bytes_read, bytes_written;


    if (argc != 3){
        perror("Usage: ./file_name [src] [dest]\n");
        exit(1); // EXIT_FAILTURE
    }

    src_fd = open(argv[1], O_RDONLY);
    dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (src_fd == -1 || dest_fd == -1){
        perror("open file error");
        exit(1);
    }

    do{
        bytes_read = read(src_fd, buffer, BUFFER_SIZE);
        if (bytes_read == -1){
            perror("failed to read");
            exit(1);}

        bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written == -1){
            perror("failed to write");
            exit(1);}

    }
    while (bytes_read);
    
    close(dest_fd);
    close(src_fd);

}
