
import os
import argparse

parser = argparse.ArgumentParser(
	prog="GPIO",
	description="Clean"
)

parser.add_argument('-f',type=str)
args = parser.parse_args()


def main(arg):
  all_file = os.listdir()
  no_delete = ["Makefile",args.f, "clean.py"]
  for f in all_file:
    if f not in no_delete:
      os.remove(f)

if __name__=="__main__":
  main(args)
