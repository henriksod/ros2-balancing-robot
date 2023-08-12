# Copyright (c) 2023, Henrik Söderlund

"""
Builds micro_ros_msgs
"""

load(
    "@com_github_mvukov_rules_ros2//ros2:interfaces.bzl",
    "cpp_ros2_interface_library",
    "ros2_interface_library",
)

ros2_interface_library(
    name = "micro_ros_msgs",
    srcs = glob(["msg/*.msg"]),
    visibility = ["//visibility:public"],
)

cpp_ros2_interface_library(
    name = "cpp_micro_ros_msgs",
    visibility = ["//visibility:public"],
    deps = [
        ":micro_ros_msgs",
    ],
)
