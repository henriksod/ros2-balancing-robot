# Copyright (c) 2023, Henrik Söderlund

"""
A module that builds the micro ros agent
"""

load(
    "@com_github_mvukov_rules_ros2//ros2:cc_defs.bzl",
    "ros2_cpp_binary",
    "ros2_cpp_library",
)

ros2_cpp_library(
    name = "micro_ros_agent_lib",
    srcs = glob(["micro_ros_agent/src/agent/**/*.cpp"]),
    hdrs = glob(["micro_ros_agent/include/agent/**/*.hpp"]),
    includes = ["micro_ros_agent/include"],
    visibility = ["//visibility:public"],
    deps = [
        "@ros2_rclcpp//:rclcpp",
        "@com_github_micro_ros_micro_ros_msgs//:cpp_micro_ros_msgs",
        "@com_github_eprosima_micro_xrce_dds_agent//:micro_xrce_dds_agent",
        "@ros2_rmw_fastrtps//:rmw_fastrtps_shared_cpp",
        "@ros2_rosidl_typesupport_fastrtps//:rosidl_typesupport_fastrtps_cpp",
    ],
)

ros2_cpp_binary(
    name = "micro_ros_agent",
    srcs = ["micro_ros_agent/src/main.cpp"],
    linkstatic = False,
    visibility = ["//visibility:public"],
    deps = [
        ":micro_ros_agent_lib",
    ],
)
