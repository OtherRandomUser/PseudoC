add_executable(pseudo
    pseudoc/ast
    pseudoc/main
    pseudoc/lexer
    pseudoc/parser
)

target_include_directories(randomc
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)