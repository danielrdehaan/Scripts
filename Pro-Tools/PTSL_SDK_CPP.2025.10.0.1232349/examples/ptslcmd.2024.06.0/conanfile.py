# Copyright 2022-2024 by Avid Technology, Inc.
# CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

from conans import ConanFile
from conan.tools.cmake import CMakeDeps, CMakeToolchain


class PtslCmdConan(ConanFile):
    name = "PtslCmd"
    settings = "os", "arch", "compiler", "build_type"

    def requirements(self):
        self.requires("gtest/1.10.0")
        self.requires("boost/1.83.0")
        self.requires("zlib/1.2.13")
        self.requires("nlohmann_json/3.11.3")
        self.requires("date/3.0.1")

    def configure(self):
        self.options["boost"].header_only = True 
        self.options["date"].header_only = True 

    def build_requirements(self):
        self.tool_requires("grpc/1.43.0")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.user_presets_path = False
        tc.generate()

        cmake = CMakeDeps(self)
        cmake.generate()
