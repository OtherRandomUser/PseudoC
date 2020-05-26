#include <pseudoc/parser.hpp>

std::unique_ptr<ast::Expression> parse_primary_expression(Lexer& lexer, std::shared_ptr<ast::VariableScope> scope)
{
    if (lexer.peek_current().tk_type == TokenType::IDENTIFIER)
        return std::make_unique<ast::Identifier>(scope, lexer.bump().lexema);

    if (lexer.peek_current().tk_type == TokenType::INT_LITERAL)
        // TODO parse other literals
        return std::make_unique<ast::I32Constant>(scope, std::stoi(lexer.bump().lexema));

    // TODO better error handling
    throw std::logic_error("parse error on parse_primary_expression");
}

std::unique_ptr<ast::Expression> parse_multiplicative_expression_r(Lexer& lexer, std::unique_ptr<ast::Expression> lhs, std::shared_ptr<ast::VariableScope> scope)
{
    if (lexer.peek_current().tk_type == '*')
    {
        lexer.bump();
        auto rhs = parse_primary_expression(lexer, scope);
        auto expr = std::make_unique<ast::Multiplication>(scope, std::move(lhs), std::move(rhs));

        return parse_multiplicative_expression_r(lexer, std::move(expr), std::move(scope));
    }

    if (lexer.peek_current().tk_type == '/')
    {
        lexer.bump();
        auto rhs = parse_primary_expression(lexer, scope);
        auto expr = std::make_unique<ast::Division>(scope, std::move(lhs), std::move(rhs));

        return parse_multiplicative_expression_r(lexer, std::move(expr), std::move(scope));
    }

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

std::unique_ptr<ast::Expression> parse_multiplicative_expression(Lexer& lexer, std::shared_ptr<ast::VariableScope> scope)
{
    auto lhs = parse_primary_expression(lexer, scope);
    return parse_multiplicative_expression_r(lexer, std::move(lhs), std::move(scope));
}

std::unique_ptr<ast::Expression> parse_additive_expression_r(Lexer& lexer, std::unique_ptr<ast::Expression> lhs, std::shared_ptr<ast::VariableScope> scope)
{
    if (lexer.peek_current().tk_type == '+')
    {
        lexer.bump();
        auto rhs = parse_multiplicative_expression(lexer, scope);
        auto expr = std::make_unique<ast::Addition>(scope, std::move(lhs), std::move(rhs));

        return parse_additive_expression_r(lexer, std::move(expr), std::move(scope));
    }

    if (lexer.peek_current().tk_type == '-')
    {
        lexer.bump();
        auto rhs = parse_multiplicative_expression(lexer, scope);
        auto expr = std::make_unique<ast::Subtraction>(scope, std::move(lhs), std::move(rhs));

        return parse_additive_expression_r(lexer, std::move(expr), std::move(scope));
    }

    if (auto curr = lexer.peek_current(); curr.tk_type == ';')
    {
        return lhs;
    }

    // TODO better error handling
    throw std::logic_error("parse error on parse_additive_expression_r");
}

std::unique_ptr<ast::Expression> parse_additive_expression(Lexer& lexer, std::shared_ptr<ast::VariableScope> scope)
{
    auto lhs = parse_multiplicative_expression(lexer, scope);
    return parse_additive_expression_r(lexer, std::move(lhs), std::move(scope));
}

std::unique_ptr<ast::Expression> parse_expression(Lexer& lexer, std::shared_ptr<ast::VariableScope> scope)
{
    return parse_additive_expression(lexer, std::move(scope));
}