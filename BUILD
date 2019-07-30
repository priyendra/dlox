cc_binary(
  name = 'lox',
  srcs = [ 'main.cpp' ],
  deps = [ ':error-reporter',
           ':scanner',
           ':token',
           ':token-type',
           '@external//:gflags' ])

cc_library(
  name = 'error-reporter',
  hdrs = [ 'error-reporter.hpp' ])

cc_library(
  name = 'scanner',
  hdrs = [ 'scanner.hpp' ],
  deps = [ ':error-reporter',
           ':token',
           ':token-type',
           '@external//:fmtlib' ])

cc_library(
  name = 'token-type',
  hdrs = [ 'token-type.hpp' ],
  srcs = [ 'token-type.cpp' ])

cc_library(
  name = 'token',
  hdrs = [ 'token.hpp' ],
  deps = [ ':token-type',
           '@external//:fmtlib' ])

cc_test(
  name = 'scanner-test',
  srcs = [ 'scanner-test.cpp' ],
  deps = [ ':error-reporter',
           ':scanner',
           ':token',
           ':token-type',
           '@external//:googletest' ])
