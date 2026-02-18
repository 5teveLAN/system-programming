#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#define BUFFER_SIZE 4096

int main(int argc, char* argv[]){
    char buffer[BUFFER_SIZE];
    int src_fd, dest_fd;
    ssize_t bytes_read, bytes_written;
    ssize_t file_size;
    off_t current_pos = 0;
    off_t data_pos = 0;
    off_t dest_current_pos = 0;
    off_t hole_pos = 0;
    off_t next_bytes_read = 0;
    void *src_ptr, *dest_ptr;


    if (argc != 3){
        perror("Usage: ./file_name [src] [dest]\n");
        exit(1); // EXIT_FAILTURE
    }

    src_fd = open(argv[1], O_RDONLY);
    dest_fd = open(argv[2], O_RDWR| O_CREAT | O_TRUNC, 0644);

    if (src_fd == -1 || dest_fd == -1){
        perror("open file error");
        exit(1);
    }

    file_size = lseek(src_fd, 0, SEEK_END);
    ftruncate(dest_fd, file_size);
    while(current_pos != file_size){
        data_pos = lseek(src_fd, current_pos, SEEK_DATA);
        if (data_pos == -1&& errno == ENXIO){
            ftruncate(dest_fd, file_size);
            break;
        }
        hole_pos = lseek(src_fd, data_pos, SEEK_HOLE);
        current_pos = lseek(src_fd, data_pos, SEEK_SET);

        next_bytes_read = hole_pos - current_pos > BUFFER_SIZE ?
                            BUFFER_SIZE : hole_pos - data_pos;

        
        //bytes_read = read(src_fd, buffer, next_bytes_read);
        src_ptr = mmap(NULL, next_bytes_read, PROT_READ, MAP_SHARED , src_fd , current_pos);
        if (!src_ptr){
            perror("failed to read");
            exit(1);}

        //bytes_written = write(dest_fd, buffer, next_bytes_read);
        dest_ptr = mmap(NULL, next_bytes_read, PROT_WRITE, MAP_SHARED , dest_fd , current_pos);

        if (!dest_ptr){
            perror("failed to write");
            exit(1);}

        memcpy(dest_ptr, src_ptr, next_bytes_read);
        current_pos += next_bytes_read;
    }
    
    close(dest_fd);
    close(src_fd);

}
