# Copyright 2022-2023 by Avid Technology, Inc.
# CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

from datetime import datetime
import os
import json
import re

cwd = os.getcwd()

def get_ptsl_version() -> str:
    ptsl_version_file = os.path.join(cwd, '..', '..', '..', 'Common', 'CPP', 'Source', 'PTSL_Versions.h')
    with open(ptsl_version_file, 'r') as f:
        matches = re.finditer("PTSL_VERSION\s*=\s*(\d+);", f.read(), re.MULTILINE)
        version = str([match[1] for match in matches]).translate(str.maketrans('', '', '\'[]'))
        return f"{version}.0"

def gen_version_json():
    root_dir = os.path.join(cwd, '..', '..', '..', '..', '..',)
    pt_version = get_pt_current_version(os.path.join(root_dir,'.gitlab-avid', 'ptsw-config', 
                                                    'environmental_variables.json'))
    version_data = {
        "pt_version" : pt_version,
        "pipeline_id" : os.environ.get("CI_PIPELINE_ID"),
        "ptsl_version" : get_ptsl_version()
    }
    with open('ptsl_package_version.json', 'w+') as f:
        json.dump(version_data, f)

def get_pt_current_version(file) -> str:
    with open(file, 'r') as f:
        return json.load(f)['GLOBAL_DIGI_CURRENT_VERSION'].rsplit('.',1)[0]

if __name__ == '__main__':
        gen_version_json()
