#include <pseudoc/ast/statement.hpp>

using namespace ast;

VariableDeclaration::VariableDeclaration(std::string identifier, int type_id, std::unique_ptr<Expression> initializer):
    _identifier(std::move(identifier)),
    _type_id(type_id),
    _initializer(std::move(initializer))
{
}

std::string VariableDeclaration::print()
{
    return "var (" + std::to_string(_type_id) + ") " + _identifier + " = " + _initializer->print();
}

irl::IrlSegment VariableDeclaration::code_gen()
{
    return irl::IrlSegment();
}

DeclarationStatement::DeclarationStatement(std::unique_ptr<VariableDeclaration> decl)
{
    _decls.push_back(std::move(decl));
}

std::string DeclarationStatement::print()
{
    std::string res = "(decls ";
    std::string junc = "";

    for (auto& i: _decls)
    {
        res += junc + i->print();
        junc = ", ";
    }

    return res + " )";
}

irl::IrlSegment DeclarationStatement::code_gen()
{
    return irl::IrlSegment();
}

void DeclarationStatement::add_variable(std::unique_ptr<VariableDeclaration> decl)
{
    _decls.push_back(std::move(decl));
}

ExpressionStatement::ExpressionStatement(std::unique_ptr<Expression> expr):
    _expr(std::move(expr))
{
}

std::string ExpressionStatement::print()
{
    return _expr->print();
}

irl::IrlSegment ExpressionStatement::code_gen()
{
    return irl::IrlSegment();
}

void CompoundStatement::add_statement(std::unique_ptr<Statement> statement)
{
    _statements.push_back(std::move(statement));
}

std::string CompoundStatement::print()
{
    std::string res = "{\n";

    for (auto& statement: _statements)
    {
        res += statement->print() + ";\n";
    }

    return res + "}\n";
    
}

irl::IrlSegment CompoundStatement::code_gen()
{
    return irl::IrlSegment();
}