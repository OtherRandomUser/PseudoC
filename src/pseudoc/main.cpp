#include <fstream>
#include <iostream>
#include <string>

#include <pseudoc/lexer.hpp>
#include <pseudoc/parser.hpp>

// int main()
// {
//     std::cout << "Digite o comando a ser processado" << std::endl;
//     std::cout << "$ ";

//     std::string var;
//     std::getline(std::cin, var);
//     Lexer lexer(var);

//     // std::cout << "Tokens:" << std::endl;

//     // while (!lexer.is_eof())
//     // {
//     //     auto token = lexer.bump();

//     //     std::cout << "    type: " << (int) token.tk_type << ", lexema: \"" << token.lexema << '"' << std::endl;
//     // }

//     auto ast = parse_expression(lexer);

//     std::cout << "Tokens:" << std::endl;
//     std::cout << ast->print() << std::endl;

//     return EXIT_SUCCESS;
// }

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

    std::cout << "Tokens:" << std::endl;

    while (!lexer.is_eof())
    {
        auto token = lexer.bump();

        std::cout << "    type: " << (int) token.tk_type << ", lexema: \"" << token.lexema << '"' << std::endl;
    }

    // auto ast = parse_expression(lexer);

    // std::cout << "Tokens:" << std::endl;
    // std::cout << ast->print() << std::endl;

    return EXIT_SUCCESS;
}