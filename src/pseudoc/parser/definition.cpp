#include <pseudoc/parser.hpp>

irl::LlvmAtomic parse_type(Lexer& lexer)
{
    auto curr = lexer.peek_current();

    if (curr.lexema == "int")
    {
        lexer.bump();
        return irl::LlvmAtomic::i32;
    }

    if (curr.lexema == "float")
    {
        lexer.bump();
        return irl::LlvmAtomic::fp;
    }

    if (curr.lexema == "void")
    {
        lexer.bump();
        return irl::LlvmAtomic::v;
    }

    throw std::logic_error("unrecognized type " + curr.lexema);
}

std::unique_ptr<ast::FunctionParam> parse_param_declaration(Lexer& lexer)
{
    auto tp = parse_type(lexer);

    auto curr = lexer.bump();

    if (curr.tk_type != TokenType::IDENTIFIER)
        throw std::logic_error("Expected identifier but got " + curr.lexema);

    auto id = curr.lexema;

    return std::make_unique<ast::FunctionParam>(id, tp);
}

std::unique_ptr<std::vector<std::unique_ptr<ast::FunctionParam>>> parse_param_declaration_list_r(Lexer& lexer, std::unique_ptr<std::vector<std::unique_ptr<ast::FunctionParam>>> list)
{
    if (lexer.peek_current().tk_type == ')')
    {
        lexer.bump();
        return list;
    }

    auto curr = lexer.bump();

    if (curr.tk_type != ',')
        throw std::logic_error("expected ')' or ',', but found " + curr.lexema);

    list->push_back(parse_param_declaration(lexer));

    return parse_param_declaration_list_r(lexer, std::move(list));
}

std::unique_ptr<std::vector<std::unique_ptr<ast::FunctionParam>>> parse_param_declaration_list(Lexer& lexer)
{
    auto curr = lexer.bump();

    if (curr.tk_type != '(')
        throw std::logic_error("Expected '(' but got " + curr.lexema);

    if (lexer.peek_current().tk_type == ')')
    {
        lexer.bump();
        return std::make_unique<std::vector<std::unique_ptr<ast::FunctionParam>>>();
    }

    auto list = std::unique_ptr<std::vector<std::unique_ptr<ast::FunctionParam>>>();
    list->push_back(parse_param_declaration(lexer));

    return parse_param_declaration_list_r(lexer, std::move(list));
}

std::unique_ptr<ast::Definition> parse_definition(Lexer& lexer)
{
    auto tp = parse_type(lexer);
    auto curr = lexer.bump();

    if (curr.tk_type != TokenType::IDENTIFIER)
        throw std::logic_error("Expected identifier but got " + curr.lexema);

    auto id = curr.lexema;
    auto params = parse_param_declaration_list(lexer);
    auto body = parse_compound_statement(lexer);

    return std::make_unique<ast::FunctionDefinition>(std::move(id), tp, std::move(params), std::move(body));
}