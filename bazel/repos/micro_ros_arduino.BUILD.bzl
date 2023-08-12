# Copyright (c) 2023, Henrik Söderlund

"""
A module that builds the micro-ros-arduino library
"""

load("@rules_pkg//:pkg.bzl", "pkg_zip")
load("@rules_pkg//pkg:mappings.bzl", "pkg_files", "strip_prefix")

filegroup(
    name = "all_srcs",
    srcs = glob(["**"]),
)

pkg_files(
    name = "srcs",
    srcs = [":all_srcs"],
    strip_prefix = strip_prefix.from_pkg(),
)

pkg_zip(
    name = "micro_ros_arduino_zip",
    srcs = ["srcs"],
    package_dir = "micro_ros_arduino",
    visibility = ["//visibility:public"],
)
