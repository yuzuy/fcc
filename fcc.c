#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fcc.h"

void error_at(char *loc, char *fmt, ...);
bool consume(char op);
void expect(char op);
int expect_number();
bool at_eof();
Token *new_token(TokenKind kind, Token *cur, char *str);
Token *tokenize(char *p);
Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);
Node *expr();
Node *mul();
Node *unary();
Node *primary();
void gen(Node *node);

Token *token;
char *user_input;

void error_at(char *loc, char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, "");
    fprintf(stderr, "^ ");
    fprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

bool consume(char op) {
    if (token->kind != TK_RESERVED || token->str[0] != op)
        return false;
    token = token->next;
    return true;
}

void expect(char op) {
    if (token->kind != TK_RESERVED || token->str[0] != op)
        error_at(token->str, "current token not %c", op);
    token = token->next;
}

int expect_number() {
    if (token->kind != TK_NUM)
        error_at(token->str, "current token not number");
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof() {
    return token->kind == TK_EOF;
}

Token *new_token(TokenKind kind, Token *cur, char *str) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    cur->next = tok;
    return tok;
}

Token *tokenize(char *p) {
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (strchr("+-*/()", *p)) {
            cur = new_token(TK_RESERVED, cur, p++);
            continue;
        }

        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p);
            cur->val = (int) strtol(p, &p, 10);
            continue;
        }

        error_at(p, "cannot tokenize");
    }

    new_token(TK_EOF, cur, p);
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

Node *expr() {
    Node *node = mul();

    while (true) {
        if (consume('+'))
            node = new_node(ND_ADD, node, mul());
        else if (consume('-'))
            node = new_node(ND_SUB, node, mul());
        else
            return node;
    }
}

Node *mul() {
    Node *node = unary();

    while (true) {
        if (consume('*'))
            node = new_node(ND_MUL, node, unary());
        else if (consume('/'))
            node = new_node(ND_DIV, node, unary());
        else
            return node;
    }
}

Node *unary() {
    if (consume('-'))
        return new_node(ND_SUB, new_node_num(0), unary());
    return primary();
}

Node *primary() {
    if (consume('(')) {
        Node *node = expr();
        expect(')');
        return node;
    }

    return new_node_num(expect_number());
}

void gen(Node *node) {
    if (node->kind == ND_NUM) {
        printf("  push %d\n", node->val);
        return;
    }

    gen(node->lhs);
    gen(node->rhs);

    printf("  pop rdi\n");
    printf("  pop rax\n");

    switch (node->kind) {
        case ND_ADD:
            printf("  add rax, rdi\n");
            break;
        case ND_SUB:
            printf("  sub rax, rdi\n");
            break;
        case ND_MUL:
            printf("  imul rax, rdi\n");
            break;
        case ND_DIV:
            printf("  cqo\n");
            printf("  idiv rdi\n");
            break;
    }

    printf("  push rax\n");
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "length of arguments wrong");
        return 1;
    }

    user_input = argv[1];
    token = tokenize(user_input);
    Node *node = expr();

    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    gen(node);

    printf("  pop rax\n");
    printf("  ret\n");

    return 0;
}
