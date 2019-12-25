cc_library(
  name = "btree_node",
  srcs = ["btree_node.cc"],
  hdrs = ["btree_node.h"],
  deps = [
    "//:global",
    "//filesystem:filesystem",
    "//recordmanager:rm_rid",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "ix",
  srcs = ["ix.cc"],
  hdrs = ["ix.h"],
  deps = [
    "//:global",
    "//:return_code",
    "//filesystem:filesystem",
    "//recordmanager:rm_rid",
    "//indexing:btree_node",
  ],
  visibility = ["//visibility:public"],
)

cc_test(
  name = "btree_node_test",
  srcs = ["btree_node_test.cc"],
  deps = [
    "@gtest//:gtest_main",
    "//indexing:btree_node",
  ],
)

