import subprocess
from loguru import logger


def exec_subprocess(cmd, msg_on_error="", msg_on_success="", exit_on_failure=False):
    try:
        output = subprocess.run(cmd, capture_output=True, shell=True, text=True)
        if output.stderr:
            logger.warning(f"\n{output.stderr}")
        if output.stdout:
            logger.info(f"\n{output.stdout}")
        if output.returncode != 0:
            if msg_on_error:
                logger.error(msg_on_error)
            if exit_on_failure:
                exit(output.returncode)
        elif msg_on_success:
            logger.success(msg_on_success)
        return output.stdout
    # TODO: Use proper exception
    except Exception as e:  # noqa
        logger.error(e)
        exit()
