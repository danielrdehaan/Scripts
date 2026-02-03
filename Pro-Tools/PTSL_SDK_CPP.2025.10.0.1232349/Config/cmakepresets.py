# Copyright 2023 by Avid Technology, Inc.
import json


class CMakePresets():
    """Simple JSON wrapper around CMake presets to quickly edit it"""

    def __init__(self, path):
        self.path = path
        if path.exists():
            with open(path) as f:
                try:
                    self.presets = json.load(f)
                except Exception as e:
                    raise Exception(f"Exception while loading {path}. Please remove or fix the file. {e}")
        else:
            self.presets = {
                "version": 6,
                "configurePresets": [],
                "buildPresets": [],
                "testPresets": [],
                "packagePresets": [],
                "workflowPresets": []
            }

    def add_configure_preset(self, preset):
        self.__add_preset("configurePresets", preset)

    def add_build_preset(self, preset):
        self.__add_preset("buildPresets", preset)

    def add_workflow_preset(self, preset):
        self.__add_preset("workflowPresets", preset)

    def save(self):
        with open(self.path, 'w', encoding='utf-8') as f:
            json.dump(self.presets, f, ensure_ascii=False, indent=4)

    def find_configure_preset(self, name):
        return self.__find_preset("configurePresets", name)

    def find_build_preset(self, name):
        return self.__find_preset("buildPresets", name)

    def find_workflow_preset(self, name):
        return self.__find_preset("workflowPresets", name)

    def __find_preset(self, preset_type, preset_name):
        return next((p for p in self.presets[preset_type] if p["name"] == preset_name), None)

    def __add_preset(self, preset_type, preset):
        if preset_type not in self.presets:
            self.presets[preset_type] = []

        for i, p in enumerate(self.presets[preset_type]):
            if p["name"] == preset["name"]:
                self.presets[preset_type][i] = preset
                return

        self.presets[preset_type].append(preset)
