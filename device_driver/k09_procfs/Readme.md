
# 1. Write

``` make
sudo echo "hello world" > /proc/my_value/value
```

or

``` cpp
write(fd, write_buff, "hello world", 11); 
```

# 2. Read

``` cpp
read(fd, read_buff, 256); 
```
