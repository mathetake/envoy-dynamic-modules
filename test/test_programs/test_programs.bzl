load("@rules_cc//cc:defs.bzl", "cc_library")

def test_program(name):
    cc_library(
        name = name,
        srcs = [name + ".c"],
        linkopts = [
            "-shared",
            "-fPIC",
        ],
        deps = [
            "@envoyx-abi//:abi_lib",
        ],
        linkstatic = False,
    )
