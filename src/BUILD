cc_library(
    name = "board",
    srcs = ["board.cc"],
    hdrs = ["board.h"],
    deps = [
        "@com_google_absl//absl/base",
        "@com_google_absl//absl/strings",
    ],
)

cc_test(
    name = "board_test",
    srcs = ["board_test.cc"],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":board",
        "@com_google_absl//absl/strings",
        "@com_google_googletest//:gtest_main",
    ],
)
