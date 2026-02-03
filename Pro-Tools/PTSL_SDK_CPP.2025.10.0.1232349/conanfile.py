# Copyright 2022-2024 by Avid Technology, Inc.
# CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

from conans import ConanFile
from conan.tools.cmake import CMakeDeps, CMakeToolchain


class PtslClientConan(ConanFile):
    name = "PtslClient"
    settings = "os", "arch", "compiler", "build_type"

    def requirements(self):
        self.requires("grpc/1.43.0")
        self.requires("date/3.0.1")
        self.requires("nlohmann_json/3.11.3")

    def build_requirements(self):
        self.tool_requires("grpc/1.43.0")

    def configure(self):
        self.options["date"].header_only = True 

    def generate(self):
        tc = CMakeToolchain(self)
        tc.user_presets_path = False # TODO: What's the best way to merge that with our presets... or maybe just ignore
        tc.generate()

        cmake = CMakeDeps(self)
        cmake.generate()
