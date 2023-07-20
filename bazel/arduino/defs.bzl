
# Uploads an Arduino sketch to the board specified in config yaml file
#
# Config Example:
#   serial: '/dev/ttyACM0'
#   platform: 'arduino:avr:mega'
#
# Accepted libraries:
#   - Github link (https://github.com/*/*.git)
#   - ZIP file (*.zip)
#   - Index name from Arduino Library Manager
#
def upload_arduino_sketch(name, config, sketch, libraries, visibility=None):
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
