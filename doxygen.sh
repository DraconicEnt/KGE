#!/bin/bash
DOXYGEN_BINARY="$BUILD_WORKSPACE_DIRECTORY/bazel-bin/$2"
DOT_BINARY="$BUILD_WORKSPACE_DIRECTORY/bazel-bin/$8"
DOT_LIBS="$(realpath $(dirname $DOT_BINARY)/../lib)"

# Set library path and GVBINDIR so that DOT runs correctly
export LD_LIBRARY_PATH="$DOT_LIBS"
export GVBINDIR="$DOT_LIBS"

# Set config file to reference bazel's dot binary
CONFIG=$(mktemp)
cp ./doxyfile $CONFIG
sed -i "s?DOT_PATH               =?DOT_PATH = $DOT_BINARY?g" $CONFIG

# Generate config file to ensure plugins are loaded by dot
$DOT_BINARY -c

# doxyfile is located here
cd "$BUILD_WORKSPACE_DIRECTORY"

$DOXYGEN_BINARY $CONFIG
