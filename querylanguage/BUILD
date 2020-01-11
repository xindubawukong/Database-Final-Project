cc_library(
  name = "ql",
  srcs = ["ql.cc"],
  hdrs = ["ql.h"],
  deps = [
    "//:global",
    "//recordmanager:rm",
    "//indexing:ix",
    "//systemmanager:sm",
  ],
  visibility = ["//visibility:public"],
)

cc_binary(
    name = "test_ql",
    srcs = ["test_ql.cc"],
    deps = [
        ":ql"
    ],
)