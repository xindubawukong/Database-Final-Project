cc_library(
  name = "rm",
  srcs = ["rm.cc"],
  hdrs = ["rm.h"],
  deps = [
    "//:return_code",
    "//filesystem:filesystem",
    "//utils:bitmap",
    ":rm_rid",
    ":rm_record",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "rm_rid",
  srcs = ["rm_rid.cc"],
  hdrs = ["rm_rid.h"],
  deps = [
    "//:return_code",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "rm_record",
  srcs = ["rm_record.cc"],
  hdrs = ["rm_record.h"],
  deps = [
    "//:return_code",
    ":rm_rid",
  ]
)

cc_test(
  name = "rm_test",
  srcs = ["rm_test.cc"],
  deps = [
    "@gtest//:gtest_main",
    ":rm",
    "//:return_code",
  ]
)

cc_test(
  name = "rm_rid_test",
  srcs = ["rm_rid_test.cc"],
  deps = [
    "@gtest//:gtest_main",
    ":rm_rid",
    "//:return_code",
  ]
)

cc_test(
  name = "rm_record_test",
  srcs = ["rm_record_test.cc"],
  deps = [
    "@gtest//:gtest_main",
    ":rm_record",
    "//:return_code",
  ]
)