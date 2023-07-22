import click
import subprocess
from loguru import logger
from support import check_packages

check_packages("pyyaml")
import yaml  # noqa


@click.group()
def cli():
    """Runs setup commands"""
    pass


@cli.command()
def remote():
    """Configures  ~/.ssh/config based on remote settings in config/config.yaml"""

    with open("config/config.yaml", "r") as f:
        config = yaml.safe_load(f)
    ssh_config = (
        '"\n\n# Robot platform host for remote execution\n'
        f"Host {config['remote']['host_alias']}\n"
        f"  HostName {config['remote']['host_name']}\n"
        f"  User {config['remote']['user']}\n"
        f"  Port {config['remote']['port']}\n"
        f"  IdentityFile {config['remote']['identity_file']}\n\n\""
    )
    try:
        output = subprocess.run(
            f"echo {ssh_config} >> ~/.ssh/config", capture_output=True, shell=True, text=True
        )
        logger.info(f"\n{output.stdout}")
        if output.returncode != 0:
            logger.error("Failed to update ~/.ssh/config")
        else:
            logger.success(
                "Updated ~/.ssh/config and added remote host"
                f" {config['remote']['host_alias']!r}!"
            )
        exit(output.returncode)
    except Exception as e:  # noqa
        logger.error(e)
        exit()
