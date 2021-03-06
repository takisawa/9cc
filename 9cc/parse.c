#include "9cc.h"

static Node *stmt();
static Node *assign();
static Node *expr();
static Node *equality();
static Node *relational();
static Node *add();
static Node *mul();
static Node *unary();
static Node *primary();

Node *new_node(NodeKind kind) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    return node;
}

Node *new_binary(NodeKind kind, Node *lhs, Node *rhs) {
    Node *node = new_node(kind);
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_num(int val) {
    Node *node = new_node(ND_NUM);
    node->val = val;
    return node;
}

Node *code[100];

// program = stmt*
void program() {
    int i = 0;
    while (!at_eof()) {
        // printf("[debug]program\n");
        code[i++] = stmt();
    }
    code[i] = NULL;
}

// stmt = expr ";"
Node *stmt() {
    // printf("[debug] stmt0\n");
    Node *node = expr();
    // printf("[debug] stmt1\n");
    expect(";");
    // printf("[debug] stmt2\n");
    return node;
}

// expr = assign
Node *expr() {
    return assign();
}

// assign = equality ("=" assign)?
Node *assign() {
    // printf("[debug] assign0\n");
    Node *node = equality();
    if (consume("=")) {
        // printf("[debug] assign1 before new_binary\n");
        node = new_binary(ND_ASSIGN, node, assign());
    }
    return node;
}

// equality = relational ("==" relational | "!=" relational)*
Node *equality() {
    Node *node = relational();

    for (;;) {
        if (consume("=="))
            node = new_binary(ND_EQ, node, relational());
        else if (consume("!="))
            node = new_binary(ND_NE, node, relational());
        else
            return node;
    }
}

// relational = add ("<" add | "<=" add | ">" add | ">=" add)*
Node *relational() {
    Node *node = add();

    for (;;) {
        if (consume("<"))
            node = new_binary(ND_LT, node, add());
        else if (consume("<="))
            node = new_binary(ND_LE, node, add());
        else if (consume(">"))
            node = new_binary(ND_LT, add(), node);
        else if (consume(">="))
            node = new_binary(ND_LE, add(), node);
        else
            return node;
    }
}

// add = mul ("+" mul | "-" mul)*
Node *add() {
    Node *node = mul();

    for (;;) {
        if (consume("+"))
            node = new_binary(ND_ADD, node, mul());
        else if (consume("-"))
            node = new_binary(ND_SUB, node, mul());
        else
            return node;
    }
}

// mul = unary ("*" unary | "/" unary)*
Node *mul() {
    Node *node = unary();

    for (;;) {
        if (consume("*"))
            node = new_binary(ND_MUL, node, unary());
        else if (consume("/"))
            node = new_binary(ND_DIV, node, unary());
        else
            return node;
    }
}

// unary = ("+" | "-")? unary
//       | primary
Node *unary() {
    if (consume("+"))
      return unary();
    if (consume("-"))
      return new_binary(ND_SUB, new_num(0), unary());
    return primary();
}

// primary = num | ident | "(" expr ")"
Node *primary() {
    // printf("[debug] primary0\n");
    Token *tok = consume_ident();
    // printf("[debug] primary1: tok->str[0]:`%c`\n", tok->str[0]);
    if (tok) {
        Node *node = calloc(1, sizeof(Node));
        node->kind = ND_LVAR;
        // printf("[debug] primary2: tok->str[0]:`%c`\n", tok->str[0]);
        node->offset = (tok->str[0] - 'a' + 1) * 8;
        return node;
    }

    if (consume("(")) {
        Node *node = expr();
        expect(")");
        return node;
    }

    return new_num(expect_number());
}
