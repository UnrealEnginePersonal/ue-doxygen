from typing import List

import argparse
import click
import os

class LibCombiner:
    """
    Combine multiple libraries (.lib) into a single file using the lib.exe tool.
    """

    def __init__(self, libs: List[str], generated_out_file_path: str):
        self.libs = libs
        self.generated_out_file_path = generated_out_file_path

    def combine(self):
        cmd = f"lib.exe /OUT:{self.generated_out_file_path}"
        for lib in self.libs:
            if not os.path.exists(lib):
                raise FileNotFoundError(f"Library not found: {lib}")

            cmd += f" {lib}"
        print(f"Running command: {cmd}")
        os.system(cmd)


def find_libs_in_dir(directory: str) -> List[str]:
    return [os.path.join(directory, f) for f in os.listdir(directory) if f.endswith(".lib")]

@click.command()
@click.option("--ldir", "-l", help="Directory containing libraries", required=True)
@click.option("--out", "-o", help="Output file", required=True)
def cli(ldir, out):
    found_libs_in_dir: List[str] = find_libs_in_dir(ldir)
    print(f"Found libraries in directory: {found_libs_in_dir}")

    if not out.endswith(".lib"):
        out += ".lib"

    lib_combiner = LibCombiner(found_libs_in_dir, out)
    lib_combiner.combine()

if __name__ == "__main__":
    cli()
