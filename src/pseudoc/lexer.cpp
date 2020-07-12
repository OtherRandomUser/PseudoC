#include <pseudoc/lexer.hpp>

Lexer::Lexer(const std::string& src):
    _src(src),
    _current_pos(0),
    _current_row(0),
    _current_col(0)
{
    _current = _read_token();
    _next = _read_token();
}

Lexer::Lexer(std::string&& src):
    _src(src),
    _current_pos(0),
    _current_row(0),
    _current_col(0)
{
}

Token Lexer::bump()
{
    auto read = _current;
    _current = _next;
    _next = _read_token();
    return read;
}

Token Lexer::peek_current()
{
    Token copy(_current);
    return copy;
}

Token Lexer::peek_next()
{
    Token copy(_next);
    return copy;
}

bool Lexer::is_eof()
{
    return _current.tk_type == TokenType::END_OF_FILE;
}

Token Lexer::_read_token()
{
    // TODO maybe get rid of this ???
    _eat_whitespace();

    if (_current_pos == _src.length())
    {
        Token token
        {
            .tk_type = TokenType::END_OF_FILE,
            .lexema = std::string(),
            .row = _current_row,
            .col = _current_col
        };

        return token;
    }
        
    auto c = _src.at(_current_pos);

    // TODO maybe get this back ???
    // if (c == '\n')
    //     return _read_newline();

    // if (c == ' ' || c == '\t')
    //     return _read_whitespace();
        
    if (c >= '0' && c <= '9')
        return _read_numeral();

    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
        return _read_identifier_or_keyword();

    char d = _current_pos + 1 != _src.length() ? _src.at(_current_pos + 1) : 0;

    auto token = _create_ascii_token(c, d);

    _current_pos += token.lexema.size();
    _current_col += token.lexema.size();

    return token;
}

void Lexer::_eat_whitespace()
{
    while (true)
    {
        if (_current_pos == _src.length())
            break;

        auto curr = _src.at(_current_pos);

        if (curr == '/'
            && _current_pos + 1 < _src.length()
            && _src.at(_current_pos + 1) == '/')
        {
            _eat_comment_ln();
            continue;
        }

        if (curr != ' ' && curr != '\t' && curr != '\n' && curr != '\r')
            break;

        _current_pos++;

        if (curr == '\n')
        {
            _current_row++;
            _current_col = 0;
        }
        else
            _current_col++;
    }
}

void Lexer::_eat_comment_ln()
{
    while (true)
    {
        if (_current_pos == _src.length())
            break;

        auto curr = _src.at(_current_pos);

        _current_pos++;

        if (curr == '\n')
        {
            _current_row++;
            _current_col = 0;
            break;
        }
        else
            _current_col++;
    }
}

Token Lexer::_read_newline()
{
    _current_pos++;
    _current_row++;
    _current_row = 0;

    return _create_ascii_token('\n', '\0');
}

Token Lexer::_read_whitespace()
{
    auto start_pos = _current_pos;
    auto start_col = _current_col;

    _current_pos++;
    _current_col++;

    while (true)
    {
        if (_current_pos == _src.length())
            break;

        if (_src.at(_current_pos) != ' ' && _src.at(_current_pos) != '\t')
            break;

        _current_pos++;
        _current_col++;
    }
    
    return {
        .tk_type = TokenType::WHITESPACE,
        .lexema = _src.substr(start_pos, _current_col - start_col),
        .row = _current_row,
        .col = start_col
    };
}

// TODO floats
// TODO doubles
// TODO encodings
Token Lexer::_read_numeral()
{
    auto start_pos = _current_pos;
    auto start_col = _current_col;

    _current_pos++;
    _current_col++;

    while (true)
    {
        if (_current_pos == _src.length())
            break;

        if (_src.at(_current_pos) < '0' || _src.at(_current_pos) > '9')
            break;

        _current_pos++;
        _current_col++;
    }

    if (_current_pos < _src.length() && _src.at(_current_pos) == '.')
    {
        _current_pos++;
        _current_col++;

        while (true)
        {
            if (_current_pos == _src.length())
                break;

            if (_src.at(_current_pos) < '0' || _src.at(_current_pos) > '9')
                break;

            _current_pos++;
            _current_col++;
        }

        return {
            .tk_type = TokenType::FLOAT_LITERAL,
            .lexema = _src.substr(start_pos, _current_col - start_col),
            .row = _current_row,
            .col = start_col
        };
    }

    return {
        .tk_type = TokenType::INT_LITERAL,
        .lexema = _src.substr(start_pos, _current_col - start_col),
        .row = _current_row,
        .col = start_col
    };
}

TokenType get_keyword(const std::string& lexema)
{

    if (lexema == "void")
        return TokenType::VOID;

    if (lexema == "char")
        return TokenType::CHAR;

    if (lexema == "int")
        return TokenType::INT;

    if (lexema == "float")
        return TokenType::FLOAT;

    if (lexema == "double")
        return TokenType::DOUBLE;

    if (lexema == "long")
        return TokenType::LONG;

    if (lexema == "short")
        return TokenType::SHORT;

    if (lexema == "enum")
        return TokenType::ENUM;

    if (lexema == "struct")
        return TokenType::STRUCT;

    if (lexema == "if")
        return TokenType::IF;

    if (lexema == "else")
        return TokenType::ELSE;

    if (lexema == "while")
        return TokenType::WHILE;

    if (lexema == "do")
        return TokenType::DO;

    if (lexema == "for")
        return TokenType::FOR;

    if (lexema == "goto")
        return TokenType::GOTO;

    if (lexema == "return")
        return TokenType::RETURN;

    if (lexema == "switch")
        return TokenType::SWITCH;

    if (lexema == "case")
        return TokenType::CASE;

    if (lexema == "break")
        return TokenType::BREAK;

    if (lexema == "continue")
        return TokenType::CONTINUE;

    return TokenType::IDENTIFIER;
}

Token Lexer::_read_identifier_or_keyword()
{
    auto start_pos = _current_pos;
    auto start_col = _current_col;

    _current_pos++;
    _current_col++;

    while (true)
    {
        if (_current_pos == _src.length())
            break;

        if (auto c = _src.at(_current_pos); (c < 'a' || c > 'z') && (c < 'A' || c > 'Z') && (c != '_'))
            break;

        _current_pos++;
        _current_col++;
    }

    auto lexema = _src.substr(start_pos, _current_col - start_col);
    auto tp = get_keyword(lexema);


    return {
        .tk_type = tp,
        .lexema = lexema,
        .row = _current_row,
        .col = start_col
    };
}

Token Lexer::_create_ascii_token(char c, char d)
{
    auto tp = (TokenType) c;
    std::string lexema{c};

    if (c == '+' && d == '+')
    {
        tp = TokenType::INCREMENT;
        lexema = "++";
    }

    if (c == '-' && d == '-')
    {
        tp = TokenType::DECREMENT;
        lexema = "--";
    }

    if (c == '=' && d == '=')
    {
        tp = TokenType::EQUALS;
        lexema = "==";
    }

    if (c == '!' && d == '=')
    {
        tp = TokenType::NOT_EQUAL;
        lexema = "!=";
    }

    if (c == '<' && d == '=')
    {
        tp = TokenType::LESS_THAN;
        lexema = "<=";
    }

    if (c == '>' && d == '=')
    {
        tp = TokenType::MORE_THAN;
        lexema = ">=";
    }

    if (c == '+' && d == '=')
    {
        tp = TokenType::PLUS_ASSIGNMENT;
        lexema = "+=";
    }

    if (c == '-' && d == '=')
    {
        tp = TokenType::MINUS_ASSIGNMENT;
        lexema = "-=";
    }

    if (c == '*' && d == '=')
    {
        tp = TokenType::TIMES_ASSIGNMENT;
        lexema = "*=";
    }

    if (c == '/' && d == '=')
    {
        tp = TokenType::DIVIDE_ASSIGNMENT;
        lexema = "/=";
    }

    if (c == '&' && d == '&')
    {
        tp = TokenType::LOGICAL_AND;
        lexema = "&&";
    }

    if (c == '|' && d == '|')
    {
        tp = TokenType::LOGICAL_OR;
        lexema = "||";
    }

    return {
        .tk_type = tp,
        .lexema = lexema,
        .row = _current_row,
        .col = _current_col
    };
}