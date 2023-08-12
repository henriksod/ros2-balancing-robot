# Copyright (c) 2023, Henrik Söderlund

"""
Builds zephyr SDK
"""

filegroup(
    name = "all_srcs",
    srcs = glob(["**"]),
    visibility = ["//visibility:public"],
)
