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
    patches = ["//bazel/patches:cyclonedds_pic.patch"],
    # Here you can use e.g. sha256sum cli utility to compute the sha sum.
    sha256 = "c1ff135dd1a6a5c518357285611b1c4de4af6eb9249bf007a21479e35b1a6006",
    strip_prefix = "rules_ros2-89dd5fa0add476e85a438c8575f353ecf6162c57",
    url = "https://github.com/mvukov/rules_ros2/archive/89dd5fa0add476e85a438c8575f353ecf6162c57.tar.gz",
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

# Get buildfarm targets
http_archive(
    name = "build_buildfarm",
    sha256 = "8b77ebacaac93b12b7b1cc7ab3780e246e9f779555d906d00a701a1780e1c597",
    strip_prefix = "bazel-buildfarm-2.4.0",
    url = "https://github.com/bazelbuild/bazel-buildfarm/archive/refs/tags/2.4.0.tar.gz",
)

load("@build_buildfarm//:deps.bzl", "buildfarm_dependencies")

buildfarm_dependencies()

load("@build_buildfarm//:defs.bzl", "buildfarm_init")

buildfarm_init()

# Uncrustify, C/C++ code formatter
http_archive(
    name = "com_github_uncrustify_uncrustify",
    build_file = "//bazel/cpp/uncrustify:uncrustify.bzl",
    sha256 = "414bbc9f7860eb18a53074f9af14ed04638a633b2216a73f2629291300d37c1b",
    strip_prefix = "uncrustify-uncrustify-0.77.1",
    url = "https://github.com/uncrustify/uncrustify/archive/refs/tags/uncrustify-0.77.1.tar.gz",
)

# Toolchain: aarch64-linux-gnueabihf
#http_archive(
#    name = "rpi_bazel",
#    sha256 = "ef22526864f46d4bc42b09b421050697ebc1970f279f196b8f855048df6f3e3e",
#    strip_prefix = "rpi_bazel-964b6feb8bb14b2a58876b406f17266538794c3a",
#    url = "https://github.com/mjbots/rpi_bazel/archive/964b6feb8bb14b2a58876b406f17266538794c3a.zip",
#)

#load(
#    "@rpi_bazel//tools/workspace:default.bzl",
#    rpi_bazel_deps = "add_default_repositories",
#)

#rpi_bazel_deps()

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
    build_file = "//bazel/cpp:serial.bzl",
    patch_args = ["-p1"],
    patches = ["//bazel/patches:wjwwood_serial_fix_warnings.patch"],
    sha256 = "0c2a789ce485a83ed640c777a7d1cd1256976890ece4c126f93751a08643917a",
    strip_prefix = "serial-1.2.1",
    url = "https://github.com/wjwwood/serial/archive/refs/tags/1.2.1.tar.gz",
)

###########
# Arduino #
###########

# arduino-cli
http_archive(
    name = "com_github_arduino_arduino_cli",
    build_file = "//bazel/arduino:arduino_cli.bzl",
    #patch_args = ["-p1"],
    #patches = ["//bazel/patches:remove_filename_spaces.patch"],
    #sha256 = "ef9a39fc965e99410e2825211f95836dab9f1f6028dc35f1178a148a42cf8836",
    #strip_prefix = "arduino-cli-0.33.1",
    #url = "https://github.com/arduino/arduino-cli/archive/refs/tags/0.33.1.tar.gz",
    sha256 = "c6b23c4a028d27c31eeb7e1126e22b9a87e5e5c5daada8b990194dabfae6a6c5",
    strip_prefix = "arduino-cli-a7f0de42fa04243b948c14741ee06f30098475ff",
    url = "https://github.com/arduino/arduino-cli/archive/a7f0de42fa04243b948c14741ee06f30098475ff.tar.gz",
)

# micro-ros-agent
http_archive(
    name = "com_github_henriksod_micro_ros_arduino-cortex_m3-arm-none-eabi-gcc-7-2017q4",
    build_file = "//bazel/arduino:micro_ros_arduino.bzl",
    sha256 = "e9aba330222c346e0fb5241ddb4347306a045ca7ed8031fd7a259f492028a6b1",
    strip_prefix = "micro_ros_arduino-cortex_m3-arm-none-eabi-gcc-7-2017q4-a476b2c6ee8001af20acd1aa56fc609927d57efd",
    url = "https://github.com/henriksod/micro_ros_arduino-cortex_m3-arm-none-eabi-gcc-7-2017q4/archive/a476b2c6ee8001af20acd1aa56fc609927d57efd.tar.gz",
)
