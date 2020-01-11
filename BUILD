cc_library(
  name = "return_code",
  hdrs = ["return_code.h"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "global",
  hdrs = ["global.h"],
  srcs = ["global.cc"],
  visibility = ["//visibility:public"],
)

cc_test(
  name = "global_test",
  srcs = ["global_test.cc"],
  deps = [
    "@gtest//:gtest_main",
    ":global",
  ]
)



cc_binary(
  name = "sql",
  srcs = ["main.cpp"],
  deps = [
    "//parser:parser_1",
    "//filesystem:filesystem",
  ],
  linkstatic = 1,
)