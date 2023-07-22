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

#genrule(
#    name='bar',
#    tools=[':arduino_cli_install'],
#    outs=['test.txt'],
#    cmd="echo $$(dirname $(location :arduino_cli_install)) > $@"
#)

#genrule(
#    name = "arduino_cli",
#    tools = [":arduino_cli_install"],
#    srcs = ["install.sh"],
#    outs = ["balancing_robot/bin/arduino-cli"],
#    executable = True,
#    cmd = "source $<",
#    visibility = ["//visibility:public"],
#)
