# Copyright 2022-2025 by Avid Technology, Inc.
# CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

import sys
import platform
from conans.client.conan_api import Conan
import subprocess
import argparse
import shutil
from pathlib import Path
import jinja2
import json

this_directory = Path(__file__).parent

try:
    from cmakepresets import CMakePresets 
except ImportError:
    sys.path.append(str(this_directory.parent.parent.parent / "Common" / "Python"))
    from cmakepresets import CMakePresets 

# Conan hook
# TODO: conan/api/subapi/remotes.py
class ConanApi(Conan):
    def __init__(self):
        super().__init__()

    def get_remote_list(self) -> list:
        remote_list = self.remote_list()
        return [i[0] for i in remote_list]

def patch_cmake(dependencies_dir, dependencies_arch_settings):
    """
    Apply several patches to Conan generated CMake files:
    - All the conan binaries are linked with visibility hidden
    - Universal binary: Generate almost fully working CMake files that imitate files from Conan
    """

    def find_lines(file_path, search_string):
        with open(file_path, 'r') as file:
            lines = file.readlines()
            matching_lines = [line.strip() for line in lines if search_string in line]
            return matching_lines

    # If the universal binary (more than 1 architecture)
    if len(dependencies_arch_settings) > 1:
        # 1. Copy some architecture independent files that are needed to setup dependencies.
        # 2. Create a targets file that references targets files specific for each architecture.
        shutil.copy2(dependencies_arch_settings[0]["folder"] / "cmakedeps_macros.cmake", dependencies_dir)
        for file in Path(dependencies_arch_settings[0]["folder"]).iterdir():
            if not file.is_file():
                continue
            if "config" in file.name.lower():
                shutil.copy2(file, dependencies_dir)
            if "Targets.cmake" in file.name:
                with open(dependencies_dir / file.name, 'w') as f:
                    f.write((
                        "\n".join(f'include({s["folder"]}/{file.name})' for s in dependencies_arch_settings)
                    ))
 
        # Experimental. Universal binary toolchain is not supported by Conan (and even CMake struggles with it).
        # Adding only lines that shouldn't be affected by target host architecture.
        toolchain_info = []
        # To find CMake specific paths.
        toolchain_info.extend(find_lines(dependencies_arch_settings[0]["folder"] / "conan_toolchain.cmake", "CMAKE_PREFIX_PATH"))
        # To find tools for crosscompilation.
        toolchain_info.extend(find_lines(dependencies_arch_settings[0]["folder"] / "conan_toolchain.cmake", "CMAKE_PROGRAM_PATH"))
        # To find include directories (I hope they are same between architectures).
        toolchain_info.extend(find_lines(dependencies_arch_settings[0]["folder"] / "conan_toolchain.cmake", "CMAKE_INCLUDE_PATH"))

        environment = jinja2.Environment(loader=jinja2.FileSystemLoader(this_directory))
        template = environment.get_template("universal_toolchain.cmake.jinja")
        content = template.render({
            "architectures": ";".join([s["cmake_arch"] for s in dependencies_arch_settings]),
            "extra_lines": "\n".join(toolchain_info)
        })

        with open(dependencies_dir / "universal_toolchain.cmake", mode="w", encoding="utf-8") as message:
            message.write(content)

        # Apply the patch, so CMake can create libraries with the same name but different architectures.
        subprocess.run(["patch", "--ignore-whitespace", "-f", "-i", this_directory / "cmakedeps_universal.patch", "-d", dependencies_dir], check=True)

    # Apply the patch, so the output binary doesn't export every symbol from every linked static library.
    subprocess.run(["patch", "--ignore-whitespace", "-f", "-i", this_directory / "cmakedeps_hidden.patch", "-d", dependencies_dir], check=True)

#Parser setup
parser = argparse.ArgumentParser(description='PTSLC and PTSLCMD.sdk targets building parser')
parser.add_argument('--source_dir')
parser.add_argument('--dev', action='store_true')
parser.add_argument('--config')
parser.add_argument('--arch')
parser.add_argument('--os')
if platform.system() == "Windows":
    parser.add_argument('--vs_compiler', required=True)
    parser.add_argument('--vs_toolset')
parser.add_argument('--library_type')
parser.add_argument('--conan_settings_yml')
args = parser.parse_args()
print(args)

if not args.config:
    args.config = "Debug"
    print(f"Build config is not set, assuming {args.config}")

if not args.arch:
    if platform.system() == "Darwin":
        args.arch = "x86_64_arm64"
    elif platform.system() == "Windows":
        args.arch = "x86_64"
    
    print(f"Build architecture is not set, assuming {args.arch}")

# Extract build directory from the preset to install conan files there.
source_dir = Path(args.source_dir)
cmake_presets = CMakePresets(source_dir / "CMakePresets.json")

devmode_preset_suffix = "-devmode" if args.dev else ""
build_dir_preset_name = f"ptsl-dir-{platform.system()}-{args.arch}{devmode_preset_suffix}"

if args.dev and args.library_type == "static":
    build_dir_preset_name += "-static"

target_preset = next((p for p in cmake_presets.presets["configurePresets"] if p["name"] == build_dir_preset_name), None)
if not target_preset:
    print(f"Error: Can't find preset '{build_dir_preset_name}' for current configuration at '{cmake_presets.path}'")
    sys.exit(1)

build_dir = target_preset["binaryDir"].replace("${sourceDir}", str(source_dir))

# Conan remote setup.
conan = ConanApi()
remotes = {
    'conan-avid' : ['http://neon-nexus-mtl-1.global.avidww.com:8081/repository/conan-video/', False], 
    'conancenter' : ['https://center.conan.io', True]
    }
for i in remotes:
    if i not in conan.get_remote_list():
        conan.remote_add(i, remotes[i][0], verify_ssl=remotes[i][1])
        print(f'{i} remote is added')
remote = 'conan-avid' if args.dev else 'conancenter'
print(f'Using {remote}')

# Running conan install for each requested architecture.
dependencies_dir = Path(build_dir) / "Dependencies"

arch_settings_mapping = { 
    "arm64": [{ "conan_arch": "armv8", "cmake_arch": "arm64", "folder": dependencies_dir }],
    "x86_64": [{ "conan_arch": "x86_64", "cmake_arch": "x86_64", "folder": dependencies_dir }],
    "x86_64_arm64": [
        { "conan_arch": "armv8", "cmake_arch": "arm64", "folder": dependencies_dir / "armv8" },
        { "conan_arch": "x86_64", "cmake_arch": "x86_64", "folder": dependencies_dir / "x86_64" }
    ] 
}

dependencies_arch_settings = arch_settings_mapping[args.arch]

print("\nChecking Conan...")
which_conan_result = subprocess.run(["where" if platform.system() == "Windows" else "which", "conan"], check=True, capture_output=True, text=True)
conan_version_result = subprocess.run(["conan", "--version"], check=True, capture_output=True, text=True)
print(f"{conan_version_result.stdout}Conan path: {which_conan_result.stdout}")

# Install the conan settings.yml file.
conan_settings_yml = Path(args.conan_settings_yml)
if conan_settings_yml.exists():
    subprocess.run(["conan", "config", "install", str(conan_settings_yml)], check=True)

for setting in dependencies_arch_settings:
    conan_cmd = [
        'conan', 'install', source_dir,
        '-if', setting["folder"],
        '-s:h', f'arch={setting["conan_arch"]}',
        '-s:h', f'build_type={args.config}',
        '-pr:b=default',
        '--build', 'missing',
        '--remote', remote
    ]

    if args.os:
        conan_cmd += ['-s:h', f'os.version={args.os}', '-s:b', f'os.version={args.os}']

    # We already know the environment, force it instead of trying to guess.
    # For non-dev builds... just hope the VS environment is set up.
    vs_version_map = { "vs2017": { "version": "15", "toolset": "v141" }, "vs2019": { "version": "16", "toolset": "v142" }, "vs2022": { "version": "17", "toolset": "v143" } }
    if platform.system() == "Windows":
        vs_compiler_version = vs_version_map[args.vs_compiler]["version"]
        conan_cmd += ['-s:h', f'compiler.version={vs_compiler_version}', '-s:b', f'compiler.version={vs_compiler_version}']
        vs_toolset_version = args.vs_toolset or vs_version_map[args.vs_compiler]["toolset"]
        conan_cmd += ['-s:h', f'compiler.toolset={vs_toolset_version}', '-s:b', f'compiler.toolset={vs_toolset_version}']
        conan_cmd += ['--conf', 'tools.cmake.cmaketoolchain:toolset_arch=x64']

    print(f"Running Conan command: {conan_cmd}")
    subprocess.run(conan_cmd, check=True)

    # Copy the conanbuildinfo.txt to a more descriptive name, so we have informantion about all installed dependencies per architecture.
    shutil.copy(setting["folder"] / "conanbuildinfo.txt", setting["folder"] / f"conanbuildinfo-{setting['conan_arch']}-{args.config}.txt")

# Patching CMake files generated by Conan.
if platform.system() == "Darwin":
    patch_cmake(dependencies_dir, dependencies_arch_settings)

# Keep the name simple and short. We differentiate build folders only by arch and dev mode. So, it's better to rewrite old entries than keep incorrect ones.
preset_name = f"ptsl-{platform.system()}-{args.arch}{devmode_preset_suffix}" 
build_preset_name = f"ptsl-{platform.system()}-{args.arch}-{args.config}{devmode_preset_suffix}"

os_version_display_name = f" {args.os}" if args.os else ""
devmode_display_name = " dev" if args.dev else ""
preset_display_name = f"PTSL{devmode_display_name} preset for {platform.system()}{os_version_display_name} {args.arch} (Generated)"
build_preset_display_name = f"PTSL{devmode_display_name} preset for {platform.system()}{os_version_display_name} {args.arch} {args.config} (Generated)"

# Craft CMakeUserPresets.json based on passed settings. That sets up a project that can be built/opened with a single command.
user_presets = CMakePresets(source_dir / "CMakeUserPresets.json")

configure_inherits = ["base", build_dir_preset_name]
if args.dev:
    configure_inherits += ["devmode"]
if args.library_type == "static":
    configure_inherits += ["ptsl-static"]

cache_variables = {}
if platform.system() == "Darwin":
    configure_inherits += ["xcode"]
    if args.os:
        cache_variables["CMAKE_OSX_DEPLOYMENT_TARGET"] = args.os
elif platform.system() == "Windows":
    configure_inherits += [f"{args.vs_compiler}"]

conf_preset = {
    "name": preset_name,
    "inherits": configure_inherits,
    "displayName": preset_display_name,
    "cacheVariables": cache_variables
}
if platform.system() == "Windows":
    conf_preset["toolset"] = {
        "value": args.vs_toolset or vs_version_map[args.vs_compiler]["toolset"]
    }
user_presets.add_configure_preset(conf_preset)

user_presets.add_build_preset({
    "name": build_preset_name,
    "configurePreset": preset_name,
    "displayName": build_preset_display_name,
    "targets": ["PTSL_INSTALL"],
    "configuration": args.config
})

user_presets.add_workflow_preset({
    "name": build_preset_name,
    "steps": [
        { "type": "configure", "name": preset_name },
        { "type": "build", "name": build_preset_name }
    ],
    "displayName": build_preset_display_name
})

user_presets.save()

# Do the build.
print(f"Invoking CMake preset '{build_preset_name}'")
subprocess.run(["cmake", "--workflow", "--preset", build_preset_name], cwd=source_dir, check=True)
