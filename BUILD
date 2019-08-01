cc_binary(
  name = 'lox',
  srcs = [ 'main.cpp' ],
  deps = [ ':ast-printer',
           ':error-reporter',
           ':parser',
           ':token',
           ':token-type',
           '@external//:gflags' ])

cc_library(
  name = 'ast',
  hdrs = [ 'ast.hpp' ])

cc_library(
  name = 'ast-printer',
  hdrs = [ 'ast-printer.hpp' ])

cc_library(
  name = 'error-reporter',
  hdrs = [ 'error-reporter.hpp' ])

cc_library(
  name = 'parser',
  hdrs = [ 'parser.hpp' ],
  deps = [ ':ast',
           ':error-reporter',
           ':scanner',
           ':token',
           ':token-type' ])

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
  name = 'parser-test',
  srcs = [ 'parser-test.cpp' ],
  deps = [ ':ast-printer',
           ':error-reporter',
           ':parser',
           '@external//:googletest' ])

cc_test(
  name = 'scanner-test',
  srcs = [ 'scanner-test.cpp' ],
  deps = [ ':error-reporter',
           ':scanner',
           ':token',
           ':token-type',
           '@external//:googletest' ])
