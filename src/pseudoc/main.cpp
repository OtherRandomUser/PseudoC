#include <iostream>
#include <string>

#include <pseudoc/lexer.hpp>
#include <pseudoc/parser.hpp>

int main()
{
    std::cout << "Digite o comando a ser processado" << std::endl;
    std::cout << "$ ";

    std::string var;
    std::getline(std::cin, var);
    Lexer lexer(var);

    // std::cout << "Tokens:" << std::endl;

    // while (!lexer.is_eof())
    // {
    //     auto token = lexer.bump();

    //     std::cout << "    type: " << (int) token.tk_type << ", lexema: \"" << token.lexema << '"' << std::endl;
    // }

    auto ast = parse_expression(lexer);

    std::cout << "Tokens:" << std::endl;
    std::cout << ast->print() << std::endl;

    return EXIT_SUCCESS;
}