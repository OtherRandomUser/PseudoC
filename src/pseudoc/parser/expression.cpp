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

    std::cout << curr.tk_type << " " << curr.lexema << std::endl;

    // TODO better error handling
    throw std::logic_error("parse error on parse_primary_expression");
}

std::unique_ptr<ast::Expression> parse_increment_expression(Lexer& lexer)
{
    auto curr = lexer.peek_current();

    if (curr.tk_type == TokenType::INCREMENT || curr.tk_type == TokenType::DECREMENT)
    {
        lexer.bump();
        auto id = lexer.bump();

        if (id.tk_type != TokenType::IDENTIFIER)
            throw std::logic_error("Expected an identifier but found " + id.lexema);

        int value = curr.tk_type == TokenType::INCREMENT ? 1 : -1;

        return std::make_unique<ast::PreIncrement>(std::move(id.lexema), value);
    }

    auto next = lexer.peek_next();

    if (curr.tk_type == TokenType::IDENTIFIER && (next.tk_type == TokenType::INCREMENT || next.tk_type == TokenType::DECREMENT))
    {
        lexer.bump();
        lexer.bump();

        int value = next.tk_type == TokenType::INCREMENT ? 1 : -1;

        return std::make_unique<ast::PostIncrement>(std::move(curr.lexema), value);
    }

    return parse_primary_expression(lexer);
}

std::unique_ptr<ast::Expression> parse_multiplicative_expression_r(Lexer& lexer, std::unique_ptr<ast::Expression> lhs)
{
    if (lexer.peek_current().tk_type == '*')
    {
        lexer.bump();
        auto rhs = parse_increment_expression(lexer);
        auto expr = std::make_unique<ast::Multiplication>(std::move(lhs), std::move(rhs));

        return parse_multiplicative_expression_r(lexer, std::move(expr));
    }

    if (lexer.peek_current().tk_type == '/')
    {
        lexer.bump();
        auto rhs = parse_increment_expression(lexer);
        auto expr = std::make_unique<ast::Division>(std::move(lhs), std::move(rhs));

        return parse_multiplicative_expression_r(lexer, std::move(expr));
    }

    auto curr = lexer.peek_current();

    if (curr.tk_type == ';'
        || curr.tk_type == ')'
        || curr.tk_type == TokenType::EQUALS
        || curr.tk_type == TokenType::NOT_EQUAL
        || curr.tk_type == '<'
        || curr.tk_type == TokenType::LESS_THAN
        || curr.tk_type == '>'
        || curr.tk_type == TokenType::MORE_THAN
        || curr.tk_type == '+'
        || curr.tk_type == '-')
    {
        return lhs;
    }

    // TODO better error handling
    throw std::logic_error("parse error on parse_multiplicative_expression_r " + curr.lexema);
}

std::unique_ptr<ast::Expression> parse_multiplicative_expression(Lexer& lexer)
{
    auto lhs = parse_increment_expression(lexer);
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

    if (auto curr = lexer.peek_current();
        curr.tk_type == ';'
        || curr.tk_type == ')'
        || curr.tk_type == TokenType::EQUALS
        || curr.tk_type == TokenType::NOT_EQUAL
        || curr.tk_type == '<'
        || curr.tk_type == TokenType::LESS_THAN
        || curr.tk_type == '>'
        || curr.tk_type == TokenType::MORE_THAN)
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

std::unique_ptr<ast::Expression> parse_equality_expression_r(Lexer& lexer, std::unique_ptr<ast::Expression> lhs)
{
    if (lexer.peek_current().tk_type == TokenType::EQUALS)
    {
        lexer.bump();
        auto rhs = parse_additive_expression(lexer);
        auto expr = std::make_unique<ast::Compare>(ast::Compare::Code::EQ, std::move(lhs), std::move(rhs));

        return parse_equality_expression_r(lexer, std::move(expr));
    }

    if (lexer.peek_current().tk_type == TokenType::NOT_EQUAL)
    {
        lexer.bump();
        auto rhs = parse_additive_expression(lexer);
        auto expr = std::make_unique<ast::Compare>(ast::Compare::Code::NE, std::move(lhs), std::move(rhs));

        return parse_equality_expression_r(lexer, std::move(expr));
    }

    if (lexer.peek_current().tk_type == '<')
    {
        lexer.bump();
        auto rhs = parse_additive_expression(lexer);
        auto expr = std::make_unique<ast::Compare>(ast::Compare::Code::LT, std::move(lhs), std::move(rhs));

        return parse_equality_expression_r(lexer, std::move(expr));
    }

    if (lexer.peek_current().tk_type == TokenType::LESS_THAN)
    {
        lexer.bump();
        auto rhs = parse_additive_expression(lexer);
        auto expr = std::make_unique<ast::Compare>(ast::Compare::Code::LE, std::move(lhs), std::move(rhs));

        return parse_equality_expression_r(lexer, std::move(expr));
    }

    if (lexer.peek_current().tk_type == '>')
    {
        lexer.bump();
        auto rhs = parse_additive_expression(lexer);
        auto expr = std::make_unique<ast::Compare>(ast::Compare::Code::LT, std::move(lhs), std::move(rhs));

        return parse_equality_expression_r(lexer, std::move(expr));
    }

    if (lexer.peek_current().tk_type == TokenType::LESS_THAN)
    {
        lexer.bump();
        auto rhs = parse_additive_expression(lexer);
        auto expr = std::make_unique<ast::Compare>(ast::Compare::Code::LE, std::move(lhs), std::move(rhs));

        return parse_equality_expression_r(lexer, std::move(expr));
    }

    if (auto curr = lexer.peek_current(); curr.tk_type == ';' || curr.tk_type == ')')
    {
        return lhs;
    }

    // TODO better error handling
    throw std::logic_error("parse error on parse_additive_expression_r");
}

std::unique_ptr<ast::Expression> parse_equality_expression(Lexer& lexer)
{
    auto lhs = parse_additive_expression(lexer);
    return parse_equality_expression_r(lexer, std::move(lhs));
}

std::unique_ptr<ast::Expression> parse_assignment_expression(Lexer& lexer)
{
    auto curr = lexer.peek_next();

    if (curr.tk_type == '=')
    {
        auto curr = lexer.bump();

        if (curr.tk_type != TokenType::IDENTIFIER)
            throw std::logic_error("parse error on parse_assignment_expression (lhs)");

        lexer.bump();

        auto rhs = parse_additive_expression(lexer);

        return std::make_unique<ast::RegularAssignment>(std::move(curr.lexema), std::move(rhs));
    }

    if (curr.tk_type == TokenType::PLUS_ASSIGNMENT)
    {
        auto variable = lexer.bump();

        if (variable.tk_type != TokenType::IDENTIFIER)
            throw std::logic_error("parse error on parse_assignment_expression (lhs)");

        auto var_ref = std::make_unique<ast::VariableRef>(variable.lexema);

        lexer.bump();

        auto rhs = parse_additive_expression(lexer);
        rhs = std::make_unique<ast::Addition>(std::move(var_ref), std::move(rhs));

        return std::make_unique<ast::RegularAssignment>(std::move(variable.lexema), std::move(rhs));
    }

    if (curr.tk_type == TokenType::MINUS_ASSIGNMENT)
    {
        auto variable = lexer.bump();

        if (variable.tk_type != TokenType::IDENTIFIER)
            throw std::logic_error("parse error on parse_assignment_expression (lhs)");

        auto var_ref = std::make_unique<ast::VariableRef>(variable.lexema);

        lexer.bump();

        auto rhs = parse_additive_expression(lexer);
        rhs = std::make_unique<ast::Subtraction>(std::move(var_ref), std::move(rhs));

        return std::make_unique<ast::RegularAssignment>(std::move(variable.lexema), std::move(rhs));
    }

    if (curr.tk_type == TokenType::TIMES_ASSIGNMENT)
    {
        auto variable = lexer.bump();

        if (variable.tk_type != TokenType::IDENTIFIER)
            throw std::logic_error("parse error on parse_assignment_expression (lhs)");

        auto var_ref = std::make_unique<ast::VariableRef>(variable.lexema);

        lexer.bump();

        auto rhs = parse_additive_expression(lexer);
        rhs = std::make_unique<ast::Multiplication>(std::move(var_ref), std::move(rhs));

        return std::make_unique<ast::RegularAssignment>(std::move(variable.lexema), std::move(rhs));
    }

    if (curr.tk_type == TokenType::DIVIDE_ASSIGNMENT)
    {
        auto variable = lexer.bump();

        if (variable.tk_type != TokenType::IDENTIFIER)
            throw std::logic_error("parse error on parse_assignment_expression (lhs)");

        auto var_ref = std::make_unique<ast::VariableRef>(variable.lexema);

        lexer.bump();

        auto rhs = parse_additive_expression(lexer);
        rhs = std::make_unique<ast::Division>(std::move(var_ref), std::move(rhs));

        return std::make_unique<ast::RegularAssignment>(std::move(variable.lexema), std::move(rhs));
    }

    return parse_equality_expression(lexer);
}

std::unique_ptr<ast::Expression> parse_expression(Lexer& lexer)
{
    return parse_assignment_expression(lexer);
}