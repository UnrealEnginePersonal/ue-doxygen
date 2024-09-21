from typing import List


class LibCombiner:
    """
    Combine multiple libraries (.lib) into a single file using the lib.exe tool.
    """

    def __init__(self, libs: List[str], generated_out_file_path: str):
        self.libs = libs
        self.generated_out_file_path = generated_out_file_path

    def combine(self):
        # Combine libraries
        cmd = f"lib.exe /OUT:{self.generated_out_file_path} {' '.join(self.libs)}"
        os.system(cmd)


import argparse
import click
import os

class ConvertStrToList(click.Option):
    def type_cast_value(self, ctx, value) -> List:
        try:
            value = str(value)
            assert value.count('[') == 1 and value.count(']') == 1
            list_as_str = value.replace('"', "'").split('[')[1].split(']')[0]
            list_of_items = [item.strip().strip("'") for item in list_as_str.split(',')]
            return list_of_items
        except Exception:
            raise click.BadParameter(value)

@click.command()
@click.option("--libs", "-l", cls=ConvertStrToList, help="List of libraries to combine", default=[])
@click.option("--out", "-o", help="Output file", required=True)
def cli(libs, out):
    combiner = LibCombiner(libs, out)
    combiner.combine()


if __name__ == "__main__":
    cli()
