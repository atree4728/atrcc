#include "atrcc.h"
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

bool consume(char *op) {
    if (token->kind != TK_RESERVED || strlen(op) != token->len ||
        memcmp(token->str, op, token->len) != 0)
        return false;
    token = token->next;
    return true;
}

void expect(char *op) {
    if (token->kind != TK_RESERVED || strlen(op) != token->len ||
        memcmp(token->str, op, token->len) != 0)
        error_at(token->str, "expected: '%s'.", op);
    token = token->next;
}

int expect_number() {
    if (token->kind != TK_NUM)
        error_at(token->str, "expected: num.");
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof() { return token->kind == TK_EOF; }

Token *new_token(TokenKind kind, Token *cur, char *str, unsigned len) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
    cur->next = tok;
    return tok;
}

Token *tokenize() {
    char *p = user_input;
    Token head;
    head.next = NULL;
    Token *cur = &head;

    char reserved[][3] = {"+",  "-", "*",  "/", "(",  ")",
                          "<=", "<", ">=", ">", "==", "!="};

    while (*p) {
        if (isspace(*p)) {
            p++;
        } else if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p, 0);
            cur->val = strtol(p, &p, 10);
            cur->len = (int)(log10(cur->val)) + 1;
        } else {
            for (unsigned i = 0; i < sizeof(reserved) / sizeof(reserved[0]);
                 ++i) {
                if (starts_with(p, reserved[i])) {
                    unsigned len = strlen(reserved[i]);
                    cur = new_token(TK_RESERVED, cur, p, len);
                    p += len;
                    goto OUT;
                }
            }
            error_at(p, "fail to tokenize.");
        OUT: {}
        }
    }

    new_token(TK_EOF, cur, p, 0);
    return head.next;
}

Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_node_num(int val) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}

/* EBNF:
 *   expr       = equality
 *   equality   = relational (("==" | "!=") relational)*
 *   relational = add (("<" | "<=" | ">" | ">=") add)*
 *   add        = mul (("+" | "-") mul)*
 *   mul        = unary (("*" | "/") unary)*
 *   unary      = ("+" | "-")? primary
 *   primary    = num | "(" expr ")"
 */

Node *expr() { return equality(); }

Node *equality() {
    Node *node = relational();
    while (true) {
        if (consume("=="))
            node = new_node(ND_EQ, node, relational());
        else if (consume("!="))
            node = new_node(ND_NEQ, node, relational());
        else
            return node;
    }
}

Node *relational() {
    Node *node = add();
    while (true) {
        if (consume("<"))
            node = new_node(ND_LT, node, add());
        else if (consume("<="))
            node = new_node(ND_LE, node, add());
        else if (consume(">"))
            node = new_node(ND_LT, add(), node);
        else if (consume(">="))
            node = new_node(ND_LE, add(), node);
        else
            return node;
    }
}

Node *add() {
    Node *node = mul();
    while (true) {
        if (consume("+"))
            node = new_node(ND_ADD, node, mul());
        else if (consume("-"))
            node = new_node(ND_SUB, node, mul());
        else
            return node;
    }
}

Node *mul() {
    Node *node = unary();
    while (true) {
        if (consume("*"))
            node = new_node(ND_MUL, node, unary());
        else if (consume("/"))
            node = new_node(ND_DIV, node, unary());
        else
            return node;
    }
}

Node *unary() {
    if (consume("+"))
        return primary();
    if (consume("-"))
        return new_node(ND_SUB, new_node_num(0), primary());
    return primary();
}

Node *primary() {
    if (consume("(")) {
        Node *node = expr();
        expect(")");
        return node;
    }
    return new_node_num(expect_number());
}
