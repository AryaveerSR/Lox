#include <string.h>
#include <ctype.h>
#include "scanner.h"

void init_scanner(Scanner *scanner, const char *source)
{
    scanner->start = source;
    scanner->current = source;

    scanner->line = 1;
}

static Token make_token(Scanner *scanner, TokenType type)
{
    Token token;

    token.type = type;
    token.line = scanner->line;
    token.start = scanner->start;
    token.length = (size_t)(scanner->current - scanner->start);

    return token;
}

static Token error_token(Scanner *scanner, const char *msg)
{
    Token token;

    token.type = TOKEN_ERROR;
    token.start = msg;
    token.line = scanner->line;
    token.length = (size_t)(strlen(msg));

    return token;
}

static bool is_at_end(Scanner *scanner)
{
    return *(scanner->current) == '\0';
}

static char advance(Scanner *scanner)
{
    scanner->current++;
    return scanner->current[-1];
}

static bool match(Scanner *scanner, char expected)
{
    if (is_at_end(scanner) || (*scanner->current == expected))
    {
        return false;
    }

    scanner->current++;
    return true;
}

static char peek(Scanner *scanner)
{
    return *scanner->current;
}

static char peek_next(Scanner *scanner)
{
    if (is_at_end(scanner))
    {
        return '\0';
    }

    return scanner->current[1];
}

static void skip_whitespace(Scanner *scanner)
{
    while (true)
    {
        char ch = peek(scanner);

        switch (ch)
        {
        case ' ':
        case '\r':
        case '\t':
            advance(scanner);
            break;

        default:
            return;
        }
    }
}

static Token scan_number(Scanner *scanner)
{
    while (isdigit(peek(scanner)))
    {
        advance(scanner);
    }

    if (peek(scanner) == '.' && isdigit(peek_next(scanner)))
    {
        advance(scanner);

        while (isdigit(peek(scanner)))
        {
            advance(scanner);
        }
    }

    return make_token(scanner, TOKEN_NUMBER);
}

static Token scan_string(Scanner *scanner)
{
    while (peek(scanner) != '"' && !is_at_end(scanner))
    {
        if (peek(scanner) == '\n')
        {
            scanner->line++;
        }
        advance(scanner);
    }

    if (is_at_end(scanner))
    {
        return error_token(scanner, "Unterminated string.");
    }

    advance(scanner);
    return make_token(scanner, TOKEN_STRING);
}

static bool is_valid_ident(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c == '_');
}

static TokenType check_keyword(Scanner *scanner, size_t start, size_t length, const char *rest, TokenType type)
{
    if (((scanner->current - scanner->start) == (start + length)) &&
        memcmp(scanner->start + start, rest, length) == 0)
    {
        return type;
    }

    return TOKEN_IDENTIFIER;
}

static TokenType identifier_type(Scanner *scanner)
{
    switch (scanner->start[0])
    {
    case 'a':
        return check_keyword(scanner, 1, 2, "nd", TOKEN_AND);
    case 'c':
        return check_keyword(scanner, 1, 2, "lass", TOKEN_CLASS);
    case 'e':
        return check_keyword(scanner, 1, 2, "lse", TOKEN_ELSE);
    case 'i':
        return check_keyword(scanner, 1, 2, "f", TOKEN_IF);
    case 'n':
        return check_keyword(scanner, 1, 2, "il", TOKEN_NIL);
    case 'o':
        return check_keyword(scanner, 1, 2, "r", TOKEN_OR);
    case 'p':
        return check_keyword(scanner, 1, 2, "rint", TOKEN_PRINT);
    case 'r':
        return check_keyword(scanner, 1, 2, "eturn", TOKEN_RETURN);
    case 's':
        return check_keyword(scanner, 1, 2, "uper", TOKEN_SUPER);
    case 'v':
        return check_keyword(scanner, 1, 2, "ar", TOKEN_VAR);
    case 'w':
        return check_keyword(scanner, 1, 2, "hile", TOKEN_WHILE);

    case 't':
        if (scanner->current - scanner->start > 1)
        {
            switch (scanner->start[1])
            {
            case 'h':
                return check_keyword(scanner, 1, 2, "is", TOKEN_THIS);
            case 'r':
                return check_keyword(scanner, 1, 2, "ue", TOKEN_TRUE);

            default:
                break;
            }
        }
        break;

    default:
        break;
    }

    return TOKEN_IDENTIFIER;
}

static Token scan_identifier(Scanner *scanner)
{
    while (is_valid_ident(peek(scanner)) || isdigit(peek(scanner)))
    {
        advance(scanner);
    }

    return make_token(scanner, identifier_type(scanner));
}

Token scan_token(Scanner *scanner)
{
    skip_whitespace(scanner);
    scanner->start = scanner->current;

    if (is_at_end(scanner))
    {
        return make_token(scanner, TOKEN_EOF);
    }

    char ch = advance(scanner);

    if (is_valid_ident(ch))
    {
        return scan_identifier(scanner);
    }

    if (isdigit(ch))
    {
        return scan_number(scanner);
    }

    switch (ch)
    {
    case '(':
        return make_token(scanner, TOKEN_LEFT_PAREN);
    case ')':
        return make_token(scanner, TOKEN_RIGHT_PAREN);
    case '{':
        return make_token(scanner, TOKEN_LEFT_BRACE);
    case '}':
        return make_token(scanner, TOKEN_RIGHT_BRACE);
    case ';':
        return make_token(scanner, TOKEN_SEMICOLON);
    case ',':
        return make_token(scanner, TOKEN_COMMA);
    case '.':
        return make_token(scanner, TOKEN_DOT);
    case '-':
        return make_token(scanner, TOKEN_MINUS);
    case '+':
        return make_token(scanner, TOKEN_PLUS);
    case '/':
        if (peek_next(scanner) == '/')
        {
            while (peek(scanner) != '\n' && !is_at_end(scanner))
            {
                advance(scanner);
            }
        }
        else
        {
            return;
        }
        break;
    case '*':
        return make_token(scanner, TOKEN_STAR);

    case '\n':
        scanner->line++;
        advance(scanner);
        break;

    case '!':
        return make_token(scanner, match(scanner, '=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
    case '<':
        return make_token(scanner, match(scanner, '=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    case '>':
        return make_token(scanner, match(scanner, '=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    case '=':
        return make_token(scanner, match(scanner, '=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);

    case '"':
        return scan_string(scanner);

    default:
        break;
    }

    return error_token(scanner, "Unexpected character.");
}