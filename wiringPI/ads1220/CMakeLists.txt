
cmake_minimum_required(VERSION 3.18.4)
project (ads1220)
add_executable(
        main.c
        ads1220.c
        )
add_library(main.c ads1220.c)
target_link_libraries(ads1220 wiringPi) 
