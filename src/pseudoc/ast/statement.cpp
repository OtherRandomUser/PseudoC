#include <pseudoc/ast/statement.hpp>

using namespace ast;

VariableDeclaration::VariableDeclaration(std::string identifier, std::unique_ptr<Expression> initializer):
    _identifier(std::move(identifier)),
    _initializer(std::move(initializer))
{
}

std::string VariableDeclaration::print()
{
    return "var (" + _identifier + " = " + _initializer->print() +")";
}

std::unique_ptr<irl::IrlSegment> VariableDeclaration::code_gen(std::shared_ptr<TempVariableGenerator> temp_gen)
{
    auto segment =  std::make_unique<irl::IrlSegment>();

    // variable type
    irl::LlvmType tp;
    tp.atomic = irl::LlvmAtomic::i32;
    tp.ptr_level = 0;

    // add variable
    _var_scope->add_variable(_identifier, tp);

    // get temporary
    auto ref = temp_gen->get_ref_value(_identifier);

    // alloc instruction
    segment->instructions.push_back(std::make_unique<irl::Alloca>(ref, 4));

    // initializer expr
    auto inner = _initializer->code_gen(temp_gen);

    // merge initializer instructions
    for (auto& i: inner->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    // store value on variable
    segment->instructions.push_back(std::make_unique<irl::Store>(inner->out_value, ref, 4));

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

std::unique_ptr<irl::IrlSegment> DeclarationStatement::code_gen(std::shared_ptr<TempVariableGenerator> temp_gen)
{
    auto segment =  std::make_unique<irl::IrlSegment>();

    for (auto& d: _decls)
    {
        auto inner = d->code_gen(temp_gen);

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

std::unique_ptr<irl::IrlSegment> ExpressionStatement::code_gen(std::shared_ptr<TempVariableGenerator> temp_gen)
{
    auto segment = _expr->code_gen(temp_gen);
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

std::unique_ptr<irl::IrlSegment> CompoundStatement::code_gen(std::shared_ptr<TempVariableGenerator> temp_gen)
{
    auto segment = std::make_unique<irl::IrlSegment>();

    // TODO create a new subordinate temp gen to prevent variables leaking from scope
    temp_gen->set_variable_scope(_var_scope);

    for (auto& statement: _statements)
    {
        auto inner = statement->code_gen(temp_gen);

        for (auto& i: inner->instructions)
        {
            segment->instructions.push_back(std::move(i));
        }
    }

    return segment;
}