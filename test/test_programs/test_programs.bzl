load("@rules_cc//cc:defs.bzl", "cc_library")

def test_program(name):
    cc_library(
        name = name,
        srcs = [name + ".c", "//abi:abi.h"],
        linkopts = [
            "-shared",
            "-fPIC",
        ],
        linkstatic = False,
    )
