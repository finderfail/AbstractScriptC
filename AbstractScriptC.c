#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_MODULO,
    TOKEN_ASSIGN,
    TOKEN_EQUALS,
    TOKEN_NOT_EQUALS,
    TOKEN_GT,
    TOKEN_GTE,
    TOKEN_LT,
    TOKEN_LTE,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_SEMICOLON,
    TOKEN_COMMA,
    TOKEN_LET,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_FUNCTION,
    TOKEN_RETURN,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_PRINT,
    TOKEN_IMPORT,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    union {
        double number_value;
        char* string_value;
    } value;
} Token;

typedef struct {
    char* input;
    int position;
    int input_length;
    char current_char;
} Lexer;

// Forward declarations for AST node structures
typedef struct ASTNode ASTNode;

typedef enum {
    NODE_PROGRAM,
    NODE_BLOCK_STATEMENT,
    NODE_VARIABLE_DECLARATION,
    NODE_ASSIGNMENT_EXPRESSION,
    NODE_BINARY_EXPRESSION,
    NODE_LOGICAL_EXPRESSION,
    NODE_LITERAL,
    NODE_IDENTIFIER,
    NODE_IF_STATEMENT,
    NODE_WHILE_STATEMENT,
    NODE_FUNCTION_DECLARATION,
    NODE_CALL_EXPRESSION,
    NODE_RETURN_STATEMENT,
    NODE_PRINT_STATEMENT,
    NODE_IMPORT_STATEMENT
} NodeType;

struct ASTNode {
    NodeType type;
    union {
        struct {
            ASTNode** body;
            int body_length;
        } program;

        struct {
            ASTNode** body;
            int body_length;
        } block_statement;

        struct {
            char* name;
            ASTNode* value;
        } variable_declaration;

        struct {
            char* name;
            ASTNode* value;
        } assignment_expression;

        struct {
            char* operator;
            ASTNode* left;
            ASTNode* right;
        } binary_expression;

        struct {
            char* operator;
            ASTNode* left;
            ASTNode* right;
        } logical_expression;

        struct {
            union {
                double number;
                char* string;
                bool boolean;
            } value;
            char value_type; // 'n' for number, 's' for string, 'b' for boolean
        } literal;

        struct {
            char* name;
        } identifier;

        struct {
            ASTNode* test;
            ASTNode* consequent;
            ASTNode* alternate;
        } if_statement;

        struct {
            ASTNode* test;
            ASTNode* body;
        } while_statement;

        struct {
            char* name;
            char** params;
            int params_length;
            ASTNode* body;
        } function_declaration;

        struct {
            char* name;
            ASTNode** arguments;
            int arguments_length;
        } call_expression;

        struct {
            ASTNode* argument;
        } return_statement;

        struct {
            ASTNode* argument;
        } print_statement;


        struct {
            char* path;
        } import_statement;
    } data;
};

typedef struct {
    Token* tokens;
    int position;
    int tokens_length;
    Token current_token;
} Parser;

typedef enum {
    VALUE_NUMBER,
    VALUE_STRING,
    VALUE_BOOLEAN,
    VALUE_FUNCTION,
    VALUE_NULL
} ValueType;

typedef struct Scope Scope;

typedef struct {
    ValueType type;
    union {
        double number;
        char* string;
        bool boolean;
        struct {
            char* name;
            char** params;
            int params_length;
            ASTNode* body;
            Scope** closure;
            int closure_length;
        } function;
    } data;
} Value;

struct Scope {
    char** names;
    Value* values;
    int length;
    int capacity;
};

typedef struct {
    Scope** scope_stack;
    int scope_stack_length;
    int scope_stack_capacity;
    Value return_value;
    bool has_return;
    char* base_dir;
} Interpreter;

char** imported_files = NULL;
int imported_files_length = 0;
int imported_files_capacity = 0;

Lexer* create_lexer(char* input);
void advance_lexer(Lexer* lexer);
void skip_whitespace(Lexer* lexer);
Token get_number_token(Lexer* lexer);
Token get_identifier_token(Lexer* lexer);
Token get_string_token(Lexer* lexer);
Token get_next_token(Lexer* lexer);
Token* tokenize(Lexer* lexer, int* token_count);
void free_lexer(Lexer* lexer);

Parser* create_parser(Token* tokens, int tokens_length);
void advance_parser(Parser* parser);
Token eat(Parser* parser, TokenType type);
ASTNode* parse_program(Parser* parser);
ASTNode* parse_statement(Parser* parser);
ASTNode* parse_block_statement(Parser* parser);
ASTNode* parse_variable_declaration(Parser* parser);
ASTNode* parse_if_statement(Parser* parser);
ASTNode* parse_while_statement(Parser* parser);
ASTNode* parse_function_declaration(Parser* parser);
ASTNode* parse_function_call(Parser* parser, char* name);
ASTNode* parse_return_statement(Parser* parser);
ASTNode* parse_print_statement(Parser* parser);
ASTNode* parse_import_statement(Parser* parser);
ASTNode* parse_expression(Parser* parser);
ASTNode* parse_logical_or(Parser* parser);
ASTNode* parse_logical_and(Parser* parser);
ASTNode* parse_equality(Parser* parser);
ASTNode* parse_comparison(Parser* parser);
ASTNode* parse_addition(Parser* parser);
ASTNode* parse_multiplication(Parser* parser);
ASTNode* parse_primary(Parser* parser);
ASTNode* parse(Parser* parser);
void free_parser(Parser* parser);
void free_ast_node(ASTNode* node);

Interpreter* create_interpreter(void);
Scope* create_scope(void);
void push_scope(Interpreter* interpreter, Scope* scope);
Scope* pop_scope(Interpreter* interpreter);
Scope* get_current_scope(Interpreter* interpreter);
void define_variable(Scope* scope, char* name, Value value);
Value* lookup_variable(Interpreter* interpreter, char* name);
Value evaluate(Interpreter* interpreter, ASTNode* node);
Value evaluate_program(Interpreter* interpreter, ASTNode* node);
Value evaluate_block_statement(Interpreter* interpreter, ASTNode* node);
Value evaluate_variable_declaration(Interpreter* interpreter, ASTNode* node);
Value evaluate_assignment_expression(Interpreter* interpreter, ASTNode* node);
Value evaluate_binary_expression(Interpreter* interpreter, ASTNode* node);
Value evaluate_logical_expression(Interpreter* interpreter, ASTNode* node);
Value evaluate_literal(Interpreter* interpreter, ASTNode* node);
Value evaluate_identifier(Interpreter* interpreter, ASTNode* node);
Value evaluate_if_statement(Interpreter* interpreter, ASTNode* node);
Value evaluate_while_statement(Interpreter* interpreter, ASTNode* node);
Value evaluate_function_declaration(Interpreter* interpreter, ASTNode* node);
Value evaluate_call_expression(Interpreter* interpreter, ASTNode* node);
Value evaluate_return_statement(Interpreter* interpreter, ASTNode* node);
Value evaluate_print_statement(Interpreter* interpreter, ASTNode* node);
Value evaluate_import_statement(Interpreter* interpreter, ASTNode* node);
void free_interpreter(Interpreter* interpreter);
void free_scope(Scope* scope);
void free_value(Value value);

Value process_import(char* code, Scope* global_scope, char* base_dir);
Value run_interpreter(char* code, bool is_main_file);

bool is_keyword(char* identifier);
char* read_file(const char* filename);
void add_imported_file(char* filename);
bool is_file_imported(char* filename);
void clear_imported_files(void);
char* strdup(const char* str);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename.as>\n", argv[0]);
        return 1;
    }
    char* ithink = "-i";
    if (argv[1] == ithink) {
        printf("AbstractScript interpretator, proted to C");
        return 1;
    }

    if (argc == 2) {
        char* filename = argv[1];
        char* code = read_file(filename);

        if (code == NULL) {
            printf("Error: Could not read file '%s'\n", filename);
            return 1;
        }

        printf("Running %s...\n\n", filename);
        run_interpreter(code, true);

        free(code);
        return 0;
    }
}

// String duplication (not available in all C standard libraries)
char* strdup(const char* str) {
    size_t len = strlen(str) + 1;
    char* new_str = (char*)malloc(len);
    if (new_str) {
        memcpy(new_str, str, len);
    }
    return new_str;
}

// Lexer implementation
Lexer* create_lexer(char* input) {
    Lexer* lexer = (Lexer*)malloc(sizeof(Lexer));
    if (!lexer) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    lexer->input = input;
    lexer->position = 0;
    lexer->input_length = strlen(input);
    lexer->current_char = (lexer->input_length > 0) ? input[0] : '\0';
    return lexer;
}

void advance_lexer(Lexer* lexer) {
    lexer->position++;
    if (lexer->position < lexer->input_length) {
        lexer->current_char = lexer->input[lexer->position];
    }
    else {
        lexer->current_char = '\0';
    }
}

void skip_whitespace(Lexer* lexer) {
    while (lexer->current_char && isspace(lexer->current_char)) {
        advance_lexer(lexer);
    }
}

Token get_number_token(Lexer* lexer) {
    Token token;
    token.type = TOKEN_NUMBER;

    char* number_str = (char*)malloc(64);
    if (!number_str) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    int i = 0;

    // Get integer part
    while (lexer->current_char && isdigit(lexer->current_char)) {
        number_str[i++] = lexer->current_char;
        advance_lexer(lexer);
    }

    // Get decimal part if exists
    if (lexer->current_char == '.') {
        number_str[i++] = lexer->current_char;
        advance_lexer(lexer);

        while (lexer->current_char && isdigit(lexer->current_char)) {
            number_str[i++] = lexer->current_char;
            advance_lexer(lexer);
        }
    }

    number_str[i] = '\0';
    token.value.number_value = atof(number_str);
    free(number_str);

    return token;
}

Token get_identifier_token(Lexer* lexer) {
    Token token;

    char* identifier = (char*)malloc(256);
    if (!identifier) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    int i = 0;

    while (lexer->current_char && (isalnum(lexer->current_char) || lexer->current_char == '_')) {
        identifier[i++] = lexer->current_char;
        advance_lexer(lexer);
    }

    identifier[i] = '\0';

    // Check if it's a keyword
    if (strcmp(identifier, "let") == 0) token.type = TOKEN_LET;
    else if (strcmp(identifier, "if") == 0) token.type = TOKEN_IF;
    else if (strcmp(identifier, "else") == 0) token.type = TOKEN_ELSE;
    else if (strcmp(identifier, "while") == 0) token.type = TOKEN_WHILE;
    else if (strcmp(identifier, "function") == 0) token.type = TOKEN_FUNCTION;
    else if (strcmp(identifier, "return") == 0) token.type = TOKEN_RETURN;
    else if (strcmp(identifier, "true") == 0) token.type = TOKEN_TRUE;
    else if (strcmp(identifier, "false") == 0) token.type = TOKEN_FALSE;
    else if (strcmp(identifier, "print") == 0) token.type = TOKEN_PRINT;
    else if (strcmp(identifier, "import") == 0) token.type = TOKEN_IMPORT;
    else {
        token.type = TOKEN_IDENTIFIER;
        token.value.string_value = strdup(identifier);
    }

    free(identifier);
    return token;
}

Token get_string_token(Lexer* lexer) {
    Token token;
    token.type = TOKEN_STRING;

    advance_lexer(lexer); // Skip opening quote

    char* string = (char*)malloc(1024);
    if (!string) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    int i = 0;

    while (lexer->current_char && lexer->current_char != '"') {
        string[i++] = lexer->current_char;
        advance_lexer(lexer);
    }

    string[i] = '\0';
    token.value.string_value = strdup(string);

    advance_lexer(lexer); // Skip closing quote
    free(string);

    return token;
}

Token get_next_token(Lexer* lexer) {
    Token token;

    while (lexer->current_char) {
        if (isspace(lexer->current_char)) {
            skip_whitespace(lexer);
            continue;
        }

        if (isdigit(lexer->current_char)) {
            return get_number_token(lexer);
        }

        if (isalpha(lexer->current_char) || lexer->current_char == '_') {
            return get_identifier_token(lexer);
        }

        if (lexer->current_char == '"') {
            return get_string_token(lexer);
        }

        if (lexer->current_char == '+') {
            token.type = TOKEN_PLUS;
            advance_lexer(lexer);
            return token;
        }

        if (lexer->current_char == '-') {
            token.type = TOKEN_MINUS;
            advance_lexer(lexer);
            return token;
        }

        if (lexer->current_char == '*') {
            token.type = TOKEN_MULTIPLY;
            advance_lexer(lexer);
            return token;
        }

        if (lexer->current_char == '/') {
            advance_lexer(lexer);

            // Check for comments
            if (lexer->current_char == '/') {
                // Skip single-line comment
                while (lexer->current_char && lexer->current_char != '\n') {
                    advance_lexer(lexer);
                }
                continue;
            }

            token.type = TOKEN_DIVIDE;
            return token;
        }

        if (lexer->current_char == '=') {
            advance_lexer(lexer);

            if (lexer->current_char == '=') {
                advance_lexer(lexer);
                token.type = TOKEN_EQUALS;
            }
            else {
                token.type = TOKEN_ASSIGN;
            }

            return token;
        }

        if (lexer->current_char == '>') {
            advance_lexer(lexer);

            if (lexer->current_char == '=') {
                advance_lexer(lexer);
                token.type = TOKEN_GTE;
            }
            else {
                token.type = TOKEN_GT;
            }

            return token;
        }

        if (lexer->current_char == '<') {
            advance_lexer(lexer);

            if (lexer->current_char == '=') {
                advance_lexer(lexer);
                token.type = TOKEN_LTE;
            }
            else {
                token.type = TOKEN_LT;
            }

            return token;
        }

        if (lexer->current_char == '!') {
            advance_lexer(lexer);

            if (lexer->current_char == '=') {
                advance_lexer(lexer);
                token.type = TOKEN_NOT_EQUALS;
                return token;
            }

            fprintf(stderr, "Unexpected character after '!'\n");
            exit(1);
        }

        if (lexer->current_char == '&' && lexer->position + 1 < lexer->input_length &&
            lexer->input[lexer->position + 1] == '&') {
            advance_lexer(lexer);
            advance_lexer(lexer);
            token.type = TOKEN_AND;
            return token;
        }

        if (lexer->current_char == '|' && lexer->position + 1 < lexer->input_length &&
            lexer->input[lexer->position + 1] == '|') {
            advance_lexer(lexer);
            advance_lexer(lexer);
            token.type = TOKEN_OR;
            return token;
        }

        if (lexer->current_char == '(') {
            token.type = TOKEN_LPAREN;
            advance_lexer(lexer);
            return token;
        }

        if (lexer->current_char == ')') {
            token.type = TOKEN_RPAREN;
            advance_lexer(lexer);
            return token;
        }

        if (lexer->current_char == '{') {
            token.type = TOKEN_LBRACE;
            advance_lexer(lexer);
            return token;
        }

        if (lexer->current_char == '}') {
            token.type = TOKEN_RBRACE;
            advance_lexer(lexer);
            return token;
        }

        if (lexer->current_char == ';') {
            token.type = TOKEN_SEMICOLON;
            advance_lexer(lexer);
            return token;
        }

        if (lexer->current_char == ',') {
            token.type = TOKEN_COMMA;
            advance_lexer(lexer);
            return token;
        }

        if (lexer->current_char == '%') {
            token.type = TOKEN_MODULO;
            advance_lexer(lexer);
            return token;
        }

        // If we get here, we have an invalid character
        fprintf(stderr, "Invalid character: %c\n", lexer->current_char);
        exit(1);
    }

    token.type = TOKEN_EOF;
    return token;
}

Token* tokenize(Lexer* lexer, int* token_count) {
    int capacity = 1024;
    Token* tokens = (Token*)malloc(sizeof(Token) * capacity);
    if (!tokens) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    int count = 0;

    Token token = get_next_token(lexer);
    while (token.type != TOKEN_EOF) {
        if (count >= capacity) {
            capacity *= 2;
            tokens = (Token*)realloc(tokens, sizeof(Token) * capacity);
            if (!tokens) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(1);
            }
        }

        tokens[count++] = token;
        token = get_next_token(lexer);
    }

    tokens[count++] = token; // Add EOF token
    *token_count = count;

    return tokens;
}

void free_lexer(Lexer* lexer) {
    free(lexer);
}

// Parser implementation
Parser* create_parser(Token* tokens, int tokens_length) {
    Parser* parser = (Parser*)malloc(sizeof(Parser));
    if (!parser) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    parser->tokens = tokens;
    parser->tokens_length = tokens_length;
    parser->position = 0;
    parser->current_token = tokens[0];
    return parser;
}

void advance_parser(Parser* parser) {
    parser->position++;
    if (parser->position < parser->tokens_length) {
        parser->current_token = parser->tokens[parser->position];
    }
    else {
        parser->current_token.type = TOKEN_EOF;
    }
}

Token eat(Parser* parser, TokenType type) {
    if (parser->current_token.type == type) {
        Token token = parser->current_token;
        advance_parser(parser);
        return token;
    }
    else {
        fprintf(stderr, "Expected token type %d but got %d\n", type, parser->current_token.type);
        exit(1);
    }
}

ASTNode* parse_program(Parser* parser) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    node->type = NODE_PROGRAM;

    // Allocate initial capacity for body
    int capacity = 100;
    node->data.program.body = (ASTNode**)malloc(sizeof(ASTNode*) * capacity);
    if (!node->data.program.body) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    node->data.program.body_length = 0;

    while (parser->current_token.type != TOKEN_EOF) {
        if (node->data.program.body_length >= capacity) {
            capacity *= 2;
            node->data.program.body = (ASTNode**)realloc(node->data.program.body,
                sizeof(ASTNode*) * capacity);
            if (!node->data.program.body) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(1);
            }
        }

        node->data.program.body[node->data.program.body_length++] = parse_statement(parser);
    }

    return node;
}

ASTNode* parse_statement(Parser* parser) {
    switch (parser->current_token.type) {
    case TOKEN_LET:
        return parse_variable_declaration(parser);
    case TOKEN_IF:
        return parse_if_statement(parser);
    case TOKEN_WHILE:
        return parse_while_statement(parser);
    case TOKEN_FUNCTION:
        return parse_function_declaration(parser);
    case TOKEN_RETURN:
        return parse_return_statement(parser);
    case TOKEN_PRINT:
        return parse_print_statement(parser);
    case TOKEN_IMPORT:
        return parse_import_statement(parser);
    case TOKEN_IDENTIFIER: {
        Token identifier = eat(parser, TOKEN_IDENTIFIER);

        if (parser->current_token.type == TOKEN_ASSIGN) {
            eat(parser, TOKEN_ASSIGN);
            ASTNode* value = parse_expression(parser);
            eat(parser, TOKEN_SEMICOLON);

            ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
            if (!node) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(1);
            }

            node->type = NODE_ASSIGNMENT_EXPRESSION;
            node->data.assignment_expression.name = strdup(identifier.value.string_value);
            node->data.assignment_expression.value = value;

            return node;
        }
        else if (parser->current_token.type == TOKEN_LPAREN) {
            ASTNode* call = parse_function_call(parser, identifier.value.string_value);
            eat(parser, TOKEN_SEMICOLON);
            return call;
        }

        break;
    }
    case TOKEN_LBRACE:
        return parse_block_statement(parser);
    default:
        break;
    }

    fprintf(stderr, "Unexpected token type: %d\n", parser->current_token.type);
    exit(1);
    return NULL; // To avoid compiler warning
}

ASTNode* parse_block_statement(Parser* parser) {
    eat(parser, TOKEN_LBRACE);

    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    node->type = NODE_BLOCK_STATEMENT;

    // Allocate initial capacity for body
    int capacity = 100;
    node->data.block_statement.body = (ASTNode**)malloc(sizeof(ASTNode*) * capacity);
    if (!node->data.block_statement.body) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    node->data.block_statement.body_length = 0;

    while (parser->current_token.type != TOKEN_RBRACE) {
        if (node->data.block_statement.body_length >= capacity) {
            capacity *= 2;
            node->data.block_statement.body = (ASTNode**)realloc(node->data.block_statement.body,
                sizeof(ASTNode*) * capacity);
            if (!node->data.block_statement.body) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(1);
            }
        }

        node->data.block_statement.body[node->data.block_statement.body_length++] = parse_statement(parser);
    }

    eat(parser, TOKEN_RBRACE);
    return node;
}

ASTNode* parse_variable_declaration(Parser* parser) {
    eat(parser, TOKEN_LET);
    Token name = eat(parser, TOKEN_IDENTIFIER);
    eat(parser, TOKEN_ASSIGN);
    ASTNode* value = parse_expression(parser);
    eat(parser, TOKEN_SEMICOLON);

    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    node->type = NODE_VARIABLE_DECLARATION;
    node->data.variable_declaration.name = strdup(name.value.string_value);
    node->data.variable_declaration.value = value;

    return node;
}

ASTNode* parse_if_statement(Parser* parser) {
    eat(parser, TOKEN_IF);
    eat(parser, TOKEN_LPAREN);
    ASTNode* test = parse_expression(parser);
    eat(parser, TOKEN_RPAREN);
    ASTNode* consequent = parse_statement(parser);

    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    node->type = NODE_IF_STATEMENT;
    node->data.if_statement.test = test;
    node->data.if_statement.consequent = consequent;
    node->data.if_statement.alternate = NULL;

    if (parser->current_token.type == TOKEN_ELSE) {
        eat(parser, TOKEN_ELSE);
        node->data.if_statement.alternate = parse_statement(parser);
    }

    return node;
}

ASTNode* parse_while_statement(Parser* parser) {
    eat(parser, TOKEN_WHILE);
    eat(parser, TOKEN_LPAREN);
    ASTNode* test = parse_expression(parser);
    eat(parser, TOKEN_RPAREN);
    ASTNode* body = parse_statement(parser);

    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    node->type = NODE_WHILE_STATEMENT;
    node->data.while_statement.test = test;
    node->data.while_statement.body = body;

    return node;
}

ASTNode* parse_function_declaration(Parser* parser) {
    eat(parser, TOKEN_FUNCTION);
    Token name = eat(parser, TOKEN_IDENTIFIER);
    eat(parser, TOKEN_LPAREN);

    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    node->type = NODE_FUNCTION_DECLARATION;
    node->data.function_declaration.name = strdup(name.value.string_value);

    // Allocate initial capacity for params
    int capacity = 20;
    node->data.function_declaration.params = (char**)malloc(sizeof(char*) * capacity);
    if (!node->data.function_declaration.params) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    node->data.function_declaration.params_length = 0;

    if (parser->current_token.type != TOKEN_RPAREN) {
        Token param = eat(parser, TOKEN_IDENTIFIER);
        node->data.function_declaration.params[node->data.function_declaration.params_length++] = strdup(param.value.string_value);

        while (parser->current_token.type == TOKEN_COMMA) {
            eat(parser, TOKEN_COMMA);
            param = eat(parser, TOKEN_IDENTIFIER);
            node->data.function_declaration.params[node->data.function_declaration.params_length++] = strdup(param.value.string_value);
        }
    }

    eat(parser, TOKEN_RPAREN);
    node->data.function_declaration.body = parse_statement(parser);

    return node;
}

ASTNode* parse_function_call(Parser* parser, char* name) {
    eat(parser, TOKEN_LPAREN);

    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    node->type = NODE_CALL_EXPRESSION;
    node->data.call_expression.name = strdup(name);

    // Allocate initial capacity for arguments
    int capacity = 20;
    node->data.call_expression.arguments = (ASTNode**)malloc(sizeof(ASTNode*) * capacity);
    if (!node->data.call_expression.arguments) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    node->data.call_expression.arguments_length = 0;

    if (parser->current_token.type != TOKEN_RPAREN) {
        node->data.call_expression.arguments[node->data.call_expression.arguments_length++] = parse_expression(parser);

        while (parser->current_token.type == TOKEN_COMMA) {
            eat(parser, TOKEN_COMMA);

            if (node->data.call_expression.arguments_length >= capacity) {
                capacity *= 2;
                node->data.call_expression.arguments = (ASTNode**)realloc(node->data.call_expression.arguments,
                    sizeof(ASTNode*) * capacity);
                if (!node->data.call_expression.arguments) {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(1);
                }
            }

            node->data.call_expression.arguments[node->data.call_expression.arguments_length++] = parse_expression(parser);
        }
    }

    eat(parser, TOKEN_RPAREN);

    return node;
}

ASTNode* parse_return_statement(Parser* parser) {
    eat(parser, TOKEN_RETURN);
    ASTNode* argument = parse_expression(parser);
    eat(parser, TOKEN_SEMICOLON);

    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    node->type = NODE_RETURN_STATEMENT;
    node->data.return_statement.argument = argument;

    return node;
}

ASTNode* parse_print_statement(Parser* parser) {
    eat(parser, TOKEN_PRINT);
    eat(parser, TOKEN_LPAREN);
    ASTNode* argument = parse_expression(parser);
    eat(parser, TOKEN_RPAREN);
    eat(parser, TOKEN_SEMICOLON);

    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    node->type = NODE_PRINT_STATEMENT;
    node->data.print_statement.argument = argument;

    return node;
}

ASTNode* parse_import_statement(Parser* parser) {
    eat(parser, TOKEN_IMPORT);
    eat(parser, TOKEN_LPAREN);
    Token path = eat(parser, TOKEN_STRING);
    eat(parser, TOKEN_RPAREN);
    eat(parser, TOKEN_SEMICOLON);

    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    node->type = NODE_IMPORT_STATEMENT;
    node->data.import_statement.path = strdup(path.value.string_value);

    return node;
}

ASTNode* parse_expression(Parser* parser) {
    return parse_logical_or(parser);
}

ASTNode* parse_logical_or(Parser* parser) {
    ASTNode* left = parse_logical_and(parser);

    while (parser->current_token.type == TOKEN_OR) {
        eat(parser, TOKEN_OR);
        ASTNode* right = parse_logical_and(parser);

        ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
        if (!node) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }

        node->type = NODE_LOGICAL_EXPRESSION;
        node->data.logical_expression.operator = strdup("||");
        node->data.logical_expression.left = left;
        node->data.logical_expression.right = right;

        left = node;
    }

    return left;
}

ASTNode* parse_logical_and(Parser* parser) {
    ASTNode* left = parse_equality(parser);

    while (parser->current_token.type == TOKEN_AND) {
        eat(parser, TOKEN_AND);
        ASTNode* right = parse_equality(parser);

        ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
        if (!node) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }

        node->type = NODE_LOGICAL_EXPRESSION;
        node->data.logical_expression.operator = strdup("&&");
        node->data.logical_expression.left = left;
        node->data.logical_expression.right = right;

        left = node;
    }

    return left;
}

ASTNode* parse_equality(Parser* parser) {
    ASTNode* left = parse_comparison(parser);

    while (parser->current_token.type == TOKEN_EQUALS ||
        parser->current_token.type == TOKEN_NOT_EQUALS) {

        char* operator;
        if (parser->current_token.type == TOKEN_EQUALS) {
            operator = strdup("==");
            eat(parser, TOKEN_EQUALS);
        }
        else {
            operator = strdup("!=");
            eat(parser, TOKEN_NOT_EQUALS);
        }

        ASTNode* right = parse_comparison(parser);

        ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
        if (!node) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }

        node->type = NODE_BINARY_EXPRESSION;
        node->data.binary_expression.operator = operator;
        node->data.binary_expression.left = left;
        node->data.binary_expression.right = right;

        left = node;
    }

    return left;
}

ASTNode* parse_comparison(Parser* parser) {
    ASTNode* left = parse_addition(parser);

    while (parser->current_token.type == TOKEN_GT ||
        parser->current_token.type == TOKEN_GTE ||
        parser->current_token.type == TOKEN_LT ||
        parser->current_token.type == TOKEN_LTE) {

        char* operator;
        if (parser->current_token.type == TOKEN_GT) {
            operator = strdup(">");
            eat(parser, TOKEN_GT);
        }
        else if (parser->current_token.type == TOKEN_GTE) {
            operator = strdup(">=");
            eat(parser, TOKEN_GTE);
        }
        else if (parser->current_token.type == TOKEN_LT) {
            operator = strdup("<");
            eat(parser, TOKEN_LT);
        }
        else {
            operator = strdup("<=");
            eat(parser, TOKEN_LTE);
        }

        ASTNode* right = parse_addition(parser);

        ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
        if (!node) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }

        node->type = NODE_BINARY_EXPRESSION;
        node->data.binary_expression.operator = operator;
        node->data.binary_expression.left = left;
        node->data.binary_expression.right = right;

        left = node;
    }

    return left;
}

ASTNode* parse_addition(Parser* parser) {
    ASTNode* left = parse_multiplication(parser);

    while (parser->current_token.type == TOKEN_PLUS ||
        parser->current_token.type == TOKEN_MINUS) {

        char* operator;
        if (parser->current_token.type == TOKEN_PLUS) {
            operator = strdup("+");
            eat(parser, TOKEN_PLUS);
        }
        else {
            operator = strdup("-");
            eat(parser, TOKEN_MINUS);
        }

        ASTNode* right = parse_multiplication(parser);

        ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
        if (!node) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }

        node->type = NODE_BINARY_EXPRESSION;
        node->data.binary_expression.operator = operator;
        node->data.binary_expression.left = left;
        node->data.binary_expression.right = right;

        left = node;
    }

    return left;
}

ASTNode* parse_multiplication(Parser* parser) {
    ASTNode* left = parse_primary(parser);

    while (parser->current_token.type == TOKEN_MULTIPLY ||
        parser->current_token.type == TOKEN_DIVIDE ||
        parser->current_token.type == TOKEN_MODULO) {

        char* operator;
        if (parser->current_token.type == TOKEN_MULTIPLY) {
            operator = strdup("*");
            eat(parser, TOKEN_MULTIPLY);
        }
        else if (parser->current_token.type == TOKEN_DIVIDE) {
            operator = strdup("/");
            eat(parser, TOKEN_DIVIDE);
        }
        else {
            operator = strdup("%");
            eat(parser, TOKEN_MODULO);
        }

        ASTNode* right = parse_primary(parser);

        ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
        if (!node) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }

        node->type = NODE_BINARY_EXPRESSION;
        node->data.binary_expression.operator = operator;
        node->data.binary_expression.left = left;
        node->data.binary_expression.right = right;

        left = node;
    }

    return left;
}

ASTNode* parse_primary(Parser* parser) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    switch (parser->current_token.type) {
    case TOKEN_NUMBER: {
        node->type = NODE_LITERAL;
        node->data.literal.value.number = parser->current_token.value.number_value;
        node->data.literal.value_type = 'n';
        eat(parser, TOKEN_NUMBER);
        break;
    }
    case TOKEN_STRING: {
        node->type = NODE_LITERAL;
        node->data.literal.value.string = strdup(parser->current_token.value.string_value);
        node->data.literal.value_type = 's';
        eat(parser, TOKEN_STRING);
        break;
    }
    case TOKEN_TRUE: {
        node->type = NODE_LITERAL;
        node->data.literal.value.boolean = true;
        node->data.literal.value_type = 'b';
        eat(parser, TOKEN_TRUE);
        break;
    }
    case TOKEN_FALSE: {
        node->type = NODE_LITERAL;
        node->data.literal.value.boolean = false;
        node->data.literal.value_type = 'b';
        eat(parser, TOKEN_FALSE);
        break;
    }
    case TOKEN_IDENTIFIER: {
        Token identifier = eat(parser, TOKEN_IDENTIFIER);

        if (parser->current_token.type == TOKEN_LPAREN) {
            free(node);
            return parse_function_call(parser, identifier.value.string_value);
        }

        node->type = NODE_IDENTIFIER;
        node->data.identifier.name = strdup(identifier.value.string_value);
        break;
    }
    case TOKEN_LPAREN: {
        eat(parser, TOKEN_LPAREN);
        free(node);
        node = parse_expression(parser);
        eat(parser, TOKEN_RPAREN);
        break;
    }
    default:
        fprintf(stderr, "Unexpected token in primary expression: %d\n", parser->current_token.type);
        exit(1);
    }

    return node;
}

ASTNode* parse(Parser* parser) {
    return parse_program(parser);
}

void free_parser(Parser* parser) {
    // Free tokens
    for (int i = 0; i < parser->tokens_length; i++) {
        if (parser->tokens[i].type == TOKEN_IDENTIFIER || parser->tokens[i].type == TOKEN_STRING) {
            free(parser->tokens[i].value.string_value);
        }
    }

    free(parser->tokens);
    free(parser);
}

void free_ast_node(ASTNode* node) {
    if (!node) return;

    switch (node->type) {
    case NODE_PROGRAM:
        for (int i = 0; i < node->data.program.body_length; i++) {
            free_ast_node(node->data.program.body[i]);
        }
        free(node->data.program.body);
        break;
    case NODE_BLOCK_STATEMENT:
        for (int i = 0; i < node->data.block_statement.body_length; i++) {
            free_ast_node(node->data.block_statement.body[i]);
        }
        free(node->data.block_statement.body);
        break;
    case NODE_VARIABLE_DECLARATION:
        free(node->data.variable_declaration.name);
        free_ast_node(node->data.variable_declaration.value);
        break;
    case NODE_ASSIGNMENT_EXPRESSION:
        free(node->data.assignment_expression.name);
        free_ast_node(node->data.assignment_expression.value);
        break;
    case NODE_BINARY_EXPRESSION:
        free(node->data.binary_expression.operator);
        free_ast_node(node->data.binary_expression.left);
        free_ast_node(node->data.binary_expression.right);
        break;
    case NODE_LOGICAL_EXPRESSION:
        free(node->data.logical_expression.operator);
        free_ast_node(node->data.logical_expression.left);
        free_ast_node(node->data.logical_expression.right);
        break;
    case NODE_LITERAL:
        if (node->data.literal.value_type == 's') {
            free(node->data.literal.value.string);
        }
        break;
    case NODE_IDENTIFIER:
        free(node->data.identifier.name);
        break;
    case NODE_IF_STATEMENT:
        free_ast_node(node->data.if_statement.test);
        free_ast_node(node->data.if_statement.consequent);
        if (node->data.if_statement.alternate) {
            free_ast_node(node->data.if_statement.alternate);
        }
        break;
    case NODE_WHILE_STATEMENT:
        free_ast_node(node->data.while_statement.test);
        free_ast_node(node->data.while_statement.body);
        break;
    case NODE_FUNCTION_DECLARATION:
        free(node->data.function_declaration.name);
        for (int i = 0; i < node->data.function_declaration.params_length; i++) {
            free(node->data.function_declaration.params[i]);
        }
        free(node->data.function_declaration.params);
        free_ast_node(node->data.function_declaration.body);
        break;
    case NODE_CALL_EXPRESSION:
        free(node->data.call_expression.name);
        for (int i = 0; i < node->data.call_expression.arguments_length; i++) {
            free_ast_node(node->data.call_expression.arguments[i]);
        }
        free(node->data.call_expression.arguments);
        break;
    case NODE_RETURN_STATEMENT:
        free_ast_node(node->data.return_statement.argument);
        break;
    case NODE_PRINT_STATEMENT:
        free_ast_node(node->data.print_statement.argument);
        break;
    case NODE_IMPORT_STATEMENT:
        free(node->data.import_statement.path);
        break;
    }

    free(node);
}

// Interpreter implementation
Interpreter* create_interpreter(void) {
    Interpreter* interpreter = (Interpreter*)malloc(sizeof(Interpreter));
    if (!interpreter) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    interpreter->scope_stack = (Scope**)malloc(sizeof(Scope*) * 10);
    if (!interpreter->scope_stack) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    interpreter->scope_stack_length = 0;
    interpreter->scope_stack_capacity = 10;
    interpreter->has_return = false;
    interpreter->base_dir = strdup(".");

    // Create global scope
    Scope* global_scope = create_scope();
    push_scope(interpreter, global_scope);

    return interpreter;
}

Scope* create_scope(void) {
    Scope* scope = (Scope*)malloc(sizeof(Scope));
    if (!scope) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    scope->names = (char**)malloc(sizeof(char*) * 10);
    if (!scope->names) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    scope->values = (Value*)malloc(sizeof(Value) * 10);
    if (!scope->values) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    scope->length = 0;
    scope->capacity = 10;
    return scope;
}

void push_scope(Interpreter* interpreter, Scope* scope) {
    if (interpreter->scope_stack_length >= interpreter->scope_stack_capacity) {
        interpreter->scope_stack_capacity *= 2;
        interpreter->scope_stack = (Scope**)realloc(interpreter->scope_stack,
            sizeof(Scope*) * interpreter->scope_stack_capacity);
        if (!interpreter->scope_stack) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
    }

    interpreter->scope_stack[interpreter->scope_stack_length++] = scope;
}

Scope* pop_scope(Interpreter* interpreter) {
    if (interpreter->scope_stack_length == 0) {
        return NULL;
    }

    return interpreter->scope_stack[--interpreter->scope_stack_length];
}

Scope* get_current_scope(Interpreter* interpreter) {
    if (interpreter->scope_stack_length == 0) {
        return NULL;
    }

    return interpreter->scope_stack[interpreter->scope_stack_length - 1];
}

void define_variable(Scope* scope, char* name, Value value) {
    if (scope->length >= scope->capacity) {
        scope->capacity *= 2;
        scope->names = (char**)realloc(scope->names, sizeof(char*) * scope->capacity);
        if (!scope->names) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }

        scope->values = (Value*)realloc(scope->values, sizeof(Value) * scope->capacity);
        if (!scope->values) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
    }

    scope->names[scope->length] = strdup(name);
    scope->values[scope->length] = value;
    scope->length++;
}

Value* lookup_variable(Interpreter* interpreter, char* name) {
    for (int i = interpreter->scope_stack_length - 1; i >= 0; i--) {
        Scope* scope = interpreter->scope_stack[i];

        for (int j = 0; j < scope->length; j++) {
            if (strcmp(scope->names[j], name) == 0) {
                return &scope->values[j];
            }
        }
    }

    fprintf(stderr, "Variable '%s' is not defined\n", name);
    exit(1);
    return NULL; // To avoid compiler warning
}

Value evaluate(Interpreter* interpreter, ASTNode* node) {
    switch (node->type) {
    case NODE_PROGRAM:
        return evaluate_program(interpreter, node);
    case NODE_BLOCK_STATEMENT:
        return evaluate_block_statement(interpreter, node);
    case NODE_VARIABLE_DECLARATION:
        return evaluate_variable_declaration(interpreter, node);
    case NODE_ASSIGNMENT_EXPRESSION:
        return evaluate_assignment_expression(interpreter, node);
    case NODE_BINARY_EXPRESSION:
        return evaluate_binary_expression(interpreter, node);
    case NODE_LOGICAL_EXPRESSION:
        return evaluate_logical_expression(interpreter, node);
    case NODE_LITERAL:
        return evaluate_literal(interpreter, node);
    case NODE_IDENTIFIER:
        return evaluate_identifier(interpreter, node);
    case NODE_IF_STATEMENT:
        return evaluate_if_statement(interpreter, node);
    case NODE_WHILE_STATEMENT:
        return evaluate_while_statement(interpreter, node);
    case NODE_FUNCTION_DECLARATION:
        return evaluate_function_declaration(interpreter, node);
    case NODE_CALL_EXPRESSION:
        return evaluate_call_expression(interpreter, node);
    case NODE_RETURN_STATEMENT:
        return evaluate_return_statement(interpreter, node);
    case NODE_PRINT_STATEMENT:
        return evaluate_print_statement(interpreter, node);
    case NODE_IMPORT_STATEMENT:
        return evaluate_import_statement(interpreter, node);
    default:
        fprintf(stderr, "Unknown node type: %d\n", node->type);
        exit(1);
    }

    // To avoid compiler warning
    Value null_value;
    null_value.type = VALUE_NULL;
    return null_value;
}

Value evaluate_program(Interpreter* interpreter, ASTNode* node) {
    Value result;
    result.type = VALUE_NULL;

    for (int i = 0; i < node->data.program.body_length; i++) {
        result = evaluate(interpreter, node->data.program.body[i]);

        if (interpreter->has_return) {
            return interpreter->return_value;
        }
    }

    return result;
}

Value evaluate_block_statement(Interpreter* interpreter, ASTNode* node) {
    Value result;
    result.type = VALUE_NULL;

    Scope* scope = create_scope();
    push_scope(interpreter, scope);

    for (int i = 0; i < node->data.block_statement.body_length; i++) {
        result = evaluate(interpreter, node->data.block_statement.body[i]);

        if (interpreter->has_return) {
            break;
        }
    }

    pop_scope(interpreter);
    return result;
}

Value evaluate_variable_declaration(Interpreter* interpreter, ASTNode* node) {
    Value value = evaluate(interpreter, node->data.variable_declaration.value);
    define_variable(get_current_scope(interpreter), node->data.variable_declaration.name, value);
    return value;
}

Value evaluate_assignment_expression(Interpreter* interpreter, ASTNode* node) {
    Value value = evaluate(interpreter, node->data.assignment_expression.value);

    for (int i = interpreter->scope_stack_length - 1; i >= 0; i--) {
        Scope* scope = interpreter->scope_stack[i];

        for (int j = 0; j < scope->length; j++) {
            if (strcmp(scope->names[j], node->data.assignment_expression.name) == 0) {
                scope->values[j] = value;
                return value;
            }
        }
    }

    fprintf(stderr, "Variable '%s' is not defined\n", node->data.assignment_expression.name);
    exit(1);

    // To avoid compiler warning
    Value null_value;
    null_value.type = VALUE_NULL;
    return null_value;
}

Value evaluate_binary_expression(Interpreter* interpreter, ASTNode* node) {
    Value left = evaluate(interpreter, node->data.binary_expression.left);
    Value right = evaluate(interpreter, node->data.binary_expression.right);
    Value result;

    // Handle numeric operations
    if (left.type == VALUE_NUMBER && right.type == VALUE_NUMBER) {
        result.type = VALUE_NUMBER;

        if (strcmp(node->data.binary_expression.operator, "+") == 0) {
            result.data.number = left.data.number + right.data.number;
        }
        else if (strcmp(node->data.binary_expression.operator, "-") == 0) {
            result.data.number = left.data.number - right.data.number;
        }
        else if (strcmp(node->data.binary_expression.operator, "*") == 0) {
            result.data.number = left.data.number * right.data.number;
        }
        else if (strcmp(node->data.binary_expression.operator, "/") == 0) {
            result.data.number = left.data.number / right.data.number;
        }
        else if (strcmp(node->data.binary_expression.operator, "%") == 0) {
            result.data.number = (int)left.data.number % (int)right.data.number;
        }
        else if (strcmp(node->data.binary_expression.operator, "==") == 0) {
            result.type = VALUE_BOOLEAN;
            result.data.boolean = left.data.number == right.data.number;
        }
        else if (strcmp(node->data.binary_expression.operator, "!=") == 0) {
            result.type = VALUE_BOOLEAN;
            result.data.boolean = left.data.number != right.data.number;
        }
        else if (strcmp(node->data.binary_expression.operator, ">") == 0) {
            result.type = VALUE_BOOLEAN;
            result.data.boolean = left.data.number > right.data.number;
        }
        else if (strcmp(node->data.binary_expression.operator, ">=") == 0) {
            result.type = VALUE_BOOLEAN;
            result.data.boolean = left.data.number >= right.data.number;
        }
        else if (strcmp(node->data.binary_expression.operator, "<") == 0) {
            result.type = VALUE_BOOLEAN;
            result.data.boolean = left.data.number < right.data.number;
        }
        else if (strcmp(node->data.binary_expression.operator, "<=") == 0) {
            result.type = VALUE_BOOLEAN;
            result.data.boolean = left.data.number <= right.data.number;
        }
    }
    // Handle string operations
    else if (left.type == VALUE_STRING && right.type == VALUE_STRING) {
        if (strcmp(node->data.binary_expression.operator, "+") == 0) {
            result.type = VALUE_STRING;
            result.data.string = (char*)malloc(strlen(left.data.string) + strlen(right.data.string) + 1);
            if (!result.data.string) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(1);
            }

            strcpy(result.data.string, left.data.string);
            strcat(result.data.string, right.data.string);
        }
        else if (strcmp(node->data.binary_expression.operator, "==") == 0) {
            result.type = VALUE_BOOLEAN;
            result.data.boolean = strcmp(left.data.string, right.data.string) == 0;
        }
        else if (strcmp(node->data.binary_expression.operator, "!=") == 0) {
            result.type = VALUE_BOOLEAN;
            result.data.boolean = strcmp(left.data.string, right.data.string) != 0;
        }
        else {
            fprintf(stderr, "Invalid operator '%s' for strings\n", node->data.binary_expression.operator);
            exit(1);
        }
    }
    // Handle boolean operations
    else if (left.type == VALUE_BOOLEAN && right.type == VALUE_BOOLEAN) {
        result.type = VALUE_BOOLEAN;

        if (strcmp(node->data.binary_expression.operator, "==") == 0) {
            result.data.boolean = left.data.boolean == right.data.boolean;
        }
        else if (strcmp(node->data.binary_expression.operator, "!=") == 0) {
            result.data.boolean = left.data.boolean != right.data.boolean;
        }
        else {
            fprintf(stderr, "Invalid operator '%s' for booleans\n", node->data.binary_expression.operator);
            exit(1);
        }
    }
    // Handle mixed types with type coercion for + operator
    else if (strcmp(node->data.binary_expression.operator, "+") == 0) {
        // Convert to string and concatenate
        char left_str[64];
        char right_str[64];

        if (left.type == VALUE_NUMBER) {
            sprintf(left_str, "%g", left.data.number);
        }
        else if (left.type == VALUE_STRING) {
            strcpy(left_str, left.data.string);
        }
        else if (left.type == VALUE_BOOLEAN) {
            strcpy(left_str, left.data.boolean ? "true" : "false");
        }
        else {
            strcpy(left_str, "null");
        }

        if (right.type == VALUE_NUMBER) {
            sprintf(right_str, "%g", right.data.number);
        }
        else if (right.type == VALUE_STRING) {
            strcpy(right_str, right.data.string);
        }
        else if (right.type == VALUE_BOOLEAN) {
            strcpy(right_str, right.data.boolean ? "true" : "false");
        }
        else {
            strcpy(right_str, "null");
        }

        result.type = VALUE_STRING;
        result.data.string = (char*)malloc(strlen(left_str) + strlen(right_str) + 1);
        if (!result.data.string) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }

        strcpy(result.data.string, left_str);
        strcat(result.data.string, right_str);
    }
    // Handle other mixed types
    else if (strcmp(node->data.binary_expression.operator, "==") == 0) {
        result.type = VALUE_BOOLEAN;
        result.data.boolean = false; // Different types are never equal
    }
    else if (strcmp(node->data.binary_expression.operator, "!=") == 0) {
        result.type = VALUE_BOOLEAN;
        result.data.boolean = true; // Different types are always not equal
    }
    else {
        fprintf(stderr, "Invalid operator '%s' for mixed types\n", node->data.binary_expression.operator);
        exit(1);
    }

    return result;
}

Value evaluate_logical_expression(Interpreter* interpreter, ASTNode* node) {
    Value left = evaluate(interpreter, node->data.logical_expression.left);
    Value result;

    if (strcmp(node->data.logical_expression.operator, "&&") == 0) {
        if (left.type == VALUE_BOOLEAN && !left.data.boolean) {
            result.type = VALUE_BOOLEAN;
            result.data.boolean = false;
            return result;
        }

        Value right = evaluate(interpreter, node->data.logical_expression.right);
        result.type = VALUE_BOOLEAN;
        result.data.boolean = (right.type == VALUE_BOOLEAN) ? right.data.boolean : false;
    }
    else if (strcmp(node->data.logical_expression.operator, "||") == 0) {
        if (left.type == VALUE_BOOLEAN && left.data.boolean) {
            result.type = VALUE_BOOLEAN;
            result.data.boolean = true;
            return result;
        }

        Value right = evaluate(interpreter, node->data.logical_expression.right);
        result.type = VALUE_BOOLEAN;
        result.data.boolean = (right.type == VALUE_BOOLEAN) ? right.data.boolean : false;
    }

    return result;
}

Value evaluate_literal(Interpreter* interpreter, ASTNode* node) {
    Value result;

    switch (node->data.literal.value_type) {
    case 'n':
        result.type = VALUE_NUMBER;
        result.data.number = node->data.literal.value.number;
        break;
    case 's':
        result.type = VALUE_STRING;
        result.data.string = strdup(node->data.literal.value.string);
        break;
    case 'b':
        result.type = VALUE_BOOLEAN;
        result.data.boolean = node->data.literal.value.boolean;
        break;
    default:
        fprintf(stderr, "Unknown literal type: %c\n", node->data.literal.value_type);
        exit(1);
    }

    return result;
}

Value evaluate_identifier(Interpreter* interpreter, ASTNode* node) {
    Value* value = lookup_variable(interpreter, node->data.identifier.name);
    return *value;
}

Value evaluate_if_statement(Interpreter* interpreter, ASTNode* node) {
    Value test = evaluate(interpreter, node->data.if_statement.test);
    Value result;
    result.type = VALUE_NULL;

    if (test.type == VALUE_BOOLEAN && test.data.boolean) {
        result = evaluate(interpreter, node->data.if_statement.consequent);
    }
    else if (node->data.if_statement.alternate != NULL) {
        result = evaluate(interpreter, node->data.if_statement.alternate);
    }

    return result;
}

Value evaluate_while_statement(Interpreter* interpreter, ASTNode* node) {
    Value result;
    result.type = VALUE_NULL;

    while (true) {
        Value test = evaluate(interpreter, node->data.while_statement.test);

        if (test.type != VALUE_BOOLEAN || !test.data.boolean) {
            break;
        }

        result = evaluate(interpreter, node->data.while_statement.body);

        if (interpreter->has_return) {
            break;
        }
    }

    return result;
}

Value evaluate_function_declaration(Interpreter* interpreter, ASTNode* node) {
    Value result;
    result.type = VALUE_FUNCTION;
    result.data.function.name = strdup(node->data.function_declaration.name);

    // Copy parameters
    result.data.function.params = (char**)malloc(sizeof(char*) * node->data.function_declaration.params_length);
    if (!result.data.function.params) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    result.data.function.params_length = node->data.function_declaration.params_length;

    for (int i = 0; i < node->data.function_declaration.params_length; i++) {
        result.data.function.params[i] = strdup(node->data.function_declaration.params[i]);
    }

    result.data.function.body = node->data.function_declaration.body;

    // Capture current scope (closure)
    result.data.function.closure = (Scope**)malloc(sizeof(Scope*) * interpreter->scope_stack_length);
    if (!result.data.function.closure) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    result.data.function.closure_length = interpreter->scope_stack_length;

    for (int i = 0; i < interpreter->scope_stack_length; i++) {
        result.data.function.closure[i] = interpreter->scope_stack[i];
    }

    define_variable(get_current_scope(interpreter), node->data.function_declaration.name, result);

    return result;
}

Value evaluate_call_expression(Interpreter* interpreter, ASTNode* node) {
    Value* func_value = lookup_variable(interpreter, node->data.call_expression.name);

    if (func_value->type != VALUE_FUNCTION) {
        fprintf(stderr, "'%s' is not a function\n", node->data.call_expression.name);
        exit(1);
    }

    // Evaluate arguments
    Value* args = (Value*)malloc(sizeof(Value) * node->data.call_expression.arguments_length);
    if (!args) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    for (int i = 0; i < node->data.call_expression.arguments_length; i++) {
        args[i] = evaluate(interpreter, node->data.call_expression.arguments[i]);
    }

    // Save current scope
    Scope** previous_scope = (Scope**)malloc(sizeof(Scope*) * interpreter->scope_stack_length);
    if (!previous_scope) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    int previous_scope_length = interpreter->scope_stack_length;

    for (int i = 0; i < interpreter->scope_stack_length; i++) {
        previous_scope[i] = interpreter->scope_stack[i];
    }

    // Set up function scope
    interpreter->scope_stack_length = 0;

    for (int i = 0; i < func_value->data.function.closure_length; i++) {
        push_scope(interpreter, func_value->data.function.closure[i]);
    }

    Scope* local_scope = create_scope();
    push_scope(interpreter, local_scope);

    // Bind parameters to arguments
    for (int i = 0; i < func_value->data.function.params_length; i++) {
        if (i < node->data.call_expression.arguments_length) {
            define_variable(local_scope, func_value->data.function.params[i], args[i]);
        }
        else {
            // Default to null if not enough arguments
            Value null_value;
            null_value.type = VALUE_NULL;
            define_variable(local_scope, func_value->data.function.params[i], null_value);
        }
    }

    // Execute function body
    Value result = evaluate(interpreter, func_value->data.function.body);

    // Restore previous scope
    interpreter->scope_stack_length = 0;

    for (int i = 0; i < previous_scope_length; i++) {
        push_scope(interpreter, previous_scope[i]);
    }

    free(previous_scope);
    free(args);

    // Handle return value
    if (interpreter->has_return) {
        result = interpreter->return_value;
        interpreter->has_return = false;
    }

    return result;
}

Value evaluate_return_statement(Interpreter* interpreter, ASTNode* node) {
    interpreter->return_value = evaluate(interpreter, node->data.return_statement.argument);
    interpreter->has_return = true;
    return interpreter->return_value;
}

Value evaluate_print_statement(Interpreter* interpreter, ASTNode* node) {
    Value value = evaluate(interpreter, node->data.print_statement.argument);

    switch (value.type) {
    case VALUE_NUMBER:
        printf("%g\n", value.data.number);
        break;
    case VALUE_STRING:
        printf("%s\n", value.data.string);
        break;
    case VALUE_BOOLEAN:
        printf("%s\n", value.data.boolean ? "true" : "false");
        break;
    case VALUE_FUNCTION:
        printf("[Function: %s]\n", value.data.function.name);
        break;
    case VALUE_NULL:
        printf("null\n");
        break;
    }

    return value;
}

Value evaluate_import_statement(Interpreter* interpreter, ASTNode* node) {
    char* file_path = node->data.import_statement.path;
    char* full_path = (char*)malloc(strlen(interpreter->base_dir) + strlen(file_path) + 2);
    if (!full_path) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    sprintf(full_path, "%s/%s", interpreter->base_dir, file_path);

    if (is_file_imported(full_path)) {
        Value result;
        result.type = VALUE_NULL;
        free(full_path);
        return result;
    }

    add_imported_file(full_path);

    char* code = read_file(full_path);

    if (code == NULL) {
        fprintf(stderr, "Error importing file '%s'\n", file_path);
        exit(1);
    }

    char* previous_base_dir = interpreter->base_dir;

    // Extract directory from full path
    char* last_slash = strrchr(full_path, '/');
    if (last_slash != NULL) {
        *last_slash = '\0';
        interpreter->base_dir = strdup(full_path);
    }

    Value result = process_import(code, get_current_scope(interpreter), interpreter->base_dir);

    free(interpreter->base_dir);
    interpreter->base_dir = previous_base_dir;
    free(full_path);
    free(code);

    return result;
}

void free_interpreter(Interpreter* interpreter) {
    // Free scopes
    for (int i = 0; i < interpreter->scope_stack_length; i++) {
        free_scope(interpreter->scope_stack[i]);
    }

    free(interpreter->scope_stack);
    free(interpreter->base_dir);
    free(interpreter);
}

void free_scope(Scope* scope) {
    for (int i = 0; i < scope->length; i++) {
        free(scope->names[i]);
        free_value(scope->values[i]);
    }

    free(scope->names);
    free(scope->values);
    free(scope);
}

void free_value(Value value) {
    if (value.type == VALUE_STRING) {
        free(value.data.string);
    }
    else if (value.type == VALUE_FUNCTION) {
        free(value.data.function.name);

        for (int i = 0; i < value.data.function.params_length; i++) {
            free(value.data.function.params[i]);
        }

        free(value.data.function.params);
        free(value.data.function.closure);
    }
}

Value process_import(char* code, Scope* global_scope, char* base_dir) {
    Lexer* lexer = create_lexer(code);
    int token_count;
    Token* tokens = tokenize(lexer, &token_count);

    Parser* parser = create_parser(tokens, token_count);
    ASTNode* ast = parse(parser);

    Interpreter* interpreter = create_interpreter();
    free(interpreter->base_dir);
    interpreter->base_dir = strdup(base_dir);

    // Use the same global scope
    interpreter->scope_stack[0] = global_scope;

    Value result = evaluate(interpreter, ast);

    // Don't free the global scope as it's shared
    interpreter->scope_stack[0] = NULL;
    free_interpreter(interpreter);
    free_parser(parser);
    free_ast_node(ast);
    free_lexer(lexer);

    return result;
}

Value run_interpreter(char* code, bool is_main_file) {
    Value result;
    result.type = VALUE_NULL;

    Lexer* lexer = create_lexer(code);
    int token_count;
    Token* tokens = tokenize(lexer, &token_count);

    Parser* parser = create_parser(tokens, token_count);
    ASTNode* ast = parse(parser);

    Interpreter* interpreter = create_interpreter();
    result = evaluate(interpreter, ast);

    if (is_main_file) {
        clear_imported_files();
    }

    // Free resources
    free_interpreter(interpreter);
    free_parser(parser);
    free_ast_node(ast);
    free_lexer(lexer);

    return result;
}

// Utility functions
bool is_keyword(char* identifier) {
    const char* keywords[] = {
        "let", "if", "else", "while", "function",
        "return", "true", "false", "print", "import"
    };

    for (int i = 0; i < 10; i++) {
        if (strcmp(identifier, keywords[i]) == 0) {
            return true;
        }
    }

    return false;
}

char* read_file(const char* filename) {
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(file_size + 1);
    if (!buffer) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        exit(1);
    }

    size_t bytes_read = fread(buffer, 1, file_size, file);
    buffer[bytes_read] = '\0';

    fclose(file);
    return buffer;
}

void add_imported_file(char* filename) {
    if (imported_files == NULL) {
        imported_files_capacity = 10;
        imported_files = (char**)malloc(sizeof(char*) * imported_files_capacity);
        if (!imported_files) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }

        imported_files_length = 0;
    }

    if (imported_files_length >= imported_files_capacity) {
        imported_files_capacity *= 2;
        imported_files = (char**)realloc(imported_files, sizeof(char*) * imported_files_capacity);
        if (!imported_files) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
    }

    imported_files[imported_files_length++] = strdup(filename);
}

bool is_file_imported(char* filename) {
    if (imported_files == NULL) {
        return false;
    }

    for (int i = 0; i < imported_files_length; i++) {
        if (strcmp(imported_files[i], filename) == 0) {
            return true;
        }
    }

    return false;
}

void clear_imported_files(void) {
    if (imported_files != NULL) {
        for (int i = 0; i < imported_files_length; i++) {
            free(imported_files[i]);
        }

        free(imported_files);
        imported_files = NULL;
        imported_files_length = 0;
        imported_files_capacity = 0;
    }
}