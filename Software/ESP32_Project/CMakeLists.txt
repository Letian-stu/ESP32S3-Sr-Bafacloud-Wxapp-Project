# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(EXTRA_COMPONENT_DIRS 
    ${CMAKE_CURRENT_SOURCE_DIR}/components/speech_recognition
    ${CMAKE_CURRENT_SOURCE_DIR}/components/button
    ${CMAKE_CURRENT_SOURCE_DIR}/components/esp_cmd
    ${CMAKE_CURRENT_SOURCE_DIR}/components/i2c_master
    ${CMAKE_CURRENT_SOURCE_DIR}/components/aht20
    )

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(main)
