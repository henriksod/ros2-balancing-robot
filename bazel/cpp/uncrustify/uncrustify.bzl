"""
A module that builds the uncrustify formatting tool
"""

load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

filegroup(
    name = "all_srcs",
    srcs = glob(["**"]),
)

cmake(
    name = "uncrustify_build",
    cache_entries = {
        "CMAKE_BUILD_TYPE": "Release",
    },
    lib_source = ":all_srcs",
    out_binaries = ["uncrustify"],
)

sh_library(
    name = "uncrustify_build_sh",
    data = [":uncrustify_build"],
)

sh_binary(
    name = "uncrustify",
    srcs = ["@balancing_robot//bazel/cpp/uncrustify:exec.sh"],
    deps = [":uncrustify_build_sh"],
    visibility = ["//visibility:public"],
)
