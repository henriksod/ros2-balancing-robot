# Copyright (c) 2023, Henrik Söderlund

"""
A module that builds the uncrustify formatting tool
"""

load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

filegroup(
    name = "all_srcs",
    srcs = glob(["**"]),
)

cache_entries = {
    "CMAKE_POSITION_INDEPENDENT_CODE": "ON",  # Must be set!
}

cmake(
    name = "uncrustify_build",
    cache_entries = cache_entries,
    lib_source = ":all_srcs",
    out_binaries = ["uncrustify"],
)

sh_library(
    name = "uncrustify_build_sh",
    data = [":uncrustify_build"],
)

sh_binary(
    name = "uncrustify",
    srcs = ["@balancing_robot//bazel/repos/uncrustify:exec.sh"],
    deps = [":uncrustify_build_sh"],
    visibility = ["//visibility:public"],
)
