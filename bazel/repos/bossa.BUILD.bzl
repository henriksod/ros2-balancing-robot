# Copyright (c) 2023, Henrik Söderlund

"""
Builds BOSSA
"""

SRCS = [
    "src/Samba.cpp",
    "src/Flash.cpp",
    "src/NvmFlash.cpp",
    "src/EfcFlash.cpp",
    "src/EefcFlash.cpp",
    "src/FlashFactory.cpp",
    "src/Applet.cpp",
    "src/WordCopyApplet.cpp",
    "src/Flasher.cpp",
    "src/PosixSerialPort.cpp",
    "src/LinuxPortFactory.cpp",
    "src/CmdOpts.cpp",
    "src/WordCopyArm.cpp",
]

HDRS = [
    "{}.h".format(f.removesuffix(".cpp"))
    for f in SRCS
] + [
    "src/SerialPort.h",
    "src/PortFactory.h",
    "src/FileError.h",
    "src/Devices.h",
]

MAIN = ["src/bossac.cpp"]

cc_binary(
    name = "bossac",
    srcs = SRCS + HDRS + MAIN,
    copts = ["-std=c++11"],
    defines = ["VERSION=\\\"1.9.1\\\""],
    visibility = ["//visibility:public"],
)
