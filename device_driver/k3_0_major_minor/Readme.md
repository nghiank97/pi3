<span style="font-family: Courier New">

## 1. Major & minor
```shell
$ cat /proc/devices
```
![image info](./Img/get_major_minor.png)

## 2. Declaer the major & minor number
Include the library
```c
    #include <linux/fs.h>
```
Declare the number
```c
    dev_t dev = MKDEV(7, 0);
```
![image info](./Img/get_major_minor.png)
## 3. Info
![image info](./Img/info.png)
## 4. Result
```c
register_chrdev_region(dev, 1, "k21_dev");
```
![image info](./Img/result.png)
## 4. Delete
```c
register_chrdev_region(dev, 1, "k21_dev");
```
![image info](./Img/result.png)
</span>