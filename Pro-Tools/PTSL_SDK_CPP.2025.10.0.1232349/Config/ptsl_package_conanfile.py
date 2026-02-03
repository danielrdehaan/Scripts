# Copyright 2022-2023 by Avid Technology, Inc.
# CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

from conans import ConanFile
import os
import json
import re

cwd = os.getcwd()

if ".conan" in cwd:
    with open(os.path.join('..', 'export_source', 'setup', 'ptsl_package_version.json')) as f:
        version_data = json.load(f)
    file_list = [ "../*", "!MacBuild/*", "!WinBuild/*", "!Config/conanfile.py", "!*.pyc", "!conan_upload.*",  "!examples/build/*", "!examples/ptslcmd/MacBuild/*",
    "!examples/ptslcmd/WinBuild/*", "!dependencies/*", "!setup/gen_version_cfg_cpp_ptsl_sdk.py", "!setup/ptsl_package_version.json"]
else:
    with open(os.path.join('..', 'setup', 'ptsl_package_version.json')) as f:
        version_data = json.load(f)
    file_list = [ "../*", "!MacBuild/*", "!WinBuild/*", "!Config/conanfile.py", "!*.pyc", "!conan_upload.*",  "!examples/build/*", "!examples/ptslcmd/MacBuild/*",
    "!examples/ptslcmd/WinBuild/*", "!dependencies/*", "!setup/gen_version_cfg_cpp_ptsl_sdk.py"]


if version_data['pipeline_id']:
    current_version = f"{version_data['time_stamp']}.{version_data['ptsl_version']}.{version_data['pipeline_id']}"
else:
    current_version = f"{version_data['time_stamp']}.{version_data['ptsl_version']}"

cwd = os.getcwd()

if ".conan" in cwd:
    ptsl_version_file = os.path.join(cwd, '..', 'export_source', 'Source', 'PTSL_Versions.h')
else:
    ptsl_version_file = os.path.join(cwd, '..', 'Source', 'PTSL_Versions.h')
    # Dev path to a version file for debugging
    # ptsl_version_file = os.path.join(cwd, '..', '..', '..', 'Common', 'CPP', 'Source', 'PTSL_Versions.h')

def get_package_version():
    with open(ptsl_version_file, 'r') as f:
        matches = re.finditer("PTSL_VERSION\s*=\s*(\d+);", f.read(), re.MULTILINE)
        version = str([match[1] for match in matches]).translate(str.maketrans('', '', '\'[]'))
        return f"{version}.0"

class PTSLConan(ConanFile):
    name = "ptslsdk"
    version = current_version
    description = "Pro Tools Scripting Lybrary C++"
    exports_sources =  file_list

    def package_info(self):
        self.cpp_info.libs = ["PTSL_SDK_CPP"]

