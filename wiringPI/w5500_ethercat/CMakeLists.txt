
cmake_minimum_required(VERSION 3.18.4)
project (w5500_ethercat)
add_executable(w5500_ethercat
        main.c
        ethercat.c
        socket.c
        w5500.c
        wizchip_conf.c
        )
add_library(main.c ethercat.c socket.c w5500.c wizchip_conf.c)
target_link_libraries(w5500_ethercat wiringPi) 
