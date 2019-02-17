load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "com_google_absl",
    sha256 = "888fadeb958033229263430e1a0543d6e37b6237164b2fe4c52b56f923b624ec",
    strip_prefix = "abseil-cpp-426eaa4aa44e4580418bee46c1bd13911151bfb1",
    urls = ["https://github.com/abseil/abseil-cpp/archive/426eaa4aa44e4580418bee46c1bd13911151bfb1.zip"],  # 2019-02-14
)

http_archive(
    name = "com_google_googletest",
    sha256 = "ff7a82736e158c077e76188232eac77913a15dac0b22508c390ab3f88e6d6d86",
    strip_prefix = "googletest-b6cd405286ed8635ece71c72f118e659f4ade3fb",
    urls = ["https://github.com/google/googletest/archive/b6cd405286ed8635ece71c72f118e659f4ade3fb.zip"],  # 2019-01-07
)

http_archive(
    name = "com_grail_bazel_toolchain",
    sha256 = "7ba2d72cd4e6f958aa104b2e091aaefe819ee86eccacd97229803b363a90eb45",
    strip_prefix = "bazel-toolchain-3622268bc3730868543e1fb963ac1948f1424700",
    urls = ["https://github.com/grailbio/bazel-toolchain/archive/3622268bc3730868543e1fb963ac1948f1424700.tar.gz"],
)

load("@com_grail_bazel_toolchain//toolchain:configure.bzl", "llvm_toolchain")

llvm_toolchain(
    name = "llvm_toolchain",
    llvm_version = "7.0.0",
)
