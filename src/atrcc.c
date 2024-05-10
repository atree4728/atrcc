#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "ERROR: the number of argument is wrong.\n");
        return 1;
    }

    char *p = argv[1];

    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");
    printf("  mov rax, %ld\n", strtol(p, &p, 10));

    while (*p) {
        if (*p == '+' || *p == '-') {
            const char *op = ((*p++ == '+') ? "add" : "sub");
            printf("  %s rax, %ld\n", op, strtol(p, &p, 10));
        } else {
            fprintf(stderr, "ERROR: unexpected charater: '%c'\n", *p);
            return 1;
        }
    }

    printf("  ret\n");
}
