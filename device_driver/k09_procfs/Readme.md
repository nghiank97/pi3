
# 1. Write and read

``` make
sudo echo "hello world" > /proc/my_value/value
```

or (Cpp)

``` cpp
write(fd, write_buff, "hello world", 11); 
```

``` cpp
read(fd, read_buff, 256); 
```

or (Python)

``` python
import os
file = os.open("/proc/my_value/value", os.O_RDWR)
line = str.encode("khac nghia")
os.write(file, line)
rx_date = os.read(file,11)
print(" Data ",rx_date)
os.close(file)
```