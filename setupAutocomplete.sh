#!/usr/bin/bash

mkdir -p $BUILD_WORKSPACE_DIRECTORY/autocomplete
cp -R $BUILD_WORKSPACE_DIRECTORY/bazel-KGE/external/enet/enet-1.3.17 $BUILD_WORKSPACE_DIRECTORY/autocomplete
cp -R $BUILD_WORKSPACE_DIRECTORY/bazel-KGE/external/easydelegate $BUILD_WORKSPACE_DIRECTORY/autocomplete
cp -R $BUILD_WORKSPACE_DIRECTORY/bazel-KGE/external/osg $BUILD_WORKSPACE_DIRECTORY/autocomplete
