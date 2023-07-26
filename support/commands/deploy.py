import os
import sys
import click
import json
import tarfile
import shutil
from loguru import logger
from itertools import product
from support import check_packages
from support.utils.exec_subprocess import exec_subprocess


SUPPORTED_ARCHITECTURES = ["x86_64", "aarch64"]
SUPPORTED_COMPILERS = ["gcc", "clang"]
SUPPORTED_CONFIGS = {
    (a, c): f"{a}_linux_{c}" for a, c in list(product(SUPPORTED_ARCHITECTURES, SUPPORTED_COMPILERS))
}

PYTHON_PLATFORM_MAPPING = {"aarch64": "aarch64-unknown-linux-gnu"}


def make_executable(path):
    mode = os.stat(path).st_mode
    mode |= (mode & 0o444) >> 2  # copy R bits to X
    os.chmod(path, mode)


@click.command()
@click.option(
    "--rule",
    type=str,
    help="The name of the bazel rule to deploy (found in BUILD.bazel under deployment/)",
)
@click.option(
    "--arch",
    default="x86_64",
    type=str,
    help="Target architecture",
)
@click.option(
    "--compiler",
    default="gcc",
    type=str,
    help="Compiler to use for deployment",
)
def cli(rule, arch, compiler):
    """Deploys ROS2 node composition to target host"""

    run_entrypoint = os.environ["ROS2_BALANCING_ROBOT_ENTRYPOINT"]
    project_config = json.loads(os.environ["ROS2_BALANCING_ROBOT_CONFIG"])

    if arch not in SUPPORTED_ARCHITECTURES:
        msg = f"Architecture {arch} is not supported!" f" (not one of {SUPPORTED_ARCHITECTURES})"
        logger.error(msg)
        raise AssertionError(msg)

    if compiler not in SUPPORTED_COMPILERS:
        msg = f"Compiler {compiler} is not supported!" f" (not one of {SUPPORTED_COMPILERS})"
        logger.error(msg)
        raise AssertionError(msg)

    # Look for required libraries
    check_packages("scp")

    # Get git commit hash
    logger.info("Getting commit hash...")
    commit_hash = exec_subprocess("git rev-parse --short HEAD").rstrip("\n")

    # Build package
    config = SUPPORTED_CONFIGS[(arch, compiler)]
    exec_subprocess(
        f"{run_entrypoint} bazel build --config={config} //deployment:{rule}",
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
    with tarfile.open(f"{package_dir}/serial_can_receiver_example.tar", "r") as tarball:
        tarball.extractall(extraction_dir)

    # Read the launch script template
    with open(f"{os.getcwd()}/support/tools/launch_package.py.template", "r") as file:
        filedata = file.read()

    # Replace the target string
    filedata = filedata.replace("@@@LAUNCHER_NAME@@@", f"{rule}")
    filedata = filedata.replace("@@@PYTHON_PLATFORM@@@", f"{PYTHON_PLATFORM_MAPPING[arch]}")

    # Create the launch file
    with open(f"{extraction_dir}/launch_package", "w") as file:
        file.write(filedata)
    make_executable(f"{extraction_dir}/launch_package")

    # Zip the directory
    tarball_deliverable = f"{extraction_dir}/serial_can_receiver_example.tar.gz"
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

    _, _, ssh_stderr = ssh.exec_command(
        f"tar -xzvf ~/balancing_robot-{commit_hash}.tar.gz -C ~"
        f" && sudo rm -rf /opt/balancing_robot-{commit_hash}"
        f" && sudo mv ~/balancing_robot-{commit_hash} /opt/"
        f" && sudo ln -sf /opt/balancing_robot-{commit_hash}/ /opt/balancing_robot"
    )
    ssh_stderr_msg = ssh_stderr.read()
    if ssh_stderr_msg:
        logger.error(ssh_stderr_msg)
        exit()

    logger.success("Successfully deployed to target!")
