
import os

def main():
  all_file = os.listdir()
  
  no_delete = ["Makefile", "my_driver.c", "clean.py"]
  
  for f in all_file:
    if f not in no_delete:
      os.remove(f)

if __name__=="__main__":
  main()