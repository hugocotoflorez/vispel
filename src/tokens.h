#ifndef VTOKENS_H
#define VTOKENS_H

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

static void
report(char *format, ...)
{
        va_list args;
        va_start(args, format);
        if (isatty(STDERR_FILENO)) fprintf(stderr, "\033[31m");
        vfprintf(stderr, format, args);
        if (isatty(STDERR_FILENO)) fprintf(stderr, "\033[0m");
        va_end(args);
        FILE *f = fopen("log.txt", "a");
        if (f) {
                va_start(args, format);
                vfprintf(f, format, args);
                va_end(args);
                fclose(f);
        }
}

typedef enum {
        LEFT_PARENT,
        RIGHT_PARENT,
        LEFT_BRACE,
        RIGHT_BRACE,
        LEFT_BRACKET,
        RIGHT_BRACKET,
        COMMA,
        DOT,
        MINUS,
        PLUS,
        SEMICOLON,
        SLASH,
        STAR,
        BANG,
        BANG_EQUAL,
        EQUAL,
        EQUAL_EQUAL,
        GREATER,
        GREATER_EQUAL,
        LESS,
        LESS_EQUAL,
        IDENTIFIER,
        STRING,
        NUMBER,
        AND,
        CLASS,
        ELSE,
        FALSE,
        FUNCTION,
        VAR,
        FOR,
        IF,
        NIL,
        OR,
        EXTERN,
        RETURN,
        TRUE,
        WHILE,
        END_OF_FILE,
        BITWISE_AND,
        BITWISE_OR,
        BITWISE_XOR,
        BITWISE_NOT,
        PLUS_PLUS,
        LESS_LESS,
        SHIFT_LEFT,
        SHIFT_RIGHT,
        FUNC_INPUT,
        FUNC_OUTPUT,
        CHAR,
        UNKNOWN,
} vtoktype;

static const char *TOKEN_REPR[] = {
        [LEFT_PARENT] = "LEFT_PARENT",
        [RIGHT_PARENT] = "RIGHT_PARENT",
        [LEFT_BRACE] = "LEFT_BRACE",
        [RIGHT_BRACE] = "RIGHT_BRACE",
        [LEFT_BRACKET] = "LEFT_BRACKET",
        [RIGHT_BRACKET] = "RIGHT_BRACKET",
        [COMMA] = "COMMA",
        [DOT] = "DOT",
        [MINUS] = "MINUS",
        [PLUS] = "PLUS",
        [SEMICOLON] = "SEMICOLON",
        [SLASH] = "SLASH",
        [STAR] = "STAR",
        [BANG] = "BANG",
        [BANG_EQUAL] = "BANG_EQUAL",
        [EQUAL] = "EQUAL",
        [EQUAL_EQUAL] = "EQUAL_EQUAL",
        [GREATER] = "GREATER",
        [GREATER_EQUAL] = "GREATER_EQUAL",
        [LESS] = "LESS",
        [LESS_EQUAL] = "LESS_EQUAL",
        [IDENTIFIER] = "IDENTIFIER",
        [STRING] = "STRING",
        [NUMBER] = "NUMBER",
        [AND] = "AND",
        [CLASS] = "CLASS",
        [ELSE] = "ELSE",
        [FALSE] = "FALSE",
        [FUNCTION] = "FUNCTION",
        [VAR] = "VAR",
        [FOR] = "FOR",
        [IF] = "IF",
        [NIL] = "NIL",
        [OR] = "OR",
        [EXTERN] = "EXTERN",
        [RETURN] = "RETURN",
        [TRUE] = "TRUE",
        [WHILE] = "WHILE",
        [END_OF_FILE] = "END_OF_FILE",
        [BITWISE_AND] = "BITWISE_AND",
        [BITWISE_OR] = "BITWISE_OR",
        [BITWISE_XOR] = "BITWISE_XOR",
        [BITWISE_NOT] = "BITWISE_NOT",
        [PLUS_PLUS] = "PLUS_PLUS",
        [LESS_LESS] = "LESS_LESS",
        [SHIFT_LEFT] = "SHIFT_LEFT",
        [SHIFT_RIGHT] = "SHIFT_RIGHT",
        [FUNC_INPUT] = "FUNC_INPUT",
        [FUNC_OUTPUT] = "FUNC_OUTPUT",
        [CHAR] = "CHAR",
        [UNKNOWN] = "UNKNOWN",
};

typedef enum Valtype {
        TYPE_NUM,
        TYPE_STR,
} Valtype;

static const char *VALTYPE_REPR[] = {
        [TYPE_NUM] = "NUMBER",
        [TYPE_STR] = "STRING",
};


typedef struct vtok {
        vtoktype token;
        const char *lexeme;
        union {
                int num_literal;
                char *str_literal;
        };
        int line;
        intptr_t offset;
        /* Linked list stuff */
        struct vtok *next;
        struct vtok *prev;
} vtok;

typedef enum Exprtype {
        ASSIGNEXPR,
        BINEXPR,
        UNEXPR,
        CALLEXPR,
        LITEXPR,
        VAREXPR,
} Exprtype;

// clang-format off
typedef struct Expr {
        union {
                struct { struct Expr *value; vtok *name; } assignexpr;
                struct { struct Expr *rhs; struct Expr *lhs; vtok *op; } binexpr;
                struct { struct Expr *rhs; vtok *op; } unexpr;
                struct { struct Expr **args; int count; vtok *name; } callexpr;
                struct { struct Expr *value; vtok *name; } varexpr;
                struct { vtok *value; } litexpr;
        };
        Exprtype type;
        /* Linked list stuff */
        struct Expr *next;
        struct Expr *prev;
} Expr;
// clang-format on

static const char *EXPR_REPR[] = {
        [ASSIGNEXPR] = "Assign",
        [BINEXPR] = "Binary",
        [UNEXPR] = "Unary",
        [CALLEXPR] = "Call",
        [LITEXPR] = "Literal",
        [VAREXPR] = "Variable",
};

typedef enum {
        VARDECLSTMT,
        BLOCKSTMT,
        EXPRSTMT,
} Stmttype;

static const char *STMT_REPR[] = {
        [VARDECLSTMT] = "VARDECLSTMT",
        [BLOCKSTMT] = "BLOCKSTMT",
        [EXPRSTMT] = "EXPRSTMT",
};

typedef struct Stmt {
        union {
                struct {
                        vtok *name;
                        Expr *value;
                } vardecl;
                struct {
                        struct Stmt *body;
                } block;
                struct {
                        Expr *body;
                } expr;
        };
        Stmttype type;
        struct Stmt *prev;
        struct Stmt *next;
} Stmt;

extern vtok *head_token;
extern Stmt *head_stmt;

/* Get source code as a string and return a linked list of tokens */
void lex_analize(char *source);

/* Print the list of tokens */
void print_tokens();
void print_literal(vtok *tok);

void tok_parse();
void print_ast();

#endif
