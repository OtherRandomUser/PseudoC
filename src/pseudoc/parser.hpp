#pragma once

#include <pseudoc/ast.hpp>
#include <pseudoc/lexer.hpp>

std::unique_ptr<Expression> parse_expression(Lexer& lexer);