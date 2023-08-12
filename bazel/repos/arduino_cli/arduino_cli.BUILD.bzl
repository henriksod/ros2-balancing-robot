# Copyright (c) 2023, Henrik Söderlund

"""
A module that builds the arduino-cli tool
"""

filegroup(
    name = "all_srcs",
    srcs = glob(["**"]),
    visibility = ["//visibility:public"],
)

sh_library(
    name = "arduino_cli_install",
    srcs = ["install.sh"],
    data = [":all_srcs"],
    visibility = ["//visibility:public"],
)
