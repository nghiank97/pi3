
# 1. Install autoconf, libtool, autogen.sh

```cmd
git clone https://github.com/stephane/libmodbus.git
cd libmodbus
```
```cmd
sudo apt install autoconf
sudo apt install libtool
```
```cmd
./autogen.sh
./configure
make
sudo make install
```

![image info](./image/autoconf.png)
```cmd
./autogen.sh
```
![image info](./image/libtool.png)
```cmd
./configure
```
![image info](./image/configure.png)
```cmd
make
```
![image info](./image/make.png)
```cmd
sudo make install
```
![image info](./image/make_install.png)

```cmd
ls -l /usr/local/lib/
```
![image info](./image/make_install.png)
```cmd
sudo cp /usr/local/lib/libmodbus.so ./
sudo ldconfig
```

# 2. Result

![image info](./image/register.png)