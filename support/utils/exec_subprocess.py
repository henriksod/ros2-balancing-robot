# Copyright (c) 2023, Henrik Söderlund

import sys

from subprocess import Popen, PIPE, CalledProcessError
from loguru import logger


def _run_command(cmd, cwd="."):
    if isinstance(cmd, str):
        cmd = cmd.split()
    logger.debug(" ".join(cmd))
    popen = Popen(cmd, cwd=cwd, stdout=PIPE, stderr=PIPE, universal_newlines=True)
    for stdout_line in iter(popen.stdout.readline, ""):
        yield stdout_line
    popen.stdout.close()
    _, err = popen.communicate()
    return_code = popen.wait()
    if return_code:
        logger.error(err)
        raise CalledProcessError(return_code, cmd, stderr=err)


def exec_subprocess(cmd, msg_on_error="", msg_on_success="", exit_on_failure=False):
    out = ""
    try:
        for line in _run_command(cmd):
            out += line
            sys.stdout.write(line)
        if msg_on_success:
            logger.success(msg_on_success)
    except CalledProcessError as e:
        logger.error(e)
        if msg_on_error:
            logger.error(msg_on_error)
        if exit_on_failure:
            raise e
    return out
