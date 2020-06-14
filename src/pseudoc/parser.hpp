#pragma once

#include <pseudoc/ast.hpp>
#include <pseudoc/lexer.hpp>

irl::LlvmAtomic parse_type(Lexer& lexer);
std::unique_ptr<ast::Expression> parse_expression(Lexer& lexer);
std::unique_ptr<ast::Statement> parse_statement(Lexer& lexer);
std::unique_ptr<ast::CompoundStatement> parse_compound_statement(Lexer& lexer);
std::unique_ptr<ast::Definition> parse_definition(Lexer& lexer);