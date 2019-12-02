cc_library(
  name = "bitmap",
  srcs = ["bitmap.cc"],
  hdrs = ["bitmap.h"],
  deps = [
    "//:return_code",
  ],
  visibility = ["//visibility:public"],
)

cc_test(
  name = "bitmap_test",
  srcs = ["bitmap_test.cc"],
  deps = [
    "@gtest//:gtest_main",
    ":bitmap",
    "//:return_code",
  ]
)