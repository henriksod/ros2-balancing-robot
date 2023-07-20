#!/bin/bash

set -euo pipefail

# Find arduino-cli and install it if not found
path="$(pwd)/bin/arduino-cli"
if [[ ! -f "${path:-}" ]]; then
  ./external/com_github_arduino_arduino_cli/install.sh
  if [[ ! -f "${path:-}" ]]; then
    echo >&2 "ERROR: could not look up the arduino-cli path"
    exit 1
  fi
fi

echo >&2 "INFO: Working directory $(pwd)"
echo >&2 "INFO: Connected boards $($path board list)"

# Parse config
platform=$(cat $1 | grep -oP '(?<=platform: ).*')
port=$(cat $1 | grep -oP '(?<=serial: ).*')
sketch=$2

arduino_dir="$($path config dump | grep -oP '(?<=data: )[a-zA-z\/.0-9-_]+')"
sam_version="$($path core list | grep -oP '(?<=arduino:sam )[0-9.]+')"
sam_hardware_path="$arduino_dir/packages/arduino/hardware/sam/$sam_version"
curl https://raw.githubusercontent.com/micro-ROS/micro_ros_arduino/iron/extras/patching_boards/platform_arduinocore_sam.txt > $sam_hardware_path/platform.txt

# Install libraries
# $path config set library.enable_unsafe_install true
$path config dump > "arduino_cli_config.yaml"
sed -i 's/enable_unsafe_install: false/enable_unsafe_install: true/g' arduino_cli_config.yaml
if [ $# > 2 ]; then
  for i in "${@:3}"; do
    var=$i
    if [ "$(echo $var | grep -oP 'https://')" = "https://" ]; then
        $path lib install --git-url --config-file "arduino_cli_config.yaml" $var
    elif [ "$(echo $var | grep -oP '\.zip')" = ".zip" ]; then
        $path lib install --zip-path --config-file "arduino_cli_config.yaml" "$(pwd)/$var"
    else
        $path lib install $var
    fi
  done
fi

$path core install $(echo $platform | grep -oP '(.*)(?=:)')
$path compile --fqbn $platform $sketch -v
$path upload -p $port --fqbn $platform $sketch -v
