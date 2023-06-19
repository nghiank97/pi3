
cmake_minimum_required(VERSION 3.18.4)
project (tft_1_8in)
add_executable(tft_1_8in
        main.c
        st7735.c
        st7735_graphics.c
        st7735_text.c
        )
add_library(main.c st7735.c st7735_graphics.c st7735_text.c)
target_link_libraries(tft_1_8in wiringPi) 
