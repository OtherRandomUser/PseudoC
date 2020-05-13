#include <pseudoc/parser.hpp>

// !!!
#include <iostream>

void eat_whitespace(Lexer& lexer)
{
    while (lexer.peek_current().tk_type == TokenType::WHITESPACE)
        lexer.bump();
}

std::unique_ptr<Expression> parse_primary_expression(Lexer& lexer)
{
    eat_whitespace(lexer);
    
    if (lexer.peek_current().tk_type == TokenType::IDENTIFIER)
        return std::make_unique<Identifier>(lexer.bump().lexema);

    if (lexer.peek_current().tk_type == TokenType::INT_LITERAL)
        // TODO parse other literals
        return std::make_unique<I32Literal>(std::stoi(lexer.bump().lexema));

    // TODO better error handling
    throw std::logic_error("parse error on parse_primary_expression");
}

std::unique_ptr<Expression> parse_multiplicative_expression_r(Lexer& lexer, std::unique_ptr<Expression> lhs)
{
    if (lexer.peek_current().tk_type == '*')
    {
        lexer.bump();
        auto rhs = parse_primary_expression(lexer);
        auto expr = std::make_unique<Multiplication>(std::move(lhs), std::move(rhs));

        return parse_multiplicative_expression_r(lexer, std::move(expr));
    }

    if (lexer.peek_current().tk_type == '/')
    {
        lexer.bump();
        auto rhs = parse_primary_expression(lexer);
        auto expr = std::make_unique<Division>(std::move(lhs), std::move(rhs));

        return parse_multiplicative_expression_r(lexer, std::move(expr));
    }

    std::cout << "current " << lexer.peek_current().tk_type << std::endl;

    if (auto curr = lexer.peek_current();
        curr.tk_type == ';'
        || curr.tk_type == '+'
        || curr.tk_type == '-')
    {
        return lhs;
    }

    // TODO better error handling
    throw std::logic_error("parse error on parse_multiplicative_expression_r");
}

std::unique_ptr<Expression> parse_multiplicative_expression(Lexer& lexer)
{
    auto lhs = parse_primary_expression(lexer);
    return parse_multiplicative_expression_r(lexer, std::move(lhs));
}

std::unique_ptr<Expression> parse_additive_expression_r(Lexer& lexer, std::unique_ptr<Expression> lhs)
{
    if (lexer.peek_current().tk_type == '+')
    {
        lexer.bump();
        auto rhs = parse_multiplicative_expression(lexer);
        auto expr = std::make_unique<Addition>(std::move(lhs), std::move(rhs));

        return parse_additive_expression_r(lexer, std::move(expr));
    }

    if (lexer.peek_current().tk_type == '-')
    {
        lexer.bump();
        auto rhs = parse_multiplicative_expression(lexer);
        auto expr = std::make_unique<Subtraction>(std::move(lhs), std::move(rhs));

        return parse_additive_expression_r(lexer, std::move(expr));
    }

    if (auto curr = lexer.peek_current(); curr.tk_type == ';')
    {
        return lhs;
    }

    // TODO better error handling
    throw std::logic_error("parse error on parse_additive_expression_r");
}

std::unique_ptr<Expression> parse_additive_expression(Lexer& lexer)
{
    auto lhs = parse_multiplicative_expression(lexer);
    return parse_additive_expression_r(lexer, std::move(lhs));
}

std::unique_ptr<Expression> parse_expression(Lexer& lexer)
{
    return parse_additive_expression(lexer);
}