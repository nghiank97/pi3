import os
import subprocess


os.system("echo 222 > /sys/kernel/etx_sysfs/etx_value")

# result = subprocess.run(['echo', '123 > /sys/kernel/etx_sysfs/etx_value'], stdout=subprocess.PIPE)
# print(result.stdout)


result = subprocess.run(['cat', '/sys/kernel/etx_sysfs/etx_value'], stdout=subprocess.PIPE)
print(result.stdout)
