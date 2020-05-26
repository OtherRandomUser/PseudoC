#include <pseudoc/ast/statement.hpp>

using namespace ast;

VariableDeclaration::VariableDeclaration(std::shared_ptr<VariableScope> scope, std::string identifier, int type_id, std::unique_ptr<Expression> initializer):
    AstNode(std::move(scope)),
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

DeclarationStatement::DeclarationStatement(std::shared_ptr<VariableScope> scope, std::unique_ptr<VariableDeclaration> decl):
    Statement(std::move(scope))
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

ExpressionStatement::ExpressionStatement(std::shared_ptr<VariableScope> scope, std::unique_ptr<Expression> expr):
    Statement(std::move(scope)),
    _expr(std::move(expr))
{
}

std::string ExpressionStatement::print()
{
    return _expr->print();
}

irl::IrlSegment code_gen()
{
    return irl::IrlSegment();
}