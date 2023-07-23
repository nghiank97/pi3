import os


file = os.open("/proc/my_value/value", os.O_RDWR)

line = str.encode("khac nghia")
os.write(file, line)

rx_date = os.read(file,11)
print(" Data ",rx_date)
os.close(file)
