import os
import re
import sys
import click
from loguru import logger
from support import check_packages
from support.utils.exec_subprocess import exec_subprocess
from support.utils.staged_files import get_staged_files


def is_python_file(file):
    return re.search(r"\.py", file.lower())


def is_cxx_file(file):
    return re.search(r"\.(c|cc|cpp|h|hpp)", file.lower())


@click.group()
def cli():
    """Runs formatting checks and auto formatting"""
    pass


@cli.command()
@click.option(
    "--all-files",
    "all_files",
    is_flag=True,
    show_default=True,
    default=False,
    help="Run formatting checks on all files.",
)
def check(all_files):
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
    if not all_files:
        files = get_staged_files()
        python_files = [f for f in files if is_python_file(f)]
        cxx_files = [f for f in files if is_cxx_file(f)]
        logger.info("Staged python files:\n%s" % ("\n".join(python_files)))
        cmd_flake8 = (
            f"{sys.executable} -m flake8 {' '.join(python_files)}" if python_files else "true"
        )
        logger.info("Staged cxx files:\n%s" % ("\n".join(cxx_files)))
        cmd_cpplint = (
            (
                f"{sys.executable} -m cpplint --filter=-build/c++11 --linelength=100"
                f" {' '.join(cxx_files)}"
            )
            if cxx_files
            else "true"
        )
    else:
        cmd_flake8 = f"{sys.executable} -m flake8 {os.getcwd()}"
        cmd_cpplint = (
            f"find {os.getcwd()} -regextype posix-extended -regex '.*\\.(cc|cpp|h|hpp)'"
            f" | xargs {sys.executable} -m cpplint --filter=-build/c++11 --linelength=100"
        )

    cmd_buildifier = f"{run_entrypoint} bazel run //:buildifier_check"
    cmd_uncrustify = "true"  # f"{run_entrypoint} bazel run //:uncrustify_check"
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
        exit_on_failure=True,
    )


@cli.command()
@click.option(
    "--all-files",
    "all_files",
    is_flag=True,
    show_default=True,
    default=False,
    help="Run formatting fix on all files.",
)
def fix(all_files):
    """Fix formatting issues using Black and Buildifier"""
    check_packages("black")
    run_entrypoint = os.environ["ROS2_BALANCING_ROBOT_ENTRYPOINT"]
    if not all_files:
        files = get_staged_files()
        python_files = [f for f in files if is_python_file(f)]
        logger.info("Staged python files:\n%s" % ("\n".join(python_files)))
        cmd_black = (
            (f"{sys.executable} -m black --line-length=100" f" {' '.join(python_files)}")
            if python_files
            else "true"
        )
    else:
        cmd_black = f"{sys.executable} -m black --line-length=100 {os.getcwd()}"

    cmd_buildifier = f"{run_entrypoint} bazel run //:buildifier_fix"
    cmd_uncrustify = "true"  # f"{run_entrypoint} bazel run //:uncrustify_fix"
    exec_subprocess(
        "%s && %s && %s"
        % (
            cmd_black,
            cmd_buildifier,
            cmd_uncrustify,
        ),
        msg_on_error="Auto formatting failed!",
        msg_on_success="Auto formatting complete!",
        exit_on_failure=True,
    )
