load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

def repositories():
    maybe(
        git_repository,
        name = "rules_third_party",
        remote = "https://github.com/DraconicEnt/rules_third_party.git",
        commit = "c420260becd3c31c145af386025d51d06287ab9c"
    )

    # Foreign CC
    maybe(
        http_archive,
        name = "rules_foreign_cc",
        sha256 = "d54742ffbdc6924f222d2179f0e10e911c5c659c4ae74158e9fe827aad862ac6",
        strip_prefix = "rules_foreign_cc-0.2.0",
        url = "https://github.com/bazelbuild/rules_foreign_cc/archive/0.2.0.tar.gz"
    )

    # Load EasyDelegate
    maybe(
        git_repository,
        name = "easydelegate",
        shallow_since = "1607857580 -0800",
        remote = "https://github.com/Ragora/EasyDelegate.git",
        commit = "68f60feb91025bc354ae67271f5e671800f782cd"
    )

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
