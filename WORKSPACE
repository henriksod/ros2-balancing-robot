# Copyright (c) 2023, Henrik Söderlund

workspace(name = "balancing_robot")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Toolchain: aarch64-linux-gnueabihf
http_archive(
    name = "aspect_gcc_toolchain",
    sha256 = "8850373f24d3f8bb6e8f36e3e8e7edc93d948964f8f201e920af2c8ffba2002c",
    strip_prefix = "gcc-toolchain-4bd1f94536ee92b7c49673931773038d923ee86e",
    url = "https://github.com/aspect-build/gcc-toolchain/archive/4bd1f94536ee92b7c49673931773038d923ee86e.tar.gz",
)

load("@aspect_gcc_toolchain//toolchain:repositories.bzl", "gcc_toolchain_dependencies")

gcc_toolchain_dependencies()

load("@bazel_skylib//:workspace.bzl", "bazel_skylib_workspace")

bazel_skylib_workspace()

load("@aspect_bazel_lib//lib:repositories.bzl", "aspect_bazel_lib_dependencies")

aspect_bazel_lib_dependencies()

load("@aspect_gcc_toolchain//toolchain:defs.bzl", "ARCHS", "gcc_register_toolchain")

gcc_register_toolchain(
    name = "gcc_toolchain_aarch64",
    target_arch = ARCHS.aarch64,
)

# In a normal workflow, you would typically import rules_ros2 into your
# (mono)repo as follows:
http_archive(
    name = "com_github_mvukov_rules_ros2",
    patch_args = ["-p1"],
    patches = ["//bazel/repos/patches:spdlog_fix.patch"],
    #patches = ["//bazel/patches:cyclonedds_pic.patch", "//bazel/patches:test.patch"],
    # Here you can use e.g. sha256sum cli utility to compute the sha sum.
    #sha256 = "c1ff135dd1a6a5c518357285611b1c4de4af6eb9249bf007a21479e35b1a6006",
    #strip_prefix = "rules_ros2-89dd5fa0add476e85a438c8575f353ecf6162c57",
    #url = "https://github.com/mvukov/rules_ros2/archive/89dd5fa0add476e85a438c8575f353ecf6162c57.tar.gz",
    sha256 = "0ffd8802b4915aa5c75db7187207a4746e5ad8a8079432d1e622e7bb251210cc",
    strip_prefix = "rules_ros2-82aa24da167725c0ba1a8d069a58fed0245bd2fa",
    url = "https://github.com/henriksod/rules_ros2/archive/82aa24da167725c0ba1a8d069a58fed0245bd2fa.tar.gz",
)

http_archive(
    name = "fastrtps",
    build_file = "@com_github_mvukov_rules_ros2//repositories:fastrtps.BUILD.bazel",
    sha256 = "3fe8b9f67a13a5d2aa40c0bd10581bd90f0a192b39c71f92ee233ffe584d3374",
    strip_prefix = "Fast-DDS-2.11.1",
    url = "https://github.com/eProsima/Fast-DDS/archive/refs/tags/v2.11.1.tar.gz",
)

load("@com_github_mvukov_rules_ros2//repositories:repositories.bzl", "ros2_repositories")

ros2_repositories()

load("@com_github_mvukov_rules_ros2//repositories:deps.bzl", "PIP_ANNOTATIONS", "ros2_deps")

ros2_deps()

load("@rules_python//python:repositories.bzl", "python_register_toolchains")

python_register_toolchains(
    name = "rules_ros2_python",
    python_version = "3.8.15",
)

load("@rules_python//python:pip.bzl", "pip_parse")
load("@rules_ros2_python//:defs.bzl", python_interpreter_target = "interpreter")

pip_parse(
    name = "rules_ros2_pip_deps",
    annotations = PIP_ANNOTATIONS,
    python_interpreter_target = python_interpreter_target,
    requirements_lock = "@com_github_mvukov_rules_ros2//:requirements_lock.txt",
)

load(
    "@rules_ros2_pip_deps//:requirements.bzl",
    install_rules_ros2_pip_deps = "install_deps",
)

install_rules_ros2_pip_deps()

# Rule repository, note that it's recommended to use a pinned commit to a released version of the rules
# http_archive(
#     name = "rules_foreign_cc",
#     sha256 = "9561b3994232ccb033278ade83c2ce48e763e9cae63452cd8fea457bedd87d05",
#     strip_prefix = "rules_foreign_cc-816905a078773405803e86635def78b61d2f782d",
#     url = "https://github.com/bazelbuild/rules_foreign_cc/archive/816905a078773405803e86635def78b61d2f782d.tar.gz",
# )

#load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

# This sets up some common toolchains for building targets. For more details, please see
# https://github.com/bazelbuild/rules_foreign_cc/tree/main/docs#rules_foreign_cc_dependencies
#rules_foreign_cc_dependencies()

http_archive(
    name = "rules_yaml",
    sha256 = "8541337f1208f18616cd03dcc7bf032e3e0eebc1455fd49b5a3360d404fb9080",
    url = "https://github.com/calebfroese/rules_yaml/releases/download/v1.0.0/rules_yaml-v1.0.0.tar.gz",
)

load("@rules_yaml//:defs.bzl", "yaml_repositories")

# This sets up the dependencies for rules_yaml
yaml_repositories()

http_archive(
    name = "rules_pkg",
    sha256 = "8f9ee2dc10c1ae514ee599a8b42ed99fa262b757058f65ad3c384289ff70c4b8",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/rules_pkg/releases/download/0.9.1/rules_pkg-0.9.1.tar.gz",
        "https://github.com/bazelbuild/rules_pkg/releases/download/0.9.1/rules_pkg-0.9.1.tar.gz",
    ],
)

load("@rules_pkg//:deps.bzl", "rules_pkg_dependencies")

rules_pkg_dependencies()

# Buildifier
# buildifier is written in Go and hence needs rules_go to be built.
# See https://github.com/bazelbuild/rules_go for the up to date setup instructions.
http_archive(
    name = "io_bazel_rules_go",
    sha256 = "6dc2da7ab4cf5d7bfc7c949776b1b7c733f05e56edc4bcd9022bb249d2e2a996",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/rules_go/releases/download/v0.39.1/rules_go-v0.39.1.zip",
        "https://github.com/bazelbuild/rules_go/releases/download/v0.39.1/rules_go-v0.39.1.zip",
    ],
)

load("@io_bazel_rules_go//go:deps.bzl", "go_register_toolchains", "go_rules_dependencies")

go_rules_dependencies()

go_register_toolchains(version = "1.20.3")

http_archive(
    name = "bazel_gazelle",
    sha256 = "727f3e4edd96ea20c29e8c2ca9e8d2af724d8c7778e7923a854b2c80952bc405",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/bazel-gazelle/releases/download/v0.30.0/bazel-gazelle-v0.30.0.tar.gz",
        "https://github.com/bazelbuild/bazel-gazelle/releases/download/v0.30.0/bazel-gazelle-v0.30.0.tar.gz",
    ],
)

load("@bazel_gazelle//:deps.bzl", "gazelle_dependencies")

# If you use WORKSPACE.bazel, use the following line instead of the bare gazelle_dependencies():
# gazelle_dependencies(go_repository_default_config = "@//:WORKSPACE.bazel")
gazelle_dependencies()

http_archive(
    name = "com_google_protobuf",
    sha256 = "3bd7828aa5af4b13b99c191e8b1e884ebfa9ad371b0ce264605d347f135d2568",
    strip_prefix = "protobuf-3.19.4",
    urls = [
        "https://github.com/protocolbuffers/protobuf/archive/v3.19.4.tar.gz",
    ],
)

load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")

protobuf_deps()

http_archive(
    name = "com_github_bazelbuild_buildtools",
    sha256 = "ae34c344514e08c23e90da0e2d6cb700fcd28e80c02e23e4d5715dddcb42f7b3",
    strip_prefix = "buildtools-4.2.2",
    urls = [
        "https://github.com/bazelbuild/buildtools/archive/refs/tags/4.2.2.tar.gz",
    ],
)

# Uncrustify, C/C++ code formatter
http_archive(
    name = "com_github_uncrustify_uncrustify",
    build_file = "//bazel/repos/uncrustify:uncrustify.BUILD.bzl",
    sha256 = "414bbc9f7860eb18a53074f9af14ed04638a633b2216a73f2629291300d37c1b",
    strip_prefix = "uncrustify-uncrustify-0.77.1",
    url = "https://github.com/uncrustify/uncrustify/archive/refs/tags/uncrustify-0.77.1.tar.gz",
)

#########################################
# Below are rules specific to this repo #
#########################################

pip_parse(
    name = "balancing_robot_pip_deps",
    python_interpreter_target = python_interpreter_target,
    requirements_lock = "//:requirements_lock.txt",
)

load(
    "@balancing_robot_pip_deps//:requirements.bzl",
    install_balancing_robot_pip_deps = "install_deps",
)

install_balancing_robot_pip_deps()

# cpp serial source code repository
http_archive(
    name = "com_github_wjwwood_serial",
    build_file = "//bazel/repos:serial.BUILD.bzl",
    patch_args = ["-p1"],
    patches = ["//bazel/repos/patches:wjwwood_serial_fix_warnings.patch"],
    sha256 = "0c2a789ce485a83ed640c777a7d1cd1256976890ece4c126f93751a08643917a",
    strip_prefix = "serial-1.2.1",
    url = "https://github.com/wjwwood/serial/archive/refs/tags/1.2.1.tar.gz",
)

###########
# Zephyr  #
###########

http_archive(
    name = "com_github_zephyrproject_rtos_sdk_ng_aarch64",
    build_file = "//bazel/repos:zephyr_sdk.BUILD.bzl",
    sha256 = "f34078627541b548310819968994559e05111733f93a7814c5cf9bc53b3a524a",
    strip_prefix = "zephyr-sdk-0.16.1",
    url = "https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.16.1/zephyr-sdk-0.16.1_linux-aarch64_minimal.tar.xz",
)

http_archive(
    name = "com_github_zephyrproject_rtos_sdk_ng_x86_64",
    build_file = "//bazel/repos:zephyr_sdk.BUILD.bzl",
    sha256 = "46449fdb0a69f83ec707dbc55a8d23ea44860f0a2bca9097442fc3f2f16b96c7",
    strip_prefix = "zephyr-sdk-0.16.1",
    url = "https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.16.1/zephyr-sdk-0.16.1_linux-x86_64_minimal.tar.xz",
)

http_archive(
    name = "com_github_zephyrproject_rtos_zephyr",
    build_file = "//bazel/repos:zephyr.BUILD.bzl",
    sha256 = "a066cb7e975f11f1f34d43807965f28ab300625dd0793688c134f4a0e6adbb4b",
    strip_prefix = "zephyr-3.4.0",
    url = "https://github.com/zephyrproject-rtos/zephyr/archive/refs/tags/v3.4.0.tar.gz",
)

###########
#  Zenoh  #
###########

http_archive(
    name = "com_github_eclipse_zenoh_zenoh",
    build_file = "//bazel/repos:zenoh.BUILD.bzl",
    sha256 = "bbd3387b02ed107b8fcbf971ecdee488b3a3f51bdeb7678ac0db6987bd5f04fa",
    strip_prefix = "zenoh-0.10.0-dev",
    url = "https://github.com/eclipse-zenoh/zenoh/archive/refs/tags/0.10.0-dev.tar.gz",
)

http_archive(
    name = "com_github_eclipse_zenoh_zenoh_plugin_dds_aarch64",
    build_file = "//bazel/repos:zenoh_bridge_dds.BUILD.bzl",
    sha256 = "e75b2c9d856bd4b604fef1ec9a8311222743079867d23b9cf446cdfe9491c049",
    #strip_prefix = "zenoh-bridge-dds-0.7.2-rc",
    url = "https://github.com/eclipse-zenoh/zenoh-plugin-dds/releases/download/0.7.2-rc/zenoh-bridge-dds-0.7.2-rc-aarch64-unknown-linux-gnu.zip",
)

http_archive(
    name = "com_github_eclipse_zenoh_zenoh_plugin_dds_x86_64",
    build_file = "//bazel/repos:zenoh_bridge_dds.BUILD.bzl",
    sha256 = "dcfd008a92c784e89ef2ee1ddf1831a34e7581350276a169971c2b00f4566997",
    #strip_prefix = "zenoh-bridge-dds-0.7.2-rc",
    url = "https://github.com/eclipse-zenoh/zenoh-plugin-dds/releases/download/0.7.2-rc/zenoh-bridge-dds-0.7.2-rc-x86_64-unknown-linux-gnu.zip",
)

http_archive(
    name = "com_github_eclipse_zenoh_zenoh_pico",
    build_file = "//bazel/repos:zenoh_pico.BUILD.bzl",
    sha256 = "59abb5e8d9d4d8b59cc8a25a9b7198bc421cb79a16b75c9af15e9f0013f4aac8",
    strip_prefix = "zenoh-pico-0.10.0-dev",
    url = "https://github.com/eclipse-zenoh/zenoh-pico/archive/refs/tags/0.10.0-dev.tar.gz",
)

###########
#  BOSSA  #
###########
http_archive(
    name = "com_github_shumatech_bossa",
    build_file = "//bazel/repos:bossa.BUILD.bzl",
    sha256 = "80f9033b614676965ccbd16fa892bfe4fa99a7510120f9d32f416447bff42a47",
    strip_prefix = "BOSSA-5cae9fee241bd3c95c197b2464e9b83240994c43",
    url = "https://github.com/shumatech/BOSSA/archive/5cae9fee241bd3c95c197b2464e9b83240994c43.tar.gz",
)

###########
# Arduino #
###########

# arduino-cli
http_archive(
    name = "com_github_arduino_arduino_cli",
    build_file = "//bazel/repos/arduino_cli:arduino_cli.BUILD.bzl",
    #patch_args = ["-p1"],
    #patches = ["//bazel/patches:remove_filename_spaces.patch"],
    #sha256 = "ef9a39fc965e99410e2825211f95836dab9f1f6028dc35f1178a148a42cf8836",
    #strip_prefix = "arduino-cli-0.33.1",
    #url = "https://github.com/arduino/arduino-cli/archive/refs/tags/0.33.1.tar.gz",
    sha256 = "c6b23c4a028d27c31eeb7e1126e22b9a87e5e5c5daada8b990194dabfae6a6c5",
    strip_prefix = "arduino-cli-a7f0de42fa04243b948c14741ee06f30098475ff",
    url = "https://github.com/arduino/arduino-cli/archive/a7f0de42fa04243b948c14741ee06f30098475ff.tar.gz",
)

# micro-xrce-dds-agent
http_archive(
    name = "com_github_eprosima_micro_xrce_dds_agent",
    build_file = "//bazel/repos:micro_xrce_dds_agent.BUILD.bzl",
    sha256 = "eae855e32fba142ffea8d91970aa0b5f14745293eb4ad03c9207b4e0ddbd5576",
    strip_prefix = "Micro-XRCE-DDS-Agent-2.4.1",
    url = "https://github.com/eProsima/Micro-XRCE-DDS-Agent/archive/refs/tags/v2.4.1.tar.gz",
)

# micro-ros-arduino
http_archive(
    name = "com_github_henriksod_micro_ros_arduino-cortex_m3-arm-none-eabi-gcc-7-2017q4",
    build_file = "//bazel/repos:micro_ros_arduino.BUILD.bzl",
    sha256 = "83d865f7af6a5ce2f718780286ef8a4e78b33bf47eb8ae34e11809d0bcafcf70",
    strip_prefix = "micro_ros_arduino-cortex_m3-arm-none-eabi-gcc-7-2017q4-2.0.7-humble",
    url = "https://github.com/henriksod/micro_ros_arduino-cortex_m3-arm-none-eabi-gcc-7-2017q4/archive/refs/tags/v2.0.7-humble.tar.gz",
)

# micro-ros-msgs
http_archive(
    name = "com_github_micro_ros_micro_ros_msgs",
    build_file = "//bazel/repos:micro_ros_msgs.BUILD.bzl",
    sha256 = "fa574bf2dc08ecd60896d830fb8b1ee5232cd59a93dbc8ffd2a058afae8b80ca",
    strip_prefix = "micro_ros_msgs-3.0.0",
    url = "https://github.com/micro-ROS/micro_ros_msgs/archive/refs/tags/3.0.0.tar.gz",
)

# micro-ros-agent
http_archive(
    name = "com_github_micro_ros_micro_ros_agent",
    build_file = "//bazel/repos:micro_ros_agent.BUILD.bzl",
    sha256 = "08d37137f4d2d5d29e535bcce3b72b42c70c785c59544fec8f90c7615feb1058",
    strip_prefix = "micro-ROS-Agent-3.0.5",
    url = "https://github.com/micro-ROS/micro-ROS-Agent/archive/refs/tags/3.0.5.tar.gz",
)
