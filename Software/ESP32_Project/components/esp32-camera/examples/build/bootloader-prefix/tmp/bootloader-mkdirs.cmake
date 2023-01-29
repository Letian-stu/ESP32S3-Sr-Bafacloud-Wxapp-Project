# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/B_Code/espidf/esp32idf/esp-idf/components/bootloader/subproject"
  "D:/Desktop/Http_cam/components/esp32-camera/examples/build/bootloader"
  "D:/Desktop/Http_cam/components/esp32-camera/examples/build/bootloader-prefix"
  "D:/Desktop/Http_cam/components/esp32-camera/examples/build/bootloader-prefix/tmp"
  "D:/Desktop/Http_cam/components/esp32-camera/examples/build/bootloader-prefix/src/bootloader-stamp"
  "D:/Desktop/Http_cam/components/esp32-camera/examples/build/bootloader-prefix/src"
  "D:/Desktop/Http_cam/components/esp32-camera/examples/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Desktop/Http_cam/components/esp32-camera/examples/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
