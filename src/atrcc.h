#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>

// parse.c
typedef enum {
    TK_RESERVED,
    TK_NUM,
    TK_EOF,
} TokenKind;

typedef struct Token Token;
struct Token {
    TokenKind kind;
    Token *next;
    int val;
    char *str;
    unsigned len;
};

typedef enum {
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_LT,
    ND_LE,
    ND_EQ,
    ND_NEQ,
    ND_NUM
} NodeKind;

typedef struct Node Node;
struct Node {
    NodeKind kind;
    Node *lhs;
    Node *rhs;
    int val;
};
bool consume(char *op);
void expect(char *op);
int expect_number();
bool at_eof();

Token *new_token(TokenKind kind, Token *cur, char *str, unsigned len);
Token *tokenize();
Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);

Node *expr();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();

extern Token *token;
extern char *user_input;

// codegen.c
void gen(Node *node);

// utils.c
bool starts_with(char *s, char *t);
void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
