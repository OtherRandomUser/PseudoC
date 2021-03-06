#include <fstream>
#include <iostream>
#include <string>

#include <pseudoc/lexer.hpp>
#include <pseudoc/parser.hpp>

int main(int argc, char **argv)
{
    // TODO use a lib
    if (argc != 2)
    {
        std::cout << "usage:" << std::endl << "pseudoc <source-file>" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream ifs(argv[1]);

    if (!ifs.is_open())
    {
        std::cout << "could not open source file\"" << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    std::string src((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    Lexer lexer(src);
    auto ftable = std::make_shared<FunctionTable>();

    irl::Context base_context;
    base_context.break_label = nullptr;
    base_context.continue_label = nullptr;

    while (!lexer.is_eof())
    {
        auto ast = parse_definition(lexer);
        auto scope = std::make_shared<VariableScope>();

        std::cout << "Definition:" << std::endl;
        std::cout << ast->print() << std::endl << std::endl;

        std::cout << "Code Gen" << std::endl;

        ast->set_variable_scope(scope, ftable);
        auto segment = ast->code_gen(base_context);

        std::cout << segment->print() << std::endl << std::endl;
    }

    return EXIT_SUCCESS;
}