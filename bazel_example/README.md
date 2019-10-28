This is an example to use bazel.

Run the following code in the same path with `WORKSPACE` file:

```shell
bazel build bazel_example/main:all bazel_example/lib:all  # build in debug mode

bazel build -c opt bazel_example/main:all bazel_example/lib:all  # build in optimization mode

bazel run bazel_example/main:hello-world  # run in debug mode

bazel test bazel_example/main:hello-greet_test  # run googletest

bazel test bazel_example/main:hello-greet_test --test_output=streamed  # run googletest with output details
```

