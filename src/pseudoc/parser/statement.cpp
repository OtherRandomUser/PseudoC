#include <pseudoc/parser.hpp>

std::unique_ptr<ast::Statement> parse_statement(Lexer& lexer, std::shared_ptr<ast::VariableScope> scope)
{
    return std::unique_ptr<ast::Statement>();
}