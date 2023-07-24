import os
import sys
import click
from support import check_packages
from support.utils.exec_subprocess import exec_subprocess


@click.group()
def cli():
    """Runs formatting checks and auto formatting"""
    pass


@cli.command()
def check():
    """Check for formatting issues using Flake8 and Buildifier"""
    check_packages(
        (
            "flake8",
            "flake8-blind-except",
            "flake8-docstrings",
            "flake8-bugbear",
            "flake8-comprehensions",
            "flake8-docstrings",
            "flake8-implicit-str-concat",
            "pydocstyle>=5.0.0",
            "codespell",
            "cpplint",
        )
    )
    run_entrypoint = os.environ["ROS2_BALANCING_ROBOT_ENTRYPOINT"]
    cmd_flake8 = f"{sys.executable} -m flake8 {os.getcwd()}"
    cmd_cpplint = (
        f"find {os.getcwd()} -regextype posix-extended -regex '.*\\.(cc|cpp|h|hpp)'"
        f" | xargs {sys.executable} -m cpplint --filter=-build/c++11  --linelength=100"
    )
    cmd_buildifier = f"{run_entrypoint} bazel run //:buildifier_check"
    cmd_uncrustify = "true"  # f"{run_entrypoint}  bazel run //:uncrustify_check"
    cmd_codespell = "codespell --count"
    exec_subprocess(
        "%s && %s && %s && %s && %s"
        % (
            cmd_flake8,
            cmd_cpplint,
            cmd_buildifier,
            cmd_uncrustify,
            cmd_codespell,
        ),
        msg_on_error=(
            "\nRun the following command to mix most issues with formatting:\n\t"
            "./run.py format fix"
        ),
        msg_on_success="Formatting checks passed!",
    )


@cli.command()
def fix():
    """Fix formatting issues using Black and Buildifier"""
    check_packages("black")
    run_entrypoint = os.environ["ROS2_BALANCING_ROBOT_ENTRYPOINT"]
    cmd_black = f"{sys.executable} -m black --line-length=100 {os.getcwd()}"
    cmd_buildifier = f"{run_entrypoint} bazel run //:buildifier_fix"
    cmd_uncrustify = "true"  # f"{run_entrypoint}  bazel run //:uncrustify_fix"
    exec_subprocess(
        "%s && %s && %s"
        % (
            cmd_black,
            cmd_buildifier,
            cmd_uncrustify,
        ),
        msg_on_error="Auto formatting failed!",
        msg_on_success="Auto formatting complete!",
    )
