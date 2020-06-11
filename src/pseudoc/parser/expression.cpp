#include <pseudoc/parser.hpp>

#include <iostream>

std::unique_ptr<ast::Expression> parse_primary_expression(Lexer& lexer)
{
    auto curr = lexer.bump();
    
    if (curr.tk_type == TokenType::IDENTIFIER)
        return std::make_unique<ast::VariableRef>(curr.lexema);

    if (curr.tk_type == TokenType::INT_LITERAL)
        // TODO parse other literals
        return std::make_unique<ast::I32Constant>(std::stoi(curr.lexema));

    std::cout << curr.tk_type << std::endl;

    // TODO better error handling
    throw std::logic_error("parse error on parse_primary_expression");
}

std::unique_ptr<ast::Expression> parse_multiplicative_expression_r(Lexer& lexer, std::unique_ptr<ast::Expression> lhs)
{
    if (lexer.peek_current().tk_type == '*')
    {
        lexer.bump();
        auto rhs = parse_primary_expression(lexer);
        auto expr = std::make_unique<ast::Multiplication>(std::move(lhs), std::move(rhs));

        return parse_multiplicative_expression_r(lexer, std::move(expr));
    }

    if (lexer.peek_current().tk_type == '/')
    {
        lexer.bump();
        auto rhs = parse_primary_expression(lexer);
        auto expr = std::make_unique<ast::Division>(std::move(lhs), std::move(rhs));

        return parse_multiplicative_expression_r(lexer, std::move(expr));
    }

    auto curr = lexer.peek_current();

    if (curr.tk_type == ';'
        || curr.tk_type == '+'
        || curr.tk_type == '-')
    {
        return lhs;
    }

    // TODO better error handling
    throw std::logic_error("parse error on parse_multiplicative_expression_r");
}

std::unique_ptr<ast::Expression> parse_multiplicative_expression(Lexer& lexer)
{
    auto lhs = parse_primary_expression(lexer);
    return parse_multiplicative_expression_r(lexer, std::move(lhs));
}

std::unique_ptr<ast::Expression> parse_additive_expression_r(Lexer& lexer, std::unique_ptr<ast::Expression> lhs)
{
    if (lexer.peek_current().tk_type == '+')
    {
        lexer.bump();
        auto rhs = parse_multiplicative_expression(lexer);
        auto expr = std::make_unique<ast::Addition>(std::move(lhs), std::move(rhs));

        return parse_additive_expression_r(lexer, std::move(expr));
    }

    if (lexer.peek_current().tk_type == '-')
    {
        lexer.bump();
        auto rhs = parse_multiplicative_expression(lexer);
        auto expr = std::make_unique<ast::Subtraction>(std::move(lhs), std::move(rhs));

        return parse_additive_expression_r(lexer, std::move(expr));
    }

    if (auto curr = lexer.peek_current(); curr.tk_type == ';')
    {
        return lhs;
    }

    // TODO better error handling
    throw std::logic_error("parse error on parse_additive_expression_r");
}

std::unique_ptr<ast::Expression> parse_additive_expression(Lexer& lexer)
{
    auto lhs = parse_multiplicative_expression(lexer);
    return parse_additive_expression_r(lexer, std::move(lhs));
}

std::unique_ptr<ast::Expression> parse_expression(Lexer& lexer)
{
    return parse_additive_expression(lexer);
}