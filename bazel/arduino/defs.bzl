"""
This module contains arduino compilation and uploading macros
"""

def upload_arduino_sketch(name, config, sketch, libraries, visibility = None):
    """Uploads an Arduino sketch to the board specified in config yaml file.

    Config Example:
        serial: '/dev/ttyACM0'
        platform: 'arduino:avr:mega'

    Accepted libraries:
        - Github link (https://github.com/*/*.git)
        - ZIP file (*.zip)
        - Index name from Arduino Library Manager

    Args:
        name: A unique name for this rule.
        config: A yaml file or label with the properties defined as the above example.
        sketch: An .ino sketch file or label
        libraries: A list of libraries to install, following one of the formats above.
        visibility: The visibility of this rule.
    """
    libraries_path = []
    additional_data = []
    for lib in libraries:
        if lib.startswith("@") or lib.startswith("//") or lib.startswith(":"):
            libraries_path.append("$(location {})".format(lib))
            additional_data.append(lib)
        else:
            libraries_path.append(lib)
    native.sh_binary(
        name = name,
        srcs = ["//bazel/arduino:upload_arduino_sketch.sh"],
        deps = [
            "@com_github_arduino_arduino_cli//:arduino_cli_install",
        ],
        data = [config, sketch] + additional_data,
        args = [
            "$(location {})".format(config),
            "$(location {})".format(sketch),
        ] + libraries_path,
        visibility = visibility,
    )
