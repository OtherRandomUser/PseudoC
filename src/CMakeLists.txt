add_executable(pseudoc
    pseudoc/ast
    pseudoc/ast/base
    pseudoc/ast/expression
    pseudoc/ast/function
    pseudoc/ast/statement
    pseudoc/irl
    pseudoc/main
    pseudoc/lexer
    pseudoc/parser
)

set_source_files_properties(
    pseudoc/ast
    pseudoc/irl
    PROPERTIES HEADER_FILE_ONLY ON
)

target_include_directories(pseudoc
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)