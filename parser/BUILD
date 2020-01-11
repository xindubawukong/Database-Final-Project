cc_library(
  name = "ast",
  srcs = ["ast.cc"],
  hdrs = ["ast.h"],
  deps = [
    "//:global",
    "//systemmanager:sm",
  ],

  visibility = ["//visibility:public"],
)

cc_library(
  name = "parser",
  srcs = ["parser.yy.cpp", "parser.tab.cpp", "parser.tab.hpp"],
  hdrs = ["parser.yy.cpp", "parser.tab.cpp", "parser.tab.hpp"],
  deps = [
    "//:global",
    "//parser:ast",
  ],
  linkstatic = True,

  visibility = ["//visibility:public"],
)

cc_import(
  name = "parser_1",
  static_library = "libSqlParser.a",
  visibility = ["//visibility:public"],
)




