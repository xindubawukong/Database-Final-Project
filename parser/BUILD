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
  srcs = [ "parser.tab.cpp"],
  hdrs = ["parser.tab.hpp", "stack.hh", "parser.yy.cpp"],
  deps = [
    "//:global",
    "//parser:ast",
  ],

  visibility = ["//visibility:public"],
)





