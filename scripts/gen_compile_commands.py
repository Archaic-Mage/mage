#!/usr/bin/env python3
"""Generate compile_commands.json from Bazel build information."""

import json
import os
import subprocess
import sys

COMPILE_COMMANDS_FILE = "compile_commands.json"


def load_deps_from_file(deps_file, base_path):
    """Load dependencies from a .d file.

    Args:
        deps_file: Path to the dependencies file
        base_path: Base path for relative dependencies

    Returns:
        List of dependency file paths
    """
    deps_list = []
    if not os.path.exists(deps_file):
        return deps_list
    with open(deps_file, "r", encoding="utf-8") as f:
        for line in f:
            split = line.split(" ")
            for item in split:
                if len(item) <= 1:
                    continue
                if item[0] == "/":
                    if os.path.exists(item):
                        deps_list.append(item)
                else:
                    if os.path.exists(f"{base_path}/{item}"):
                        deps_list.append(item)
    return deps_list


def load_existing_commands():
    """Load existing compile commands from file.

    Returns:
        Dictionary mapping file paths to command entries
    """
    commands = {}
    if os.path.exists(COMPILE_COMMANDS_FILE):
        with open(COMPILE_COMMANDS_FILE, "r", encoding="utf-8") as f:
            commands_arr = json.load(f)
        for entry in commands_arr:
            commands[entry["file"]] = entry
    return commands


def run_bazel_aquery(query):
    """Run bazel aquery and return JSON output.

    Args:
        query: Bazel query string

    Returns:
        Parsed JSON from bazel aquery output
    """
    command = [
        "bazel",
        "aquery",
        "--features=-compiler_param_file",
        "--output=jsonproto",
        "--keep_going",
        f'mnemonic("CppCompile", {query})',
    ]

    res_string = ""
    with subprocess.Popen(
        command, stdout=subprocess.PIPE, stderr=sys.stderr, bufsize=1, encoding="utf-8"
    ) as p:
        for line in p.stdout:  # pyright: ignore[reportOptionalIterable]
            res_string += line[:-1]

    return json.loads(res_string)


def parse_compile_action(entry, base_path):
    """Parse a single compile action from bazel aquery output.

    Args:
        entry: Action entry from bazel aquery
        base_path: Base path for the build

    Returns:
        Tuple of (fields dict, deps_list)
    """
    args = entry["arguments"]
    fields = {
        "command": " ".join(args),
        "directory": base_path,
    }
    deps_list = []

    for i, arg in enumerate(args):
        if arg == "-c":
            fields["file"] = args[i + 1]
        elif arg == "-MF":
            deps_file = args[i + 1]
            deps_list = load_deps_from_file(deps_file, base_path)

    return fields, deps_list


def build_header_command(fields, filename):
    """Build compile command for header file.

    Args:
        fields: Command fields dictionary
        filename: Header filename

    Returns:
        Updated fields dictionary
    """
    cmd_parts = []
    cmd_split = fields["command"].split()
    skip_next = False

    for word in cmd_split:
        if skip_next:
            skip_next = False
            continue
        if word in ("-c", "-o", "-MF"):
            skip_next = True
            continue
        if word == "-MD":
            continue
        cmd_parts.append(word)

    fields["command"] = " ".join(cmd_parts) + " -x c++-header " + filename
    fields["file"] = filename
    return fields


def main():
    """Main entry point for compile commands generation."""
    if not os.path.exists("MODULE.bazel"):
        sys.exit(
            "No WORKSPACE file detected - this script must run from the project root"
        )

    base_path = os.path.abspath(f"bazel-{os.path.basename(os.getcwd())}")
    query = " ".join(sys.argv[1:]) if len(sys.argv) > 1 else "//..."

    commands = {}
    if query != "//...":
        commands = load_existing_commands()

    res_json = run_bazel_aquery(query)

    # Process compile actions
    files = {}
    for entry in res_json["actions"]:
        fields, deps_list = parse_compile_action(entry, base_path)
        for dep in deps_list[1:]:
            files[dep] = fields.copy()
        commands[fields["file"]] = fields
        print(fields["file"])

    # Process header files
    for filename, fields in files.items():
        print(filename)
        commands[filename] = build_header_command(fields.copy(), filename)

    # Write output
    with open(COMPILE_COMMANDS_FILE, "w", encoding="utf-8") as outfile:
        json.dump(list(commands.values()), outfile, indent=4)
    print("Done")


if __name__ == "__main__":
    main()
