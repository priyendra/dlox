new_local_repository(
  name = 'external',
  path = '/usr',
  build_file_content =
"""
cc_library(
  name = "fmtlib",
  srcs = ["local/lib/libfmt.a"],
  visibility = ["//visibility:public"],
)
cc_library(
  name = 'gflags',
  srcs = ['local/lib/libgflags.a'],
  linkopts = ['-pthread'],
  visibility = ['//visibility:public'],
)
cc_library(
  name = 'googletest',
  srcs = ['local/lib/libgoogletest.a'],
  linkopts = ['-pthread'],
  visibility = ['//visibility:public'],
)
"""
)
