# Copyright 2021-2025 by Avid Technology, Inc.
# CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of
# the information contained in this document is subject to an Avid SDK license.

import argparse
import os
import platform
import shutil
import subprocess
import sys
import venv
from dataclasses import dataclass
from pathlib import Path

from _get_python_command import get_python_command
from _subprocess_wrapper import _wrap_subprocess


def create_venv(path: Path):
    print(f"\nCreating venv (python and pip will be copied there): {path.resolve()}")
    try:
        venv.EnvBuilder(clear=True, with_pip=True).create(path)
    except (subprocess.CalledProcessError, ImportError, FileExistsError, RuntimeError) as e:
        print(f"\nAn error occurred: {str(e)}. Trying to clean up and create venv without using the EnvBuilder.")
        shutil.rmtree(path, ignore_errors=True)
        try:
            _wrap_subprocess([get_python_command(), "-m", "venv", str(path)])
        except Exception as e:
            print(f"\nAn error occurred while creating venv: {e}")
            sys.exit(1)


@dataclass
class ScriptParams:
    config_dir: Path
    source_dir: Path
    config: str
    is_dev_mode_on: bool
    arch: str
    os_version: str
    bag_config_dir: Path
    vs_compiler_version: str
    vs_toolset_version: str
    library_type: str
    conan_settings_yml: str


def call_script(sp: ScriptParams):
    venv_path = Path(".venv")
    venv_python_command = venv_path / "Scripts" / "python" if platform.system() == "Windows" else venv_path / "bin" / "python"

    def setup_conan_env(sp: ScriptParams):
        out_dir = sp.bag_config_dir.resolve()
        use_short_paths = (sys.platform == "win32")
        os.environ["CONAN_USER_HOME"] = str(out_dir)
        print(f"Isolating conan cache:", flush=True)
        print(f"  CONAN_USER_HOME={os.environ['CONAN_USER_HOME']}", flush=True)
        if use_short_paths:
            os.environ["CONAN_USER_HOME_SHORT"] = str(out_dir / ".conan_s")
            os.environ["CONAN_USE_ALWAYS_SHORT_PATHS"] = "True"
            print(f"  CONAN_USE_ALWAYS_SHORT_PATHS={os.environ['CONAN_USE_ALWAYS_SHORT_PATHS']}", flush=True)
            print(f"  CONAN_USER_HOME_SHORT={os.environ['CONAN_USER_HOME_SHORT']}", flush=True)
        

    def build_command():
        command = [
            str(venv_python_command),
            f'"{sp.config_dir / "ptsl_build_script.py"}"',
            "--source_dir",
            f'"{sp.source_dir}"',
            "--config", sp.config,
            "--dev" if sp.is_dev_mode_on else ""
        ]
        command += ["--arch", sp.arch] if sp.arch else []
        command += ["--os", sp.os_version] if sp.os_version else []
        command += ["--vs_compiler", sp.vs_compiler_version] if platform.system() == "Windows" and sp.vs_compiler_version else []
        command += ["--vs_toolset", sp.vs_toolset_version] if platform.system() == "Windows" and sp.vs_toolset_version else []
        command += ["--library_type", sp.library_type] if sp.library_type else []
        command += ["--conan_settings_yml", str(sp.conan_settings_yml)]
        return command

    def get_platform_cmd_prerequisites():
        activate_cmd = f"{venv_path / 'Scripts' / 'activate.bat'}" if platform.system() == "Windows" \
            else f"source {venv_path / 'bin' / 'activate'}"  # "Darwin"
        return f"{activate_cmd} && {venv_python_command} -m pip install -r requirements.txt && "

    command = ' '.join(build_command())
    print(f"\nInitializing ptsl_build_script from the command: {command}, cwd: {sp.bag_config_dir},")
    print("\nGetting dependencies:")

    # setup the conan user home environment variables to isolate the conan cache
    # before spawning the subprocess
    setup_conan_env(sp)

    cmd_prerequisites = get_platform_cmd_prerequisites()
    cmd = f"{cmd_prerequisites}{command}"
    try:
        _wrap_subprocess(cmd, cwd=sp.bag_config_dir)
    except subprocess.CalledProcessError as cpe:
        print(f"\nAn error occurred while running the command: {cpe}")
        sys.exit(1)
    except:
        print(f"\nAn error occurred while running the following command: {cmd}")
        sys.exit(1)
    


def prepare_parser():
    parser = argparse.ArgumentParser(prog='PTSLSDK build script',
                                     epilog='Usage Example: -t ptsl.client.cpp -c Debug -a arm64 -os 11.0',
                                     add_help=True)
    parser.add_argument('-t', '--target',
                        required=True,
                        type=str,
                        metavar='',
                        help='Target that should be built: ["ptsl.client.cpp", "ptslcmd.sdk", "ptslcmd.sdk.2024.06.0"]',
                        choices=['ptsl.client.cpp', 'ptslcmd.sdk', 'ptslcmd.sdk.2024.06.0'])
    parser.add_argument('-c', '--config',
                        default='Debug',
                        type=str,
                        metavar='',
                        help='Build config: ["Debug", "Release"]',
                        choices=['Debug', 'Release'])
    parser.add_argument('-a', '--arch',
                        type=str,
                        default=None,
                        metavar='',
                        help='Host arch: ["x86_64", "arm64", "x86_64_arm64"]',
                        choices=['x86_64', 'arm64', 'x86_64_arm64', 'armv8'])
    parser.add_argument('-os', '--os_version',
                        default=None,
                        metavar='',
                        type=str,
                        help='Host os version')
    parser.add_argument('-dev', action='store_true')
    parser.add_argument('-vs_compiler', '--vs_compiler_version',
                        default='vs2022',
                        metavar='',
                        type=str,
                        help='Visual Studio compiler version: ["Visual Studio 15 2017", "Visual Studio 16 2019", "Visual Studio 17 2022"]',
                        choices=["vs2017", "vs2019", "vs2022"])
    parser.add_argument('-vs_toolset_version', '--vs_toolset_version',
                         default=None,
                         metavar='',
                         type=str,
                         help='Visual Studio toolset version: v141, v142, etc',
                        )
    parser.add_argument('-lt', '--library_type',
                        default='shared',
                        type=str,
                        metavar='',
                        help='Library Type: ["static", "shared"]',
                        choices=['static', 'shared'])
    parser.add_argument('-cs', '--conan_settings_yml',
                        default=None,
                        type=str,
                        metavar='',
                        help='Path to custom Conan settings.yml file')
    
    return parser


def clean_cmake_cache():
    '''This is to get rid of this error I faced on Win:
        # CMake Error: Error: generator toolset:
        # Does not match the toolset used previously: host=x64
        # Either remove the CMakeCache.txt file and CMakeFiles directory or choose a different binary directory.
    '''
    if is_dev_mode_on and sys.platform == 'win32':
        print("\nCleaning up CMake cache...")

        win_build_dir = bag_config_dir / ".." / "WinBuild"
        cmake_cache_file = win_build_dir / "CMakeCache.txt"
        cmake_files_dir = win_build_dir / "CMakeFiles"

        if os.path.exists(cmake_cache_file):
            os.remove(cmake_cache_file)
            print(f"\tFile removed: {cmake_cache_file.resolve()}")
        else:
            print(f"\tNo file to clean, good: {cmake_cache_file.resolve()}")

        if os.path.exists(cmake_files_dir):
            shutil.rmtree(cmake_files_dir, ignore_errors=True)
            print(f"\tDirectory removed: {cmake_files_dir.resolve()}")
        else:
            print(f"\tNo directory to clean, good: {cmake_files_dir.resolve()}")


if __name__ == "__main__":
    print(f"Platform: {platform.system()}")
    print(f"Python Version: {sys.version}")
    print(f"Python Path: {sys.executable}")
    print(f"\twhich is used as: {get_python_command(check_if_quotes_needed=True)}")

    if sys.version_info.major != 3:
        print(f"Python version {sys.version} is not supported. Python 3.x is required.")
        sys.exit(1)

    pip_ver = subprocess.run([get_python_command(), "-m", "pip", "--version"],
                             stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    print(f"pip Version: {pip_ver.stdout.decode('utf-8')}")

    print(f"Argument list: {sys.argv}")
    print("")

    args = prepare_parser().parse_args()

    target = args.target
    config = args.config
    arch = args.arch.replace("armv8", "arm64") if args.arch else None
    os_version = args.os_version
    is_dev_mode_on = args.dev
    vs_compiler_version = args.vs_compiler_version
    vs_toolset_version = args.vs_toolset_version
    library_type = args.library_type

    bag_dir = Path.cwd()
    print(f"Bag directory: {bag_dir.resolve()}")

    repo_source_dir = bag_dir / ".." / "ProTools" / "PTSL" / "Clients" / "CPP"
    if not repo_source_dir.exists():
        repo_source_dir = bag_dir / ".." / ".." / "ProTools" / "PTSL" / "Clients" / "CPP"

    inside_repo = repo_source_dir.exists()

    if is_dev_mode_on and not inside_repo:
        print("\nDev mode is not allowed. (Check you are running the script from the bag directory)")
        sys.exit(1)

    config_dir = Path(__file__).parent.parent / "Config"

    bag_config_dir = bag_dir / "Projects" / "ProTools" / "PTSL" / "Clients" / "CPP" / "PTSL_SDK_CPP" / "Config" \
        if is_dev_mode_on else config_dir

    if target == "ptsl.client.cpp":
        source_dir = config_dir.parent
    elif target == "ptslcmd.sdk":
        source_dir = config_dir.parent.parent.parent / "Examples" / "CPP" / "ptslcmd" if is_dev_mode_on \
            else config_dir.parent / "examples" / "ptslcmd"
    elif target == "ptslcmd.sdk.2024.06.0":
        source_dir = config_dir.parent.parent.parent / "Examples" / "CPP" / "ptslcmd.2024.06.0" if is_dev_mode_on \
            else config_dir.parent / "examples" / "ptslcmd.2024.06.0"
    else:
        print(f"\nUnknown target '{target}'")
        sys.exit(1)

    clean_cmake_cache()

    venv_path = bag_config_dir / ".venv"
    conan_settings_yml = Path(args.conan_settings_yml) if args.conan_settings_yml else config_dir / "settings.yml"

    try:
        create_venv(venv_path)
        call_script(ScriptParams(config_dir, source_dir, config, is_dev_mode_on, arch, os_version, bag_config_dir, vs_compiler_version, vs_toolset_version, library_type, conan_settings_yml))
        print(f"\nThe script has finished successfully.\n.venv will be removed now.")
    finally:
        shutil.rmtree(venv_path, ignore_errors=True)
