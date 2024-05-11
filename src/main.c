#include "atrcc.h"
#include <stdio.h>

char *user_input;
Token *token;

int main(int argc, char **argv) {
    if (argc != 2) {
        error("the number of arguments is wrong.");
    }

    user_input = argv[1];
    token = tokenize();
    Node *node = expr();

    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    gen(node);

    printf("  pop rax\n");
    printf("  ret\n");
}
