#include "shell.h"
#include "stdint.h"
#include "fs.h"
#include "file.h"
#include "syscall.h"
#include "stdio.h"
#include "global.h"
#include "debug.h"
#include "string.h"

#define MAX_ARG_NR 16

static char cmd_line[MAX_PATH_LEN] = {0};
char final_path[MAX_PATH_LEN] = {0};
char cwd_cache[64] = {0};
void print_prompt(void) {
    printf("[csw@localhost %s]$", cwd_cache);
}

static void readline(char* buf, int32_t count) {
    ASSERT(buf != NULL && count > 0);
    char* pos = buf;
    while (read(stdin_no, pos, 1) != -1 && (pos - buf) < count) {
        printf("fuck you all");
        switch(*pos) {
            case '\n':
            case '\r':
                *pos = 0;
                putchar('\n');
                return;
            case '\b':
                if (buf[0] != '\b') {
                    --pos;
                    putchar('\b');
                }
                break;
            case 'l' - 'a':
                *pos = 0;
                clear();
                print_prompt();
                printf("%s", buf);
                break;
            case 'u' - 'a':
                while (buf != pos) {
                    putchar('\b');
                    *(pos--) = 0;   
                }
                break;
            default:
                putchar(*pos);
                pos++;
        }
    }
    printf("readline: can't find enter_key in the cmd_line, max num of char is 128\n");
}

static int32_t cmd_parse(char* cmd_str, char** argv, char token) {
    ASSERT("cmd_str != NULL");
    int32_t arg_idx = 0;
    while (arg_idx < MAX_ARG_NR) {
        argv[arg_idx] = NULL;   
        arg_idx++;
    }
    char* next = cmd_str;
    int32_t argc = 0;
    while (*next) {
        while (*next == token) {
            next++;
        }
        if (*next == 0) {
            break;
        }
        argv[argc] = next;
        while (*next && *next != token) {
            next++;
        }
        if (*next) {
            *next++ = 0;
        }
        if (argc > MAX_ARG_NR) {
            return -1;
        }
        argc++;
    }
    return argc;
}

char* argv[MAX_ARG_NR];
int32_t argc = -1;

void my_shell(void) {
    printf("my shell start");
    cwd_cache[0] = '/';
    while (1) {
        print_prompt();
        memset(final_path, 0, MAX_PATH_LEN);
        memset(cmd_line, 0, MAX_PATH_LEN);
        readline(cmd_line, MAX_PATH_LEN);
        if (cmd_line[0] == 0) {
            continue;
        }
        argc = -1;
        argc = cmd_parse(cmd_line, argv, ' ');
        if (argc == -1) {
            printf("num of arguments exceed %d\n", MAX_ARG_NR);
            continue;
        }
        int32_t arg_idx = 0;
        while (arg_idx < argc) {
            printf("%s ", argv[arg_idx]);
            arg_idx++;
        }
        printf("\n");
    }
    printf("my_shell: should not be here");
}
