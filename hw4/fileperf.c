#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]){
    if (argc != 4){
        //perror("Usage: ./file_name [src] [dest]\n");
        fprintf(stderr,"Usage: ./%s [src] [dest] [buf_size]\n", argv[0]);
        exit(1); // EXIT_FAILTURE
    }
    char* src_file = argv[1];
    char* dest_file = argv[2];
    int buffer_size = atoi(argv[3]);
    FILE* src_fp = fopen(src_file, "r");
    if (!src_fp){
        perror("open src fail\n");
        exit(1);}
    FILE* dest_fp = fopen(dest_file, "w");
    if (!dest_fp){
        perror("open dest fail\n");
        exit(1);}
    char *vbuf_src, *vbuf_dest;
    if (buffer_size == 0){
        vbuf_src = (char*)malloc(buffer_size);
        vbuf_dest = (char*)malloc(buffer_size);
        setvbuf(src_fp, vbuf_src, _IONBF, 1024);
        setvbuf(dest_fp, vbuf_dest, _IONBF, 1024);}
    else if (buffer_size == -1){
        vbuf_src = (char*)malloc(buffer_size);
        vbuf_dest = (char*)malloc(buffer_size);
        setvbuf(src_fp, vbuf_src, _IOLBF, 1024);
        setvbuf(dest_fp, vbuf_dest, _IOLBF, 1024);}
    else{
        vbuf_src = (char*)malloc(buffer_size);
        vbuf_dest = (char*)malloc(buffer_size);
        setvbuf(src_fp, vbuf_src, _IOFBF, buffer_size);
        setvbuf(dest_fp, vbuf_dest, _IOFBF, buffer_size);}

    char readchars[81];
    int space_idx;
    int char_cnt;
    int isEOF = 0;
    long pos;
    // readlines
    while (!isEOF){
        char_cnt = 0;
        space_idx = -1;
        //read 81 char to readchars[]
        pos = ftell(src_fp);
        char_cnt += fread(readchars, 1, 81, src_fp);
        isEOF = !(char_cnt == 81);
        //If not eof: find the last space and record its index
        if (!isEOF){
            //遇到 space or linefeed就換行
            //1. 找第一個linefeed
            for(int i = 0; i <= 80; i++)
                if (readchars[i]=='\n'){
                    space_idx = i;
                    break;
                }
            //2. 找最後一個space
            if (space_idx==-1)
            for(int i = 80; i >= 0; i--)
                if (readchars[i]==' '){
                    readchars[i]='\n';
                    space_idx = i;
                    break;
                }
            //3. 此81字元都沒空格
            if (space_idx==-1)
                space_idx = 79;
        }

        if (!isEOF)
            //write readchars[0-index] to dest file
            fwrite(readchars, space_idx+1, 1, dest_fp);
        else
            // last line
            fwrite(readchars, char_cnt, 1, dest_fp);
        //set current_ptr = index
        fseek(src_fp, pos+space_idx+1, SEEK_SET);
    }

    fclose(src_fp);
    fclose(dest_fp);
}
