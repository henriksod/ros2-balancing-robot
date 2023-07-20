import os
import sys
import click
import subprocess
from loguru import logger
from support import check_packages


@click.group()
def cli():
    pass


@cli.command()
def check():
    check_packages("flake8")
    try:
        output = subprocess.run(
            f"{sys.executable} -m flake8 {os.getcwd()}", capture_output=True, shell=True, text=True
        )
        logger.info(f"\n{output.stdout}")
        if output.returncode != 0:
            logger.error(
                "Run the following command to mix most issues with formatting:\n\t"
                "./run.py format fix"
            )
        else:
            logger.success("Formatting checks passed!")
        exit(output.returncode)
    except Exception as e:  # noqa
        logger.error(e)
        exit()


@cli.command()
def fix():
    check_packages("black")
    try:
        output = subprocess.run(
            f"{sys.executable} -m black --line-length=100 {os.getcwd()}",
            capture_output=True,
            shell=True,
            text=True,
        )
        logger.info(f"\n{output.stdout}")
        if output.returncode != 0:
            logger.error("Auto formatting failed!")
        else:
            logger.success("Auto formatting complete!")
        exit(output.returncode)
    except Exception as e:  # noqa
        logger.error(e)
        exit()
