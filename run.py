#!/usr/bin/env python3

import sys
from support import check_packages

# global logger
check_packages("loguru")
from loguru import logger  # noqa

from support.commands import cli  # noqa

if __name__ == "__main__":
    logger.remove()
    logger.add(sys.stderr, colorize=True, level="INFO")
    cli()  # Main entrypoint
