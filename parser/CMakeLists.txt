find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

set(CMAKE_CXX_STANDARD 11)

BISON_TARGET(Parser ${CMAKE_CURRENT_SOURCE_DIR}/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cpp
        VERBOSE ${CMAKE_CURRENT_BINARY_DIR}/parser.output)
FLEX_TARGET(Lexer ${CMAKE_CURRENT_SOURCE_DIR}/lex.l ${CMAKE_CURRENT_BINARY_DIR}/parser.yy.cpp)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)


add_library(SqlParser
        ${BISON_Parser_OUTPUT_SOURCE}
        ${FLEX_Lexer_OUTPUTS}
        ${BISON_Parser_OUTPUT_HEADER}
        )

set(SOURCE
        ${SOURCE}
        ${CMAKE_CURRENT_SOURCE_DIR}/ast.cc
        )

set(HEADERS
        ${HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/ast.hh
        )