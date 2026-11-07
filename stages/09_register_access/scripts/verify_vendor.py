"""Verify the vendored NT-Shell bytes, optionally against its official archive."""

import argparse
import hashlib
from pathlib import Path
import tarfile

ARCHIVE_SHA256 = "ea1497ce208cf9e3d02c161fd5d72e0e785b34074e31dca34fe855ba8fd89c74"


def sha256(data):
    return hashlib.sha256(data).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--archive", type=Path)
    args = parser.parse_args()
    vendor = Path(__file__).resolve().parents[1] / "third_party" / "ntshell"
    entries = []
    for line in (vendor / "SHA256SUMS").read_text(encoding="ascii").splitlines():
        expected, relative = line.split("  ", 1)
        actual = (vendor / relative).read_bytes()
        if sha256(actual) != expected:
            raise SystemExit(f"Vendor checksum mismatch: {relative}")
        entries.append((relative, actual))
    if args.archive:
        if sha256(args.archive.read_bytes()) != ARCHIVE_SHA256:
            raise SystemExit("Archive checksum mismatch")
        with tarfile.open(args.archive, "r:gz") as archive:
            for relative, actual in entries:
                with archive.extractfile("src/lib/" + relative) as original:
                    if original.read() != actual:
                        raise SystemExit(f"Archive content mismatch: {relative}")
    print(f"PASS: {len(entries)} NT-Shell files match" +
          (" the official archive and checksums" if args.archive else " checksums"))


if __name__ == "__main__":
    main()
