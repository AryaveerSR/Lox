#include "compiler.h"
#include "scanner.h"

void compile(const char *source)
{
    Scanner scanner;
    init_scanner(&scanner, source);

    uint16_t line = 0;

    while (true)
    {
        Token token = scan_token(&scanner);

        if (token.line != line)
        {
            line = token.line;
            printf("%4d", line);
        }
        else
        {
            printf("    ");
        }

        printf("%2d '%.*s'\n", token.type, token.length, token.start);

        if (token.type == TOKEN_EOF)
        {
            break;
        }
    }
}