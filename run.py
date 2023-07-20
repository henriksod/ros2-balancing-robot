#!/usr/bin/env python3

import sys
from support import check_packages

# global logger
check_packages("loguru")
from loguru import logger

from support.commands import cli

if __name__ == "__main__":
    logger.remove()
    logger.add(sys.stderr, colorize=True, level="INFO")
    cli()  # Main entrypoint
