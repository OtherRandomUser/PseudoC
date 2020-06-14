#pragma once

#include <exception>
#include <string>
#include <vector>

enum TokenType : int
{
    IDENTIFIER      = 256,
    WHITESPACE      = 257,
    LINE_BREAK      = 258,
    NULL_TERMINATOR = 259,
    END_OF_FILE     = 260,
    COMMENT_LINE    = 261,
    COMMENT_BLOCK   = 262,

    // literals
    INT_LITERAL    = 301,
    FLOAT_LITERAL  = 302,
    CHAR_LITERAL   = 303,
    STRING_LITERAL = 304,
    OCTAL_ENCODING = 305, // 0...
    HEX_ENCODING   = 306, // 0x...

    // multi char operators
    INCREMENT         = 401, // ++
    DECREMENT         = 402, // --
    EQUALS            = 403, // ==
    NOT_EQUAL         = 404, // ==
    LESS_THAN         = 405, // <=
    MORE_THAN         = 406, // >=
    PLUS_ASSIGNMENT   = 407, // +=
    MINUS_ASSIGNMENT  = 408, // -=
    TIMES_ASSIGNMENT  = 409, // *=
    DIVIDE_ASSIGNMENT = 410, // /=
    // TODO bitwise assignment

    // type keywords
    VOID   = 501,
    CHAR   = 502,
    INT    = 503,
    FLOAT  = 504,
    DOUBLE = 505,
    LONG   = 506,
    SHORT  = 507,
    ENUM   = 508,
    STRUCT = 509,

    // flow control keywords
    IF       = 601,
    ELSE     = 602,
    WHILE    = 603,
    DO       = 604,
    FOR      = 605,
    GOTO     = 606,
    RETURN   = 607,
    SWITCH   = 608,
    CASE     = 609,
    BREAK    = 610,
    CONTINUE = 611,

    // errors
    LEX_ERROR = 1001
};

struct Token
{
    TokenType tk_type;
    // TODO stringview is probably better for this
    std::string lexema;
    unsigned long row;
    unsigned long col;
};

class Lexer
{
public:
    Lexer(const std::string& src);
    Lexer(std::string&& src);

    Lexer(Lexer& other) = delete;
    Lexer& operator = (Lexer& other) = delete;

    Token bump();
    Token peek_current();
    Token peek_next();
    bool is_eof();

private:
    const std::string _src;
    unsigned long _current_pos;
    unsigned long _current_row;
    unsigned long _current_col;

    Token _current;
    Token _next;

    Token _read_token();

    Token _read_newline();
    Token _read_whitespace();
    Token _read_numeral();
    Token _read_identifier_or_keyword();

    void _eat_whitespace();
    void _eat_comment_ln();

    Token _create_ascii_token(char c);
};