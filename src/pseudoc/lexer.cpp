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

    auto token = _create_ascii_token(c);

    _current_pos++;
    _current_col++;

    return token;
}

void Lexer::_eat_whitespace()
{
    while (true)
    {
        if (_current_pos == _src.length())
            break;

        auto curr = _src.at(_current_pos);

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

Token Lexer::_read_newline()
{
    _current_pos++;
    _current_row++;
    _current_row = 0;

    return _create_ascii_token('\n');
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

    return {
        .tk_type = TokenType::INT_LITERAL,
        .lexema = _src.substr(start_pos, _current_col - start_col),
        .row = _current_row,
        .col = start_col
    };
}

// TODO actually read keywords
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

    return {
        .tk_type = TokenType::IDENTIFIER,
        .lexema = _src.substr(start_pos, _current_col - start_col),
        .row = _current_row,
        .col = start_col
    };
}

Token Lexer::_create_ascii_token(char c)
{
    return {
        .tk_type = (TokenType) c,
        .lexema = std::string{c},
        .row = _current_row,
        .col = _current_col
    };
}