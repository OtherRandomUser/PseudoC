#pragma once

#include <pseudoc/ast.hpp>
#include <pseudoc/lexer.hpp>

std::unique_ptr<ast::Expression> parse_expression(Lexer& lexer, std::shared_ptr<ast::VariableScope> scope);
std::unique_ptr<ast::Statement> parse_statement(Lexer& lexer, std::shared_ptr<ast::VariableScope> scope);