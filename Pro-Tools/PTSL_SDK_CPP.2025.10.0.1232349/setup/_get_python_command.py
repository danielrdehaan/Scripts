# Copyright 2023 by Avid Technology, Inc.
import shutil
import subprocess
import sys


def _quote_path(path_str: str) -> str:
    """
    Quote the provided path string based on the operating system.
    """
    res = path_str.strip()

    if ' ' in path_str:
        if sys.platform == 'darwin':
            if '"' not in path_str and "'" not in path_str:
                res = f"'{path_str}'"
        else:
            if '"' not in path_str:
                res = f'"{path_str}"'

    return res


def _get_python_path(python_command: str, check_if_quotes_needed: bool) -> str:
    """
    Get the full path of the Python interpreter specified by python_command.
    """
    try:
        python_path_output = subprocess.run([python_command, '-c',
                                             'import sys;'
                                             'print(sys.executable)'],
                                            stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        python_path = python_path_output.stdout
        return _quote_path(python_path) if check_if_quotes_needed else python_path
    except Exception as e:
        print(f'An error occurred while checking path of {python_command}: {e}')


def get_python_command(check_if_quotes_needed: bool = False) -> str:
    """
    Find the best Python command to use. This function checks the currently running Python interpreter first,
    then checks common Python command aliases ('python', 'python3').

    :return: Full path or just a name (if the path is the same) of the Python interpreter.
    """
    sys_executable = sys.executable
    currently_running_python_path = _quote_path(sys_executable) if check_if_quotes_needed else sys_executable

    for command in ['python', 'python3']:
        if shutil.which(command):
            found_path = _get_python_path(command, check_if_quotes_needed)
            if found_path == currently_running_python_path:
                return command

    return currently_running_python_path


if __name__ == '__main__':
    print(f'Python command to use: {get_python_command()}')
