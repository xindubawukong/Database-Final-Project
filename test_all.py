import os

os.system('bazel test bazel_example/main:all --test_output=streamed')
print('\n')
os.system('bazel test recordmanager:all --test_output=streamed')
