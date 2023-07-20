workspace(name = "balancing_robot")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# In a normal workflow, you would typically import rules_ros2 into your
# (mono)repo as follows:
http_archive(
    name = "com_github_mvukov_rules_ros2",
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
    python_version = "3.8.13",
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
http_archive(
   name = "rules_foreign_cc",
   sha256 = "c2cdcf55ffaf49366725639e45dedd449b8c3fe22b54e31625eb80ce3a240f1e",
   strip_prefix = "rules_foreign_cc-0.1.0",
   url = "https://github.com/bazelbuild/rules_foreign_cc/archive/0.1.0.zip",
)

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

# This sets up some common toolchains for building targets. For more details, please see
# https://github.com/bazelbuild/rules_foreign_cc/tree/main/docs#rules_foreign_cc_dependencies
rules_foreign_cc_dependencies()

http_archive(
    name = "rules_yaml",
    sha256 = "8541337f1208f18616cd03dcc7bf032e3e0eebc1455fd49b5a3360d404fb9080",
    url = "https://github.com/calebfroese/rules_yaml/releases/download/v1.0.0/rules_yaml-v1.0.0.tar.gz",
)

load("@rules_yaml//:defs.bzl", "yaml_repositories")

# This sets up the dependencies for rules_yaml
yaml_repositories()

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
http_archive(
    name = "rules_pkg",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/rules_pkg/releases/download/0.9.1/rules_pkg-0.9.1.tar.gz",
        "https://github.com/bazelbuild/rules_pkg/releases/download/0.9.1/rules_pkg-0.9.1.tar.gz",
    ],
    sha256 = "8f9ee2dc10c1ae514ee599a8b42ed99fa262b757058f65ad3c384289ff70c4b8",
)
load("@rules_pkg//:deps.bzl", "rules_pkg_dependencies")
rules_pkg_dependencies()

#########################################
# Below are rules specific to this repo #
#########################################

pip_parse(
    name = "balancing_robot_pip_deps",
    requirements_lock = "//:requirements_lock.txt",
    python_interpreter_target = python_interpreter_target,
)

load(
    "@balancing_robot_pip_deps//:requirements.bzl",
    install_balancing_robot_pip_deps = "install_deps",
)

install_balancing_robot_pip_deps()

# cpp serial source code repository
http_archive(
    name = "com_github_wjwwood_serial",
    patch_args = ["-p1"],
    patches = ["//bazel/patches:wjwwood_serial_fix_warnings.patch"],
    build_file = "//bazel/cpp:serial.bazel",
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
    build_file = "//bazel/arduino:arduino_cli.bazel",
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
    name = "com_github_micro_ros_micro_ros_arduino",
    build_file = "//bazel/arduino:micro_ros_arduino.bazel",
    sha256 = "82b934bce6586a9487b0b36bcaf48d21b66ad2afe7093c0a3813362660f94fbc",
    strip_prefix = "micro_ros_arduino-2.0.7-humble",
    url = "https://github.com/micro-ROS/micro_ros_arduino/archive/refs/tags/v2.0.7-humble.tar.gz",
)



#https://github.com/micro-ROS/micro_ros_arduino/archive/refs/tags/v2.0.7-humble.tar.gz 82b934bce6586a9487b0b36bcaf48d21b66ad2afe7093c0a3813362660f94fbc
