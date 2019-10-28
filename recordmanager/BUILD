cc_library(
  name = "rm_rid",
  srcs = ["rm_rid.cc"],
  hdrs = ["rm_rid.h"],
  deps = [
    "//:return_code",
  ],
  visibility = ["//visibility:public"],
)

cc_test(
  name = "rm_rid_test",
  srcs = ["rm_rid_test.cc"],
  deps = [
    "@gtest//:gtest_main",
    ":rm_rid",
  ]
)