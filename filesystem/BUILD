cc_library(
    name = "filesystem",
    hdrs = glob(["**/*.h"]),
    srcs = glob(["**/*.cc"]),
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "testfilesystem",
    srcs = ["testfilesystem.cc"],
    deps = [
        ":filesystem"
    ],
)