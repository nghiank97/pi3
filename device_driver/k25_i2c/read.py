import os


file = os.open("/proc/lsm303dlhc/value", os.O_RDWR)
rx_date = os.read(file,256)
print(" Data ",rx_date)
os.close(file)