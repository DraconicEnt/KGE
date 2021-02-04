load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

def repositories():
    maybe(
        git_repository,
        name = "rules_third_party",
        remote = "https://github.com/DraconicEnt/rules_third_party.git",
        commit = "ad57b1e73c8990b63cea0abbac30bbbf4fb74e84"
    )

    # Foreign CC
    maybe(
        http_archive,
        name = "rules_foreign_cc",
        strip_prefix = "rules_foreign_cc-master",
        # sha256 = "3e6b0691fc57db8217d535393dcc2cf7c1d39fc87e9adb6e7d7bab1483915110",
        url = "https://github.com/bazelbuild/rules_foreign_cc/archive/master.zip",
    )

    # Load EasyDelegate
    maybe(
        git_repository,
        name = "easydelegate",
        shallow_since = "1607857580 -0800",
        remote = "https://github.com/Ragora/EasyDelegate.git",
        commit = "68f60feb91025bc354ae67271f5e671800f782cd"
    )

    # Load rules_python
    #http_archive(
    #    name = "rules_python",
    #    url = "https://github.com/bazelbuild/rules_python/releases/download/0.1.0/rules_python-0.1.0.tar.gz",
    #    sha256 = "b6d46438523a3ec0f3cead544190ee13223a52f6a6765a29eae7b7cc24cc83a0",
    #)

    #load("@rules_python//python:pip.bzl", "pip_repositories")
    #pip_repositories()

    # Load rules_pkg
    maybe(
        http_archive,
        name = "rules_pkg",
        url = "https://github.com/bazelbuild/rules_pkg/releases/download/0.3.0/rules_pkg-0.3.0.tar.gz",
        sha256 = "6b5969a7acd7b60c02f816773b06fcf32fbe8ba0c7919ccdc2df4f8fb923804a",
    )

    # Load Google Test
    maybe(
        git_repository,
        name = "gtest",
        remote = "https://github.com/google/googletest.git",
        # 1.10.0
        commit = "703bd9caab50b139428cea1aaff9974ebee5742e",
        shallow_since = "1570114335 -0400"
    )

    maybe(
        git_repository,
        name = "easylua",
        remote = "https://github.com/Ragora/EasyLua.git",
        commit = "18381fc5ba4416067117dd162afda63dc6b7951c"
    )
