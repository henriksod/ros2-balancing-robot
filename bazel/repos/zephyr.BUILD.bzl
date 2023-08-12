# Copyright (c) 2023, Henrik Söderlund

"""
Builds Zephyr
"""

load("@balancing_robot_pip_deps//:requirements.bzl", "requirement")
load("@com_github_mvukov_rules_ros2//third_party:expand_template.bzl", "expand_template")

APP = "@balancing_robot//arduino/zephyr:hello_world"

filegroup(
    name = "all_srcs",
    srcs = glob(["**"]),
)

expand_template(
    name = "setup_zephyr_script",
    out = "setup_zephyr.py",
    is_executable = True,
    data = [APP],
    substitutions = {
        "@@@APP_PATH@@@": "$(rootpath {})".format(APP),
    },
    template = "@balancing_robot//deployment/zephyr:setup_zephyr.py.template",
    visibility = ["//visibility:public"],
)

py_binary(
    name = "setup_build",
    srcs = [":setup_zephyr_script"],
    main = "setup_zephyr.py",
    data = select({
        "@platforms//cpu:x86_64": [
            APP,
            ":all_srcs",
            "@com_github_zephyrproject_rtos_sdk_ng_x86_64//:all_srcs",
        ],
        "@platforms//cpu:aarch64": [
            APP,
            ":all_srcs",
            "@com_github_zephyrproject_rtos_sdk_ng_aarch64//:all_srcs",
        ],
    }),
    visibility = ["//visibility:public"],
    deps = [
        requirement("loguru"),
        requirement("west"),
        requirement("wget"),
    ],
)
