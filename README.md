This project is the final project of Database class in Tsinghua University.

Team member: 丁相允，尹一帆.

The database is implemented in C++. Codes are build with <a href="https://www.bazel.build/">bazel</a>. Please follow the <a href="https://google.github.io/styleguide/cppguide.html">Google C++ Style Guide</a>.

## Usage

### Use Bazel

Directory `bazel_example` is an example to use Google bazel. Run the following code in the same path with `WORKSPACE` file:

```shell
bazel clean  # clean cached build files

bazel build bazel_example/main:all bazel_example/lib:all  # build in debug mode

bazel build -c opt bazel_example/main:all bazel_example/lib:all  # build in optimization mode

bazel run bazel_example/main:hello-world  # run in debug mode

bazel run -c opt bazel_example/main:hello-world  # run in optimization mode

bazel test bazel_example/main:hello-greet_test  # run googletest

bazel test bazel_example/main:hello-greet_test --test_output=streamed  # run googletest with output details
```

### Run All Unit Tests

```shell
python3 test_all.py
```

### Run database

```shell
bazel run :sql
```

## References

https://web.stanford.edu/class/cs346/2015/