#include <pseudoc/parser.hpp>

#include <pseudoc/ast/flow.hpp>

bool is_type(const Token& tk)
{
    return tk.tk_type == TokenType::INT || tk.tk_type == TokenType::FLOAT || tk.tk_type == TokenType::VOID;
}

std::unique_ptr<ast::Statement> parse_declaration_statement(Lexer& lexer)
{
    auto tp = parse_type(lexer);
    auto curr = lexer.bump();

    if (curr.tk_type != TokenType::IDENTIFIER)
        throw std::logic_error("Expected identifier but found " + curr.lexema);

    // TODO parse variables list
    auto identifier = curr.lexema;

    std::unique_ptr<ast::Expression> initializer;

    if (lexer.peek_current().tk_type == '=')
    {
        lexer.bump();
        // TODO parse initializer list
        initializer = parse_expression(lexer);
    }

    auto sem = lexer.bump();

    if (sem.tk_type != ';')
        throw std::logic_error("expected ';' but found " + sem.lexema);

    auto decl = std::make_unique<ast::VariableDeclaration>(identifier, std::move(initializer));
    return std::make_unique<ast::DeclarationStatement>(std::move(decl));
}

std::unique_ptr<ast::CompoundStatement> parse_compound_statement(Lexer& lexer)
{
    auto curr = lexer.bump();

    if (curr.tk_type != '{')
        throw std::logic_error("Expected '{' but found " + curr.lexema);

    curr = lexer.peek_current();
    auto compound = std::make_unique<ast::CompoundStatement>();

    while (curr.tk_type != '}')
    {
        if (curr.tk_type == TokenType::END_OF_FILE)
            throw std::logic_error("Expected '{' but reached end of file");

        auto statement = parse_statement(lexer);
        compound->add_statement(std::move(statement));

        curr = lexer.peek_current();
    }

    lexer.bump();
    return compound;
}

std::unique_ptr<ast::Statement> parse_expression_statement(Lexer& lexer)
{
    auto expr = parse_expression(lexer);
    auto curr = lexer.bump();

    if (curr.tk_type != ';')
        throw std::logic_error("expected ';' but found " + curr.lexema);

    return std::make_unique<ast::ExpressionStatement>(std::move(expr));
}

std::unique_ptr<ast::Statement> parse_return_statement(Lexer& lexer)
{
    lexer.bump();
    auto expr = parse_expression(lexer);
    auto curr = lexer.bump();

    if (curr.tk_type != ';')
        throw std::logic_error("expected ';' but found " + curr.lexema);

    return std::make_unique<ast::ReturnStatement>(std::move(expr));
}

std::unique_ptr<ast::Statement> parse_if_statement(Lexer& lexer)
{
    lexer.bump();
    auto curr = lexer.bump();

    if (curr.tk_type != '(')
        throw std::logic_error("expected '(' but found '" + curr.lexema + "'");

    auto condition = parse_expression(lexer);

    if (condition->get_type() != irl::LlvmAtomic::b)
    {
        condition = std::make_unique<ast::BooleanCast>(std::move(condition));
    }

    curr = lexer.bump();

    if (curr.tk_type != ')')
        throw std::logic_error("expected ')' but found '" + curr.lexema + "'");

    auto on_true = parse_statement(lexer);

    if (lexer.peek_current().tk_type == TokenType::ELSE)
    {
        lexer.bump();

        auto on_false = parse_statement(lexer);

        return std::make_unique<ast::IfStatement>(std::move(condition), std::move(on_true), std::move(on_false));
    }

    return std::make_unique<ast::IfStatement>(std::move(condition), std::move(on_true));
}

std::unique_ptr<ast::Statement> parse_while_loop(Lexer& lexer)
{
    lexer.bump();
    auto curr = lexer.bump();

    if (curr.tk_type != '(')
        throw std::logic_error("expected '(' but found '" + curr.lexema + "'");

    auto condition = parse_expression(lexer);

    if (condition->get_type() != irl::LlvmAtomic::b)
    {
        condition = std::make_unique<ast::BooleanCast>(std::move(condition));
    }

    curr = lexer.bump();

    if (curr.tk_type != ')')
        throw std::logic_error("expected ')' but found '" + curr.lexema + "'");

    auto body = parse_statement(lexer);

    return std::make_unique<ast::WhileLoop>(std::move(condition), std::move(body));
}

std::unique_ptr<ast::Statement> parse_continue_statement(Lexer& lexer)
{
    lexer.bump();
    auto curr = lexer.bump();

    if (curr.tk_type != ';')
        throw std::logic_error("expected ';' but found '" + curr.lexema + "'");

    return std::make_unique<ast::Continue>();
}

std::unique_ptr<ast::Statement> parse_break_statement(Lexer& lexer)
{
    lexer.bump();
    auto curr = lexer.bump();

    if (curr.tk_type != ';')
        throw std::logic_error("expected ';' but found '" + curr.lexema + "'");

    return std::make_unique<ast::Break>();
}

std::unique_ptr<ast::Statement> parse_statement(Lexer& lexer)
{
    auto curr = lexer.peek_current();

    if (is_type(curr))
        return parse_declaration_statement(lexer);

    if (curr.tk_type == '{')
        return parse_compound_statement(lexer);

    if (curr.tk_type == TokenType::RETURN)
        return parse_return_statement(lexer);

    if (curr.tk_type == TokenType::IF)
        return parse_if_statement(lexer);

    if (curr.tk_type == TokenType::WHILE)
        return parse_while_loop(lexer);

    if (curr.tk_type == TokenType::CONTINUE)
        return parse_continue_statement(lexer);

    if (curr.tk_type == TokenType::BREAK)
        return parse_break_statement(lexer);

    return parse_expression_statement(lexer);
}