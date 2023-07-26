# Testbed for ROS2 Balancing Robot

This is a testbed for my balancing robot, using ROS2 and Bazel.

The hardware can be reduced to the following components:
- Raspberry PI 4 Model B (aarch64, Raspberry PI OS Bullseye 64bit)
- Raspberry PI Camera V2
- Arduino Due ARM Cortex M3
- 2x NEMA17 Stepper motors
- Head Tilt Servo (Camera mounted on shaft)
- Front IR distance sensor (Sharp GP2Y0A21YK)
- 2x Side IR proximity sensor (Sharp GP2Y0D815Z0F)

Raspberry PI is running ROS2 and Arduino Due is running FreeRTOS and micro-ros.
They communicate over Serial using Micro-XRCE-DDS.

The buildsystem is Bazel and is used to remotely build and run software on the target platform.

## Requirements

This repository requires [Docker](https://www.docker.com/) and [Python 3](https://www.python.org/) to function.

```
sudo apt install docker-ce python3 python3-pip
```

Bazel is run via a [dazel](https://github.com/nadirizr/dazel) container.

Ensure arduino is installed on the raspberry pi:

```
sudo apt install arduino
```

## Entrypoint

```
./run.py --help
```