# Copyright (c) 2023, Henrik Söderlund

"""
A module that builds the cpp serial library
"""

cc_library(
    name = "cpp_serial",
    srcs = glob(["src/**/*.cc"]),
    hdrs = glob(["include/**/*.h"]),
    includes = ["include/"],
    visibility = ["//visibility:public"],
)
