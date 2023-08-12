# Copyright (c) 2023, Henrik Söderlund

import os
import re
import sys
import click
from pathlib import Path
from loguru import logger
from support import check_packages
from support.utils.exec_subprocess import exec_subprocess
from support.utils.staged_files import get_staged_files


UNCRUSTIFY_BIN_PATH = (
    f"{os.getcwd()}/bazel-bin/external/com_github_uncrustify_uncrustify/uncrustify"
)
UNCRUSTIFY_CONFIG_PATH = f"{os.getcwd()}/bazel/repos/uncrustify/defaults.cfg"


def is_python_file(file):
    return re.search(r"\.py(\.(template|in))?$", file.lower())


def is_cxx_file(file):
    return re.search(r"\.(c|cc|cpp|h|hpp)(\.(template|in))?$", file.lower())


def all_files_cmd(cmd, extensions: list[str]):
    files = [
        str(x)
        for x in Path(os.getcwd()).glob("**/*")
        if not str(x).startswith(f"{os.getcwd()}/bazel-") and x.suffix.lstrip(".") in extensions
    ]
    return f"{cmd} {' '.join(files)}"


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
        logger.info("Staged files:\n%s" % ("\n".join(files)))
        python_files = [f for f in files if is_python_file(f)]
        cxx_files = [f for f in files if is_cxx_file(f)]
        if not python_files:
            cmd_flake8 = "true"
        else:
            cmd_flake8 = f"{sys.executable} -m flake8 {' '.join(python_files)}"
        if not cxx_files:
            cmd_cpplint = "true"
            cmd_uncrustify = "true"
        else:
            cmd_cpplint = (
                f"{sys.executable} -m cpplint"
                " --filter=-build/c++11 --filter=-build/include_what_you_use --linelength=100"
                f" {' '.join(cxx_files)}"
            )
            cmd_uncrustify = (
                f"{UNCRUSTIFY_BIN_PATH} -L 0 -c {UNCRUSTIFY_CONFIG_PATH} --check"
                f" {' '.join(cxx_files)}"
            )
    else:
        cmd_flake8 = all_files_cmd(f"{sys.executable} -m flake8", ["py", "py.template"])
        cmd_cpplint = all_files_cmd(
            f"{sys.executable} -m cpplint --filter=-build/c++11 --linelength=100",
            ["cc", "cpp", "h", "hpp"],
        )
        cmd_uncrustify = all_files_cmd(
            f"{UNCRUSTIFY_BIN_PATH} -L 0 -c {UNCRUSTIFY_CONFIG_PATH} --check",
            ["cc", "cpp", "h", "hpp"],
        )

    cmd_buildifier = f"{run_entrypoint} bazel run //:buildifier_check"
    cmd_codespell = "codespell --count"

    # Build uncrustify
    exec_subprocess(
        f"{run_entrypoint} bazel build @com_github_uncrustify_uncrustify//:uncrustify",
        exit_on_failure=True,
    )

    # Execute check commands
    [
        exec_subprocess(
            cmd,
            msg_on_error=(
                "\nRun the following command to mix most issues with formatting:\n\t"
                "./run.py format fix"
            ),
            exit_on_failure=True,
        )
        for cmd in [
            cmd_flake8,
            cmd_cpplint,
            cmd_buildifier,
            cmd_uncrustify,
            cmd_codespell,
        ]
    ]
    logger.success("Formatting complete!")


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
        logger.info("Staged files:\n%s" % ("\n".join(files)))
        python_files = [f for f in files if is_python_file(f)]
        cxx_files = [f for f in files if is_cxx_file(f)]
        if not python_files:
            cmd_black = "true"
        else:
            cmd_black = f"{sys.executable} -m black --line-length=100" f" {' '.join(python_files)}"
        if not cxx_files:
            cmd_uncrustify = "true"
        else:
            cmd_uncrustify = (
                f"{UNCRUSTIFY_BIN_PATH} -L 0 -c {UNCRUSTIFY_CONFIG_PATH} --no-backup"
                f" {' '.join(cxx_files)}"
            )
        if not files:
            cmd_addcopyright = "true"
        else:
            cmd_addcopyright = f"{os.getcwd()}/support/tools/addcopyright.sh" f" {' '.join(files)}"
    else:
        cmd_black = all_files_cmd(
            f"{sys.executable} -m black --line-length=100", ["py", "py.template"]
        )
        cmd_uncrustify = all_files_cmd(
            f"{UNCRUSTIFY_BIN_PATH} -L 0 -c {UNCRUSTIFY_CONFIG_PATH} --no-backup",
            ["cc", "cpp", "h", "hpp"],
        )
        cmd_addcopyright = all_files_cmd(
            f"{os.getcwd()}/support/tools/addcopyright.sh",
            ["cc", "cpp", "h", "hpp", "py", "bazel", "bzl", "yaml", "yml"],
        )

    cmd_buildifier = f"{run_entrypoint} bazel run //:buildifier_fix"
    [
        exec_subprocess(
            cmd,
            msg_on_error="Auto formatting failed!",
            exit_on_failure=True,
        )
        for cmd in [
            cmd_addcopyright,
            cmd_black,
            cmd_buildifier,
            cmd_uncrustify,
        ]
    ]
    logger.success("Auto formatting complete!")
