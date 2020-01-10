cc_library(
  name = "sm",
  srcs = ["sm.cc"],
  hdrs = ["sm.h"],
  deps = [
    "//:global",
    "//filesystem:filesystem",
    "//recordmanager:rm",
    "//indexing:ix",
  ],
  visibility = ["//visibility:public"],
)

cc_binary(
  name = "main",
  srcs = ["main.cc"],
  deps = [
    "//:global",
    "//filesystem:filesystem",
    "//recordmanager:rm",
    "//indexing:ix",
    "//systemmanager:sm",
  ],
  visibility = ["//visibility:public"],
)


cc_test(
  name = "sm_test",
  srcs = ["sm_test.cc"],
  deps = [
    "@gtest//:gtest_main",
    "//indexing:ix",
    "//filesystem:filesystem",
    "//recordmanager:rm",
    "//systemmanager:sm",
  ]
)