#include <pseudoc/ast/statement.hpp>

using namespace ast;

VariableDeclaration::VariableDeclaration(std::string identifier, std::unique_ptr<Expression> initializer):
    _identifier(std::move(identifier)),
    _initializer(std::move(initializer))
{
}

std::string VariableDeclaration::print()
{
    return "var (" + _identifier + " = " + (_initializer ? _initializer->print() : "<null>") +")";
}

std::unique_ptr<irl::IrlSegment> VariableDeclaration::code_gen(irl::Context context)
{
    auto segment =  std::make_unique<irl::IrlSegment>();

    // variable type
    auto tp = irl::LlvmAtomic::i32;

    // add variable & get temporary
    auto ref = _var_scope->add_variable(_identifier, tp);

    // alloc instruction
    segment->instructions.push_back(std::make_unique<irl::Alloca>(ref, 4));

    if (_initializer)
    {
        // initializer expr
        auto inner = _initializer->code_gen(std::move(context));

        // merge initializer instructions
        for (auto& i: inner->instructions)
        {
            segment->instructions.push_back(std::move(i));
        }

        // store value on variable
        segment->instructions.push_back(std::make_unique<irl::Store>(inner->out_value, ref, 4));
    }

    return segment;
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

std::unique_ptr<irl::IrlSegment> DeclarationStatement::code_gen(irl::Context context)
{
    auto segment =  std::make_unique<irl::IrlSegment>();

    for (auto& d: _decls)
    {
        auto inner = d->code_gen(context);

        for (auto& i: inner->instructions)
        {
            segment->instructions.push_back(std::move(i));
        }
    }

    return segment;
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

std::unique_ptr<irl::IrlSegment> ExpressionStatement::code_gen(irl::Context context)
{
    auto segment = _expr->code_gen(std::move(context));
    return segment;
}

ReturnStatement::ReturnStatement(std::unique_ptr<Expression> expr):
    _expr(std::move(expr))
{
}

std::string ReturnStatement::print()
{
    return "ret " + _expr->print();
}

std::unique_ptr<irl::IrlSegment> ReturnStatement::code_gen(irl::Context context)
{
    auto segment = std::make_unique<irl::IrlSegment>();
    auto expr = _expr->code_gen(std::move(context));

    for (auto& i: expr->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    segment->instructions.push_back(std::make_unique<irl::Ret>(std::move(expr->out_value), _expr->get_type()));

    return segment;
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

std::unique_ptr<irl::IrlSegment> CompoundStatement::code_gen(irl::Context context)
{
    auto segment = std::make_unique<irl::IrlSegment>();

    for (auto& statement: _statements)
    {
        auto inner = statement->code_gen(context);

        for (auto& i: inner->instructions)
        {
            segment->instructions.push_back(std::move(i));
        }
    }

    return segment;
}