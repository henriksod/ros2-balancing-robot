#!/usr/bin/env python3

# Copyright (c) 2023, Henrik Söderlund

import os
import sys
import json
from support import check_packages

check_packages("pyyaml")
import yaml  # noqa

with open(f"{os.getcwd()}/config/config.yaml") as f:
    os.environ["ROS2_BALANCING_ROBOT_CONFIG"] = json.dumps(yaml.safe_load(f))

# Set environment variable to point to this file
os.environ["ROS2_BALANCING_ROBOT_ENTRYPOINT"] = __file__

# global logger
check_packages("loguru")
from loguru import logger  # noqa

from support.commands import cli  # noqa

if __name__ == "__main__":
    logger.remove()
    logger.add(sys.stderr, colorize=True, level="DEBUG")
    cli()  # Main entrypoint
