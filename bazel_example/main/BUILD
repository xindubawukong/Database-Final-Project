cc_library(
    name = "hello-greet",
    srcs = ["hello-greet.cc"],
    hdrs = ["hello-greet.h"],
)

cc_binary(
    name = "hello-world",
    srcs = ["hello-world.cc"],
    deps = [
        ":hello-greet",
        "//bazel_example/lib:hello-time",
    ],
)

cc_test(
    name = "hello-greet_test",
    srcs = ["hello-greet_test.cc"],
    deps = [
        "@gtest//:gtest_main",
        ":hello-greet",
    ],
)