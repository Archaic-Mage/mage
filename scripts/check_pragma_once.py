#!/usr/bin/env python3
"""Pre-commit hook to check that C/C++ header files contain #pragma once."""

import argparse
import sys
from pathlib import Path


def check_pragma_once(filepath: Path) -> bool:
    """Check if file contains #pragma once directive.

    Args:
        filepath: Path to the header file to check

    Returns:
        True if #pragma once is found, False otherwise
    """
    try:
        content = filepath.read_text(encoding="utf-8")
        # Check if #pragma once exists (ignoring whitespace and comments)
        for line in content.splitlines():
            stripped = line.strip()
            # Skip empty lines and C++ style comments
            if not stripped or stripped.startswith("//"):
                continue
            # Check for #pragma once
            if stripped == "#pragma once":
                return True
            # If we hit any other preprocessor directive or code, fail
            if stripped.startswith("#") and "pragma once" not in stripped:
                break
            if not stripped.startswith("#") and not stripped.startswith("/*"):
                break
        return False
    except (OSError, UnicodeDecodeError) as e:
        print(f"Error reading {filepath}: {e}", file=sys.stderr)
        return False


def main() -> int:
    """Main entry point for the pre-commit hook."""
    parser = argparse.ArgumentParser(
        description="Check that header files contain #pragma once"
    )
    parser.add_argument("filenames", nargs="*", help="Filenames to check")
    args = parser.parse_args()

    if not args.filenames:
        return 0

    failed_files = []
    for filename in args.filenames:
        filepath = Path(filename)
        # Only check header files
        if filepath.suffix not in {".h", ".hpp", ".hxx", ".hh"}:
            continue

        if not check_pragma_once(filepath):
            failed_files.append(filename)
            print(f"ERROR: {filename} is missing '#pragma once'", file=sys.stderr)

    if failed_files:
        print(
            f"\n{len(failed_files)} file(s) missing #pragma once directive",
            file=sys.stderr,
        )
        return 1

    return 0


if __name__ == "__main__":
    sys.exit(main())
