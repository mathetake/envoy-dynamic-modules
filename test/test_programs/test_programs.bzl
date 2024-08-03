load("@rules_cc//cc:defs.bzl", "cc_library")

def test_program(name):
    cc_library(
        name = name,
        srcs = [name + ".c", "//source/extensions/dynamic_modules/abi:abi.h"],
        linkopts = [
            "-shared",
            "-fPIC",
        ],
        linkstatic = False,
    )
