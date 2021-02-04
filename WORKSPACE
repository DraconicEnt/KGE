"""
    This software is licensed under the Draconic Free License version 1. Please refer
    to LICENSE.txt for more information.

    Copyright (c) 2021 Robert MacGregor
"""

workspace(name="KGE")

local_repository(
    name = "rules_third_party",
    path = "/home/ragora/Documents/Projects/DraconicEntity/rules_third_party"
)

load("//:repositories.bzl", "repositories")
repositories()

load("//:dependencies.bzl", "dependencies")
dependencies()
