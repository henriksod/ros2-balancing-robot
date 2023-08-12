# Copyright (c) 2023, Henrik Söderlund

import sys
import pkg_resources
import subprocess
import logging as LOGGING
from collections.abc import Iterable

try:
    from loguru import logger
except ImportError:

    class FallbackLogger:
        def __init__(self, **kwargs):
            self.logger = LOGGING.getLogger(**kwargs)

        def info(self, args, **kwargs):
            self.logger.info(args, **kwargs)

        def debug(self, args, **kwargs):
            self.logger.debug(args, **kwargs)

        def warning(self, args, **kwargs):
            self.logger.warning(args, **kwargs)

        def error(self, args, **kwargs):
            self.logger.error(args, **kwargs)

        def success(self, args, **kwargs):
            self.logger.info(args, **kwargs)

    logger = FallbackLogger()


def check_packages(requirements, cmds=""):
    """Test that each required package is available."""
    # Ref: https://stackoverflow.com/a/45474387/

    if not issubclass(type(requirements), Iterable) or isinstance(requirements, str):
        requirements = (requirements,)

    s = ""  # missing packages
    for r in requirements:
        r = str(r)
        try:
            pkg_resources.require(r)
        except pkg_resources.DistributionNotFound as e:
            logger.error(e)
            s += f"{r!r} "
    if s:
        logger.warning(f"\nMissing packages: {s}\nAtempting installation...")
        try:
            subprocess.check_output(
                f"{sys.executable} -m pip install --no-cache {s} {cmds}",
                shell=True,
                stderr=subprocess.STDOUT,
            )
        except Exception as e:  # noqa
            logger.error(e)
            exit()
        logger.success("All the missing packages were installed successfully")
