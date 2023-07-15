
import os

def main():
  all_file = os.listdir()
  
  for f in all_file:
    if f == "Makefile" or f == "Readme.md":
       continue
    if (".c" in f or ".py" in f or ".dts" in f) and f.count('.') == 1:
        continue
    os.remove(f)
    
if __name__=="__main__":
  main()