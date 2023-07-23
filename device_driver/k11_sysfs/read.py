import os


file = os.open("/sys/kernel/etx_sysfs/etx_value", os.O_RDWR)

# line = str.encode("khac nghia")
# os.write(file, line)

rx_date = os.read(file, 256)
print(" Data ",rx_date)
os.close(file)