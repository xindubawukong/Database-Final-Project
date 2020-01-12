cc_library(
  name = "ast",
  srcs = ["ast.cc"],
  hdrs = ["ast.hh"],
  deps = [
    "//:global",
    "//systemmanager:sm",
    "//recordmanager:rm",
    "//indexing:ix",
    "//querylanguage:ql",
  ],

  visibility = ["//visibility:public"],
)

cc_library(
  name = "parser",
  srcs = [ "parser.tab.cpp"],
  hdrs = ["parser.tab.hpp", "parser.yy.cpp"],
  deps = [
    "//:global",
    "//parser:ast",
  ],

  visibility = ["//visibility:public"],
)





