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