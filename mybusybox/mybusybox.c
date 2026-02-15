#include <stdio.h>
#include <libgen.h>

int main(int argc, char* argv[]){
    char* cmd = basename(argv[0]);
    printf("%s", argv[0]);
    printf("%s", argv[1]);
// 1. 處理 ls (不帶參數，僅執行 ls)
    if (strcmp(cmd, "ls") == 0) {
        // 即使輸入 ./ls -la，這裡也只會執行 "ls"
        return system("ls");

    } 
    // 2. 處理 cp (要求來源與目的地，argc 必須為 3: [./cp, src, dest])
    else if (strcmp(cmd, "cp") == 0) {
        if (argc == 3) {
            sprintf(command_buf, "cp %s %s", argv[1], argv[2]);
            return system(command_buf);
        } else {
            printf("Usage: cp [source] [destination]\n");
            return 1;
        }
    }
    // 3. 處理 cat (argc 為 2: [./cat, file])
    else if (strcmp(cmd, "cat") == 0) {
        if (argc >= 2) {
            sprintf(command_buf, "cat %s", argv[1]);
            return system(command_buf);
        } else {
            printf("Usage: cat [filename]\n");
            return 1;
        }
    }
    // 4. 處理 chown (argc 為 3: [./chown, owner, file])
    else if (strcmp(cmd, "chown") == 0) {
        if (argc == 3) {
            sprintf(command_buf, "chown %s %s", argv[1], argv[2]);
            return system(command_buf);
        } else {
            printf("Usage: chown [owner] [file]\n");
            return 1;
        }
    }
    // 5. 本體或其他
    else {
        printf("myBusybox: command not found\n");
        return 1;
    }    
}
