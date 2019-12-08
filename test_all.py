import os

all_bazel_tests = [
  '//:all',
  'bazel_example/main:all',
  'recordmanager:all',
  'utils:all',
]

inst = 'bazel clean'
print(inst)
os.system(inst)
print('')

for test in all_bazel_tests:
  inst = 'bazel test ' + test  # + ' --test_output=streamed'
  print(inst)
  os.system(inst)
  print('')
