import os

all_bazel_tests = [
  'bazel_example/main:all',
  'recordmanager:all',
  'utils:all',
]

for test in all_bazel_tests:
  inst = 'bazel test ' + test + ' --test_output=streamed'
  os.system(inst)
  print('')
