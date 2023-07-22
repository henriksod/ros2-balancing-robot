#!/usr/bin/env python3

import os
import sys
from support import check_packages

# Set environment variable to point to this file
os.environ["ROS2_BALANCING_ROBOT_ENTRYPOINT"] = __file__

# global logger
check_packages("loguru")
from loguru import logger  # noqa

from support.commands import cli  # noqa

if __name__ == "__main__":
    logger.remove()
    logger.add(sys.stderr, colorize=True, level="INFO")
    cli()  # Main entrypoint
