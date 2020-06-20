add_executable(pseudoc
    pseudoc/ast
    pseudoc/ast/base
    pseudoc/ast/definition
    pseudoc/ast/expression
    pseudoc/ast/flow
    pseudoc/ast/statement
    pseudoc/irl
    pseudoc/irl/generator
    pseudoc/irl/instructions
    pseudoc/irl/segment
    pseudoc/irl/type
    pseudoc/irl/value
    pseudoc/lexer
    pseudoc/main
    pseudoc/parser
    pseudoc/parser/definition
    pseudoc/parser/expression
    pseudoc/parser/statement
    pseudoc/variable-map
)

# set_source_files_properties(
#     pseudoc/ast
#     pseudoc/irl
#     pseudoc/parser
#     PROPERTIES HEADER_FILE_ONLY ON
# )

target_include_directories(pseudoc
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)