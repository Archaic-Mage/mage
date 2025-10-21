#!/usr/bin/env bash
# Generate compile_commands.json using Hedron's Bazel compile commands extractor
# This disables parse_headers feature to avoid assertion errors with header-only files

set -e

bazel run @hedron_compile_commands//:refresh_all -- \
    --features=-parse_headers \
    --host_features=-parse_headers

echo "Successfully generated compile_commands.json"
