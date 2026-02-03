# Copyright 2023-2024 by Avid Technology, Inc.
import subprocess
import os
import locale

import logging

class WarningFormatter(logging.Formatter):
    def format(self, record):
        if record.levelno in (logging.WARNING, logging.ERROR):
            return f"{record.msg}"
        return record.msg

logging.basicConfig(level=logging.INFO)

handler = logging.StreamHandler()
handler.setFormatter(WarningFormatter())

logger = logging.getLogger()

for existing_handler in logger.handlers:
    existing_handler.setFormatter(handler.formatter)

def _wrap_subprocess(cmd, cwd=os.getcwd()):
    if isinstance(cmd, list):
        cmd = " ".join(cmd)
        
    enc = locale.getpreferredencoding(False) or "utf-8"
    
    p = subprocess.Popen(
        cmd,
        shell=True,
        cwd=cwd,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        encoding=enc,
        errors="replace"
    )
    try:
        for line in p.stdout:   
            logger.info(line.rstrip())
        p.communicate()
        exit_code = p.returncode
        if exit_code != 0:
            raise RuntimeError(f"Subprocess returned non-zero exit code: {exit_code}")
    except Exception as e:
        p.terminate()
        logger.error(f"Process has been terminated due to an exception: {e}")
        raise e
    return exit_code
