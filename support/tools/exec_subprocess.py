import subprocess
from loguru import logger


def exec_subprocess(cmd, msg_on_error="", msg_on_success=""):
    try:
        output = subprocess.run(cmd, capture_output=True, shell=True, text=True)
        logger.info(f"\n{output.stderr}")
        logger.info(f"\n{output.stdout}")
        if output.returncode != 0:
            logger.error(msg_on_error)
        else:
            logger.success(msg_on_success)
        exit(output.returncode)
    # TODO: Use proper exception
    except Exception as e:  # noqa
        logger.error(e)
        exit()
