# Copyright (c) 2023, Henrik Söderlund

import os

import sys
import click
import json
import tarfile
import shutil
from loguru import logger
from support import check_packages
from support.utils.exec_subprocess import exec_subprocess


def make_executable(path):
    mode = os.stat(path).st_mode
    mode |= (mode & 0o444) >> 2  # copy R bits to X
    os.chmod(path, mode)


@click.command()
@click.option(
    "--install",
    is_flag=True,
    show_default=True,
    default=False,
    help="Install the deployed software on target system.",
)
@click.option(
    "--debug",
    is_flag=True,
    show_default=True,
    default=False,
    help="Compile in debug mode.",
)
@click.argument(
    "rule_name",
    type=str,
    required=1,
)
def cli(rule_name, install, debug):
    """Deploys packed ROS2 node composition to target host

    RULE_NAME: The name of the bazel rule to deploy (found in BUILD.bazel under deployment/)
    """

    run_entrypoint = os.environ["ROS2_BALANCING_ROBOT_ENTRYPOINT"]
    project_config = json.loads(os.environ["ROS2_BALANCING_ROBOT_CONFIG"])

    # Look for required libraries
    check_packages("scp")

    # Get git commit hash
    logger.info("Getting commit hash...")
    commit_hash = exec_subprocess("git rev-parse --short HEAD").rstrip("\n")

    # Run zephyr setup and compile RTOS project
    logger.info("Compiling Zephyr bin file...")
    exec_subprocess(
        f"{run_entrypoint} bazel run @com_github_zephyrproject_rtos_zephyr//:setup_build",
        msg_on_error="Compiling RTOS project failed!",
        msg_on_success="Compiling RTOS project succeeded!",
        exit_on_failure=True,
    )

    # Build package
    logger.info(f"Building package target {rule_name}...")
    exec_subprocess(
        (
            f"{run_entrypoint} bazel build //deployment:{rule_name} --config=aarch64_linux_gcc"
            + (" --compilation_mode=dbg" if debug else "")
        ),
        msg_on_error="Building deployment target failed!",
        msg_on_success="Building deployment target succeeded!",
        exit_on_failure=True,
    )

    logger.info("Preparing tarball for delivery...")

    # Extract the built tarball from bazel
    package_dir = os.path.realpath(f"{os.getcwd()}/bazel-bin/deployment")
    extraction_dir = f"{package_dir}/balancing_robot"
    if os.path.isdir(extraction_dir):
        shutil.rmtree(extraction_dir)
    os.mkdir(extraction_dir)
    with tarfile.open(f"{package_dir}/{rule_name}.tar", "r") as tarball:
        tarball.extractall(extraction_dir)

    # Read the launch script template
    with open(f"{os.getcwd()}/support/tools/launch_package.py.template", "r") as file:
        filedata = file.read()

    # Replace the target string
    filedata = filedata.replace("@@@LAUNCHER_NAME@@@", f"{rule_name}")
    filedata = filedata.replace("@@@PYTHON_PLATFORM@@@", "aarch64-unknown-linux-gnu")

    # Create the launch file
    with open(f"{extraction_dir}/launch_package", "w") as file:
        file.write(filedata)
    make_executable(f"{extraction_dir}/launch_package")

    # Zip the directory
    tarball_deliverable = f"{extraction_dir}/{rule_name}_example.tar.gz"
    with tarfile.open(tarball_deliverable, "w:gz") as tarball:
        tarball.add(extraction_dir, arcname=f"balancing_robot-{commit_hash}")

    from paramiko import SSHClient
    from scp import SCPClient

    logger.info("Connecting to host...")

    ssh = SSHClient()
    ssh.load_system_host_keys()
    ssh.connect(
        project_config["remote"]["host_name"],
        port=int(project_config["remote"]["port"]),
        username=project_config["remote"]["user"],
        key_filename=os.path.expanduser(project_config["remote"]["identity_file"]),
    )

    # SCPCLient takes a paramiko transport as an argument
    scp = SCPClient(ssh.get_transport())

    logger.info("Uploading tarball to host...")

    # Define progress callback that prints the current percentage completed for the file
    def progress(filename, size, sent):
        sys.stdout.write(
            "%s's progress: %.2f%%   \r" % (str(filename), float(sent) / float(size) * 100)
        )

    # SCPCLient takes a paramiko transport and progress callback as its arguments.
    scp = SCPClient(ssh.get_transport(), progress=progress)

    scp.put(tarball_deliverable, f"~/balancing_robot-{commit_hash}.tar.gz")
    # Should now be printing the current progress of your put function.

    scp.close()

    logger.info("Unpacking delivery at host...")

    pkg_name = f"balancing_robot-{commit_hash}"
    _, _, ssh_stderr = ssh.exec_command(
        f"tar -xzvf ~/{pkg_name}.tar.gz -C ~"
        f" && sudo rm ~/{pkg_name}.tar.gz"
        f" && tar -xvf ~/{pkg_name}/deployment/bossac.tar -C ~/{pkg_name}/deployment"
        f" && sudo rm ~/{pkg_name}/deployment/bossac.tar"
        f" && tar -xvf ~/{pkg_name}/deployment/zephyr_sdk_aarch64.tar -C ~/{pkg_name}/deployment"
        f" && sudo rm ~/{pkg_name}/deployment/zephyr_sdk_aarch64.tar"
        f" && tar -xvf ~/{pkg_name}/deployment/zephyr/zephyr_project.tar.gz -C ~/{pkg_name}/deployment/zephyr"
        f" && sudo rm ~/{pkg_name}/deployment/zephyr/zephyr_project.tar.gz"
        f" && sudo rm -rf /opt/{pkg_name}"
        f" && sudo mv ~/{pkg_name} /opt/"
        f" && sudo ln -sf /opt/{pkg_name} /opt/balancing_robot"
    )
    ssh_stderr_msg = ssh_stderr.read()
    if ssh_stderr_msg:
        logger.error(ssh_stderr_msg)
        exit()

    logger.success("Successfully deployed to target!")

    if install:
        logger.info("Installing software on target...")
        zephyr_bin_path = (
            "./deployment/zephyr/external/com_github_zephyrproject_rtos_zephyr/build/zephyr"
        )
        _, _, ssh_stderr = ssh.exec_command(
            " ".join(
                [
                    f"cd /opt/balancing_robot-{commit_hash}",
                    f"&& stty -F {project_config['arduino']['serial']} raw ispeed 1200 ospeed 1200",
                    "cs8 -cstopb ignpar eol 255 eof 255",
                    "&& ./deployment/external/com_github_shumatech_bossa/bossac",
                    f"--port={project_config['arduino']['serial']} -e -w -b",
                    f"{zephyr_bin_path}/zephyr.bin -R",
                ]
            )
        )
        ssh_stderr_msg = ssh_stderr.read()
        if ssh_stderr_msg:
            logger.error(ssh_stderr_msg)
            exit()

        logger.success("Successfully installed software on target!")
