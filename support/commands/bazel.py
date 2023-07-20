import os
import sys
import click
from support import check_packages

# global logger
check_packages("dazel")
from dazel import DockerInstance  # noqa


@click.group(invoke_without_command=True)
@click.argument("args", nargs=-1)
def cli(args):
    """Runs bazel through a dazel container, passing all subsequent arguments to this one"""

    # Read the configuration either from .dazelrc or from the environment.
    di = DockerInstance.from_config()

    # If there is no .dazel_run file, or it is too old, start the DockerInstance.
    if (
        not os.path.exists(di.dazel_run_file)
        or not di.is_running()
        or (
            os.path.exists(di.dockerfile)
            and os.path.getctime(di.dockerfile) > os.path.getctime(di.dazel_run_file)
        )
    ):
        rc = di.start()
        if rc:
            return rc

    sys.exit(di.send_command(list(args)))
