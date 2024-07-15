package(default_visibility = ["//visibility:public"])

LINK_OPTS = [
    "-shared",
    "-fPIC",
]

cc_library(
    name = "init",
    srcs = ["init.c"],
    linkopts = LINK_OPTS,
    linkstatic = False,
)

cc_library(
    name = "no_init",
    srcs = ["no_init.c"],
    linkopts = LINK_OPTS,
    linkstatic = False,
)

cc_library(
    name = "init_fail",
    srcs = ["init_fail.c"],
    linkopts = LINK_OPTS,
    linkstatic = False,
)

cc_library(
    name = "stream_init",
    srcs = ["stream_init.c"],
    linkopts = LINK_OPTS,
    linkstatic = False,
)

cc_library(
    name = "get_headers",
    srcs = ["get_headers.c"],
    linkopts = LINK_OPTS,
    linkstatic = False,
)
