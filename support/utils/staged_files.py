import subprocess
from loguru import logger


def get_staged_files() -> list[str]:
    try:
        output = subprocess.run(
            "git status -s | grep -E '^(AM|MM|A|M)' | cut -f2- -d' ' | tr -d ' '",
            capture_output=True,
            shell=True,
            text=True,
        )
        return output.stdout.split("\n")
    # TODO: Use proper exception
    except Exception as e:  # noqa
        logger.error(e)
        exit()
